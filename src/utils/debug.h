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

template <typename VALUE>
inline std::ostream& Write(std::ostream& out, VALUE&& value) {
  return out << value;
}

template <typename T>
inline std::ostream& Write(std::ostream& out, std::optional<T> value) {
  if (value.has_value()) {
    return Write(out, value.value());
  }
  return out << "<undefined>";
}

template <typename VALUE, typename... OTHERS>
inline std::ostream& Write(std::ostream& out, VALUE&& value,
                           OTHERS&&... others) {
  Write(out, value);
  Write(out, std::forward<OTHERS>(others)...);
  return out;
}

#define LOG(...)                                                               \
  Write(                                                                       \
      std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << ": ", \
      ##__VA_ARGS__)                                                           \
      << std::endl
#define UNIMPLEMENTED() Unimplemented(__FILE__, __LINE__, __FUNCTION__)

#endif  // DAWN_NODE_SRC_UTILS_LOG_H_
