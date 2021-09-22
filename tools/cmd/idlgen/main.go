// Copyright 2021 The Dawn Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// idlgen is a tool used to generate code from WebIDL files and a golang
// template file
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

func showUsage() {
	fmt.Println(`
idlgen is a tool used to generate code from WebIDL files and a golang
template file

Usage:
  idlgen --template=<template-path> --output=<output-path> <idl-file> [<idl-file>...]`)
	os.Exit(1)
}

func run() error {
	var templatePath string
	var outputPath string
	flag.StringVar(&templatePath, "template", "", "the template file run with the parsed WebIDL files")
	flag.StringVar(&outputPath, "output", "", "the output file")
	flag.Parse()

	idlFiles := flag.Args()

	// Check all required arguments are provided
	if templatePath == "" || outputPath == "" || len(idlFiles) == 0 {
		showUsage()
	}

	// Open up the output file
	out := os.Stdout
	if outputPath != "" {
		file, err := os.Create(outputPath)
		if err != nil {
			return fmt.Errorf("failed to open output file '%v'", outputPath)
		}
		out = file
		defer file.Close()
	}

	// Read the template file
	tmpl, err := ioutil.ReadFile(templatePath)
	if err != nil {
		return fmt.Errorf("failed to open template file '%v'", templatePath)
	}

	// idl is the combination of the parsed idlFiles
	idl := &ast.File{}

	// Parse each of the WebIDL files and add the declarations to idl
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

	// Initialize the generator
	g := generator{t: template.New(templatePath)}
	g.workingDir = filepath.Dir(templatePath)
	g.funcs = map[string]interface{}{
		// Functions exposed to the template
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

	// simplify the definitions in the WebIDL before passing this to the template
	idl, declarations := simplify(idl)
	g.declarations = declarations

	// Write the file header
	fmt.Fprintf(out, header, strings.Join(os.Args[1:], "\n//   "))

	// Execute the template
	return t.Execute(out, idl)
}

// declarations is a map of WebIDL declaration name to its AST node.
type declarations map[string]ast.Decl

// nameOf returns the name of the AST node n.
// panics if n is not a named node type.
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

// simplify processes the AST 'in', returning a new AST that:
// * Has all partial interfaces merged into a single interface.
// * Has all mixins flattened into their place of use.
// * Has all the declarations ordered in dependency order (leaf first)
// simplify also returns the map of declarations in the AST.
func simplify(in *ast.File) (*ast.File, declarations) {
	s := simplifier{
		declarations: declarations{},
		registered:   map[string]bool{},
		out:          &ast.File{},
	}

	// Walk the IDL declarations to merge together partial interfaces and embed
	// mixins into their uses.
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

	// Now traverse the declarations in to produce the dependency-ordered
	// output `s.out`.
	for _, d := range in.Declarations {
		if name := nameOf(d); name != "" {
			s.visit(s.declarations[nameOf(d)])
		}
	}

	return s.out, s.declarations
}

// simplifier holds internal state for simplify()
type simplifier struct {
	// all AST declarations
	declarations declarations
	// set of visited declarations
	registered map[string]bool
	// the dependency-ordered output
	out *ast.File
}

// visit traverses the AST declaration 'd' adding all dependent declarations to
// s.out.
func (s *simplifier) visit(d ast.Decl) {
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
			s.visit(d)
		}
		for _, m := range d.Members {
			if m, ok := m.(*ast.Member); ok {
				s.visitType(m.Type)
				for _, p := range m.Parameters {
					s.visitType(p.Type)
				}
			}
		}
	case *ast.Dictionary:
		if register(d.Name) {
			return
		}
		if d, ok := s.declarations[d.Inherits]; ok {
			s.visit(d)
		}
		for _, m := range d.Members {
			s.visitType(m.Type)
			for _, p := range m.Parameters {
				s.visitType(p.Type)
			}
		}
	case *ast.Typedef:
		if register(d.Name) {
			return
		}
		s.visitType(d.Type)
	case *ast.Mixin:
		if register(d.Name) {
			return
		}
		for _, m := range d.Members {
			if m, ok := m.(*ast.Member); ok {
				s.visitType(m.Type)
				for _, p := range m.Parameters {
					s.visitType(p.Type)
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

// visitType traverses the AST type 't' adding all dependent declarations to
// s.out.
func (s *simplifier) visitType(t ast.Type) {
	switch t := t.(type) {
	case *ast.TypeName:
		if d, ok := s.declarations[t.Name]; ok {
			s.visit(d)
		}
	case *ast.UnionType:
		for _, t := range t.Types {
			s.visitType(t)
		}
	case *ast.ParametrizedType:
		for _, t := range t.Elems {
			s.visitType(t)
		}
	case *ast.NullableType:
		s.visitType(t.Type)
	case *ast.SequenceType:
		s.visitType(t.Elem)
	case *ast.RecordType:
		s.visitType(t.Elem)
	default:
		panic(fmt.Errorf("unhandled AST type %T", t))
	}
}

// generator holds the template generator state
type generator struct {
	// the root template
	t *template.Template
	// the working directory
	workingDir string
	// map of function name to function exposed to the template executor
	funcs map[string]interface{}
	// dependency-sorted declarations
	declarations declarations
}

// eval executes the sub-template with the given name and arguments, returning
// the generated output
// args can be a single argument:
//   arg[0]
// or a list of name-value pairs:
//   (args[0]: name, args[1]: value), (args[2]: name, args[3]: value)...
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

// lookup returns the declaration with the given name, or nil if not found.
func (g *generator) lookup(name string) ast.Decl {
	return g.declarations[name]
}

// include loads the template with the given path, importing the declarations
// into the scope of the current template.
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

// enumEntryName formats the enum entry name 's' for use in a C++ enum.
func enumEntryName(s string) string {
	return "k" + strings.ReplaceAll(pascalCase(strings.Trim(s, `"`)), "-", "")
}

// Method describes a WebIDL interface method
type Method struct {
	// Name of the method
	Name string
	// The list of overloads of the method
	Overloads []*ast.Member
}

// methodsOf returns all the methods of the given WebIDL interface.
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

// attributesOf returns all the attributes of the given WebIDL interface.
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

// constantsOf returns all the constant values of the given WebIDL interface.
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

const header = `// Copyright 2021 The Dawn Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

////////////////////////////////////////////////////////////////////////////////
// File generated by tools/cmd/idlgen.go, with the arguments:
//   %v
//
// Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

`
