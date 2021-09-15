#ifndef DAWN_NODE_SRC_UTILS_LOG_H_
#define DAWN_NODE_SRC_UTILS_LOG_H_

#include <iostream>
#include <optional>

#include "dawn/webgpu_cpp_print.h"

[[noreturn]] inline void Unimplemented(const char* file, int line,
                                       const char* feature) {
  std::cout << file << ":" << line << ": "
            << "UNIMPLEMENTED : " << feature << std::endl;
  abort();
}

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

#define LOG(...)                                                               \
  Write(                                                                       \
      std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << ": ", \
      ##__VA_ARGS__)                                                           \
      << std::endl
#define UNIMPLEMENTED() Unimplemented(__FILE__, __LINE__, __FUNCTION__)

#endif  // DAWN_NODE_SRC_UTILS_LOG_H_
