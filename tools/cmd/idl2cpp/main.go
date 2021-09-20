package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"reflect"
	"strings"
	"text/template"
	"unicode"

	"github.com/ben-clayton/webidlparser/ast"
	"github.com/ben-clayton/webidlparser/parser"
)

func main() {
	if err := run(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

func run() error {
	var templatePath string
	var outputPath string
	flag.StringVar(&templatePath, "template", "", "the template file to use with the generator")
	flag.StringVar(&outputPath, "output", "", "the output file")
	flag.Parse()

	out := os.Stdout
	if outputPath != "" {
		file, err := os.Create(outputPath)
		if err != nil {
			return fmt.Errorf("failed to open output file '%v'", outputPath)
		}
		out = file
		defer file.Close()
	}

	tmpl, err := ioutil.ReadFile(templatePath)
	if err != nil {
		return fmt.Errorf("failed to open template file '%v'", templatePath)
	}

	idl := &ast.File{}

	idlFiles := flag.Args()
	for _, path := range idlFiles {
		content, err := ioutil.ReadFile(path)
		if err != nil {
			return fmt.Errorf("failed to open file '%v'", path)
		}
		fileIDL := parser.Parse(string(content))
		if numErrs := len(fileIDL.Errors); numErrs != 0 {
			errs := make([]string, numErrs)
			for i, e := range fileIDL.Errors {
				errs[i] = e.Message
			}
			return fmt.Errorf("errors found while parsing %v:\n%v", path, strings.Join(errs, "\n"))
		}
		idl.Declarations = append(idl.Declarations, fileIDL.Declarations...)
	}

	generated := &strings.Builder{}
	g := generator{t: template.New(templatePath)}
	g.workingDir = filepath.Dir(templatePath)
	g.funcs = map[string]interface{}{
		"AttributesOf":               attributesOf,
		"ConstantsOf":                constantsOf,
		"EnumEntryName":              enumEntryName,
		"Eval":                       g.eval,
		"Include":                    g.include,
		"IsBasicLiteral":             is(ast.BasicLiteral{}),
		"IsConstructor":              isConstructor,
		"IsDefaultDictionaryLiteral": is(ast.DefaultDictionaryLiteral{}),
		"IsDictionary":               is(ast.Dictionary{}),
		"IsEnum":                     is(ast.Enum{}),
		"IsInterface":                is(ast.Interface{}),
		"IsMember":                   is(ast.Member{}),
		"IsNullableType":             is(ast.NullableType{}),
		"IsParametrizedType":         is(ast.ParametrizedType{}),
		"IsRecordType":               is(ast.RecordType{}),
		"IsSequenceType":             is(ast.SequenceType{}),
		"IsTypedef":                  is(ast.Typedef{}),
		"IsTypeName":                 is(ast.TypeName{}),
		"IsUndefinedType":            isUndefinedType,
		"IsUnionType":                is(ast.UnionType{}),
		"Lookup":                     g.lookup,
		"MethodsOf":                  methodsOf,
		"Title":                      strings.Title,
	}
	t, err := g.t.
		Option("missingkey=invalid").
		Funcs(g.funcs).
		Parse(string(tmpl))
	if err != nil {
		return fmt.Errorf("failed to parse template file '%v': %w", templatePath, err)
	}
	idl, declarations := sanitize(idl)
	g.declarations = declarations
	if err := t.Execute(generated, idl); err != nil {
		return err
	}
	fmt.Fprint(out, generated.String())
	return nil
}

type declarations map[string]ast.Decl

func nameOf(n ast.Node) string {
	switch n := n.(type) {
	case *ast.Interface:
		return n.Name
	case *ast.Dictionary:
		return n.Name
	case *ast.Enum:
		return n.Name
	case *ast.Typedef:
		return n.Name
	case *ast.Mixin:
		return n.Name
	case *ast.Includes:
		return ""
	default:
		panic(fmt.Errorf("unhandled AST declaration %T", n))
	}
}

func sanitize(in *ast.File) (*ast.File, declarations) {
	s := sanitizer{
		declarations: declarations{},
		registered:   map[string]bool{},
		out:          &ast.File{},
	}

	{
		interfaces := map[string]*ast.Interface{}
		mixins := map[string]*ast.Mixin{}
		for _, d := range in.Declarations {
			switch d := d.(type) {
			case *ast.Interface:
				if i, ok := interfaces[d.Name]; ok {
					// Merge partial body into one interface
					i.Members = append(i.Members, d.Members...)
				} else {
					clone := *d
					d := &clone
					interfaces[d.Name] = d
					s.declarations[d.Name] = d
				}
			case *ast.Mixin:
				mixins[d.Name] = d
				s.declarations[d.Name] = d
			case *ast.Includes:
				// Merge mixin into interface
				i, ok := interfaces[d.Name]
				if !ok {
					panic(fmt.Errorf("%v includes %v, but %v is not an interface", d.Name, d.Source, d.Name))
				}
				m, ok := mixins[d.Source]
				if !ok {
					panic(fmt.Errorf("%v includes %v, but %v is not an mixin", d.Name, d.Source, d.Source))
				}
				// Merge mixin into the interface
				for _, member := range m.Members {
					if member, ok := member.(*ast.Member); ok {
						i.Members = append(i.Members, member)
					}
				}
			default:
				if name := nameOf(d); name != "" {
					s.declarations[nameOf(d)] = d
				}
			}
		}
	}

	for _, d := range in.Declarations {
		if name := nameOf(d); name != "" {
			s.declare(s.declarations[nameOf(d)])
		}
	}

	return s.out, s.declarations
}

type sanitizer struct {
	declarations declarations
	registered   map[string]bool
	out          *ast.File
}

func (s *sanitizer) declare(d ast.Decl) {
	register := func(name string) bool {
		if s.registered[name] {
			return true
		}
		s.registered[name] = true
		return false
	}
	switch d := d.(type) {
	case *ast.Interface:
		if register(d.Name) {
			return
		}
		if d, ok := s.declarations[d.Inherits]; ok {
			s.declare(d)
		}
		for _, m := range d.Members {
			if m, ok := m.(*ast.Member); ok {
				s.declareType(m.Type)
				for _, p := range m.Parameters {
					s.declareType(p.Type)
				}
			}
		}
	case *ast.Dictionary:
		if register(d.Name) {
			return
		}
		if d, ok := s.declarations[d.Inherits]; ok {
			s.declare(d)
		}
		for _, m := range d.Members {
			s.declareType(m.Type)
			for _, p := range m.Parameters {
				s.declareType(p.Type)
			}
		}
	case *ast.Typedef:
		if register(d.Name) {
			return
		}
		s.declareType(d.Type)
	case *ast.Mixin:
		if register(d.Name) {
			return
		}
		for _, m := range d.Members {
			if m, ok := m.(*ast.Member); ok {
				s.declareType(m.Type)
				for _, p := range m.Parameters {
					s.declareType(p.Type)
				}
			}
		}
	case *ast.Enum:
		if register(d.Name) {
			return
		}
	case *ast.Includes:
		if register(d.Name) {
			return
		}
	default:
		panic(fmt.Errorf("unhandled AST declaration %T", d))
	}

	s.out.Declarations = append(s.out.Declarations, d)
}

func (s *sanitizer) declareType(t ast.Type) {
	switch t := t.(type) {
	case *ast.TypeName:
		if d, ok := s.declarations[t.Name]; ok {
			s.declare(d)
		}
	case *ast.UnionType:
		for _, t := range t.Types {
			s.declareType(t)
		}
	case *ast.ParametrizedType:
		for _, t := range t.Elems {
			s.declareType(t)
		}
	case *ast.NullableType:
		s.declareType(t.Type)
	case *ast.SequenceType:
		s.declareType(t.Elem)
	case *ast.RecordType:
		s.declareType(t.Elem)
	default:
		panic(fmt.Errorf("unhandled AST type %T", t))
	}
}

type generator struct {
	t            *template.Template
	workingDir   string
	funcs        map[string]interface{}
	declarations declarations
}

// eval executes the sub-template with the given name and argument, returning
// the generated output
func (g *generator) eval(template string, args ...interface{}) (string, error) {
	target := g.t.Lookup(template)
	if target == nil {
		return "", fmt.Errorf("template '%v' not found", template)
	}
	sb := strings.Builder{}
	var err error
	if len(args) == 1 {
		err = target.Execute(&sb, args[0])
	} else {
		m := newMap()
		if len(args)%2 != 0 {
			return "", fmt.Errorf("Eval expects a single argument or list name-value pairs")
		}
		for i := 0; i < len(args); i += 2 {
			name, ok := args[i].(string)
			if !ok {
				return "", fmt.Errorf("Eval argument %v is not a string", i)
			}
			m.Put(name, args[i+1])
		}
		err = target.Execute(&sb, m)
	}
	if err != nil {
		return "", fmt.Errorf("while evaluating '%v': %v", template, err)
	}
	return sb.String(), nil
}

func (g *generator) lookup(name string) ast.Decl {
	return g.declarations[name]
}

func (g *generator) include(path string) (string, error) {
	t, err := g.t.
		Option("missingkey=invalid").
		Funcs(g.funcs).
		ParseFiles(filepath.Join(g.workingDir, path))
	if err != nil {
		return "", err
	}
	g.t.AddParseTree(path, t.Tree)
	return "", nil
}

// Map is a simple generic key-value map, which can be used in the template
type Map map[interface{}]interface{}

func newMap() Map { return Map{} }

// Put adds the key-value pair into the map.
// Put always returns an empty string so nothing is printed in the template.
func (m Map) Put(key, value interface{}) string {
	m[key] = value
	return ""
}

// Get looks up and returns the value with the given key. If the map does not
// contain the given key, then nil is returned.
func (m Map) Get(key interface{}) interface{} {
	return m[key]
}

// is returns a function that returns true if the value passed to the function
// matches the type of 'ty'.
func is(ty interface{}) func(interface{}) bool {
	rty := reflect.TypeOf(ty)
	return func(v interface{}) bool {
		ty := reflect.TypeOf(v)
		return ty == rty || ty == reflect.PtrTo(rty)
	}
}

// isConstructor returns true if the object is a constructor ast.Member.
func isConstructor(v interface{}) bool {
	if member, ok := v.(*ast.Member); ok {
		if ty, ok := member.Type.(*ast.TypeName); ok {
			return ty.Name == "constructor"
		}
	}
	return false
}

// isUndefinedType returns true if the type is 'undefined'
func isUndefinedType(ty ast.Type) bool {
	if ty, ok := ty.(*ast.TypeName); ok {
		return ty.Name == "undefined"
	}
	return false
}

func enumEntryName(s string) string {
	return "k" + strings.ReplaceAll(pascalCase(strings.Trim(s, `"`)), "-", "")
}

type Method struct {
	Name      string
	Overloads []*ast.Member
}

func methodsOf(iface *ast.Interface) []*Method {
	byName := map[string]*Method{}
	out := []*Method{}
	for _, member := range iface.Members {
		member := member.(*ast.Member)
		if !member.Const && !member.Attribute && !isConstructor(member) {
			if method, ok := byName[member.Name]; ok {
				method.Overloads = append(method.Overloads, member)
			} else {
				method = &Method{
					Name:      member.Name,
					Overloads: []*ast.Member{member},
				}
				byName[member.Name] = method
				out = append(out, method)
			}
		}
	}
	return out
}

func attributesOf(iface *ast.Interface) []*ast.Member {
	out := []*ast.Member{}
	for _, m := range iface.Members {
		m := m.(*ast.Member)
		if m.Attribute {
			out = append(out, m)
		}
	}
	return out
}

func constantsOf(iface *ast.Interface) []*ast.Member {
	out := []*ast.Member{}
	for _, m := range iface.Members {
		m := m.(*ast.Member)
		if m.Const {
			out = append(out, m)
		}
	}
	return out
}

// pascalCase returns the snake-case string s transformed into 'PascalCase',
// Rules:
// * The first letter of the string is capitalized
// * Characters following an underscore, hyphen or number are capitalized
// * Underscores are removed from the returned string
// See: https://en.wikipedia.org/wiki/Camel_case
func pascalCase(s string) string {
	b := strings.Builder{}
	upper := true
	for _, r := range s {
		if r == '_' || r == '-' {
			upper = true
			continue
		}
		if upper {
			b.WriteRune(unicode.ToUpper(r))
			upper = false
		} else {
			b.WriteRune(r)
		}
		if unicode.IsNumber(r) {
			upper = true
		}
	}
	return b.String()
}
