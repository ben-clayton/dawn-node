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

#ifndef DAWN_NODE_SRC_UTILS_DEBUG_H_
#define DAWN_NODE_SRC_UTILS_DEBUG_H_

#include <iostream>
#include <optional>

#include "dawn/webgpu_cpp_print.h"

namespace wgpu {
namespace utils {

////////////////////////////////////////////////////////////////////////////////
// Write() is a helper for printing container types to the std::ostream.
// Write() is used by the LOG() macro below.
////////////////////////////////////////////////////////////////////////////////

// Forward declarations
inline std::ostream& Write(std::ostream& out) { return out; }
template <typename T>
inline std::ostream& Write(std::ostream& out, const std::optional<T>& value);
template <typename T>
inline std::ostream& Write(std::ostream& out, const std::vector<T>& value);
template <typename K, typename V>
inline std::ostream& Write(std::ostream& out,
                           const std::unordered_map<K, V>& value);
template <typename... TYS>
inline std::ostream& Write(std::ostream& out,
                           const std::variant<TYS...>& value);
template <typename VALUE>
inline std::ostream& Write(std::ostream& out, const VALUE& value);

// Write() implementations
template <typename T>
std::ostream& Write(std::ostream& out, const std::optional<T>& value) {
  if (value.has_value()) {
    return Write(out, value.value());
  }
  return out << "<undefined>";
}

template <typename T>
std::ostream& Write(std::ostream& out, const std::vector<T>& value) {
  out << "[";
  bool first = true;
  for (const auto& el : value) {
    if (!first) {
      out << ", ";
    }
    first = false;
    Write(out, el);
  }
  return out << "]";
}

template <typename K, typename V>
std::ostream& Write(std::ostream& out, const std::unordered_map<K, V>& value) {
  out << "{";
  bool first = true;
  for (auto it : value) {
    if (!first) {
      out << ", ";
    }
    first = false;
    Write(out, it.first);
    out << ": ";
    Write(out, it.second);
  }
  return out << "}";
}

template <typename... TYS>
std::ostream& Write(std::ostream& out, const std::variant<TYS...>& value) {
  std::visit([&](auto&& v) { Write(out, v); }, value);
  return out;
}

template <typename VALUE>
std::ostream& Write(std::ostream& out, const VALUE& value) {
  return out << value;
}

template <typename FIRST, typename SECOND, typename... REST>
inline std::ostream& Write(std::ostream& out, FIRST&& first, SECOND&& second,
                           REST&&... rest) {
  Write(out, std::forward<FIRST>(first));
  Write(out, std::forward<SECOND>(second));
  Write(out, std::forward<REST>(rest)...);
  return out;
}

////////////////////////////////////////////////////////////////////////////////

// Unimplemented() prints an 'UNIMPLEMENTED' message to stdout with the given
// file, line and function, then calls abort().
// Unimplemented() is usually not called directly, but by the UNIMPLEMENTED()
// macro below.
[[noreturn]] inline void Unimplemented(const char* file, int line,
                                       const char* function) {
  std::cout << file << ":" << line << ": "
            << "UNIMPLEMENTED : " << function << std::endl;
  abort();
}

// LOG() prints the current file, line and function to stdout, followed by a
// string representation of all the variadic arguments.
#define LOG(...)                                                               \
  ::wgpu::utils::Write(                                                        \
      std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << ": ", \
      ##__VA_ARGS__)                                                           \
      << std::endl

// UNIMPLEMENTED() prints 'UNIMPLEMENTED' with the current file, line and
// function to stdout, then calls abort().
// The macro calls Unimplemented(), which is annotated with [[noreturn]].
// Used to stub code that has not yet been implemented.
#define UNIMPLEMENTED() \
  ::wgpu::utils::Unimplemented(__FILE__, __LINE__, __FUNCTION__)

}  // namespace utils
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_UTILS_DEBUG_H_
