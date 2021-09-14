#ifndef DAWN_NODE_SRC_BIND_CORE_H_
#define DAWN_NODE_SRC_BIND_CORE_H_

#include <stdint.h>

#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include "napi.h"

namespace wgpu {
namespace interop {

using ArrayBuffer = Napi::ArrayBuffer;

using Int8Array = Napi::TypedArrayOf<int8_t>;
using Int16Array = Napi::TypedArrayOf<int16_t>;
using Int32Array = Napi::TypedArrayOf<int32_t>;
using Uint8Array = Napi::TypedArrayOf<uint8_t>;
using Uint16Array = Napi::TypedArrayOf<uint16_t>;
using Uint32Array = Napi::TypedArrayOf<uint32_t>;
using Float32Array = Napi::TypedArrayOf<float>;
using Float64Array = Napi::TypedArrayOf<double>;
using DataView = Napi::TypedArray;

template <typename T>
class Interface {
 public:
  inline Interface() {}
  inline Interface(Napi::Object o) : object(o) {}
  inline operator napi_value() const { return object; }

  inline T* operator->() const { return T::Unwrap(object); }
  inline T* operator*() const { return T::Unwrap(object); }

  template <typename IMPL>
  inline IMPL* As() const {
    return static_cast<IMPL*>(T::Unwrap(object));
  }

 private:
  Napi::Object object;
};

template <typename T>
class Promise {
 public:
  Promise(Napi::Env env) : deferred(Napi::Promise::Deferred::New(env)) {}

  inline Napi::Promise Value() const { return deferred.Promise(); }

  void Resolve(T&& value) const {
    deferred.Resolve(Marshal(deferred.Env(), std::forward<T>(value)));
  }

  void Reject(std::string reason) const {
    deferred.Reject(Napi::String::New(deferred.Env(), std::move(reason)));
  }

 private:
  Napi::Promise::Deferred deferred;
};

template <>
class Promise<void> {
 public:
  Promise(Napi::Env env) : deferred(Napi::Promise::Deferred::New(env)) {}

  inline Napi::Value Value() const { return deferred.Promise(); }

  void Resolve() const { deferred.Resolve(deferred.Env().Undefined()); }

 private:
  Napi::Promise::Deferred deferred;
};

template <typename T>
class Serializer {};

template <>
class Serializer<Napi::Object> {
 public:
  static inline Napi::Object Unmarshal(Napi::Value value) {
    return value.ToObject();
  }
  static inline Napi::Value Marshal(Napi::Env, Napi::Object value) {
    return value;
  }
};

template <>
class Serializer<ArrayBuffer> {
 public:
  static inline ArrayBuffer Unmarshal(Napi::Value value) {
    return value.As<ArrayBuffer>();
  };
  static inline Napi::Value Marshal(Napi::Env, ArrayBuffer value) {
    return value;
  }
};

template <>
class Serializer<Napi::TypedArray> {
 public:
  static inline Napi::TypedArray Unmarshal(Napi::Value value) {
    return value.As<Napi::TypedArray>();
  };
  static inline Napi::Value Marshal(Napi::Env, ArrayBuffer value) {
    return value;
  }
};

template <typename T>
class Serializer<Napi::TypedArrayOf<T>> {
 public:
  static inline Napi::TypedArrayOf<T> Unmarshal(Napi::Value value) {
    return value.As<Napi::TypedArrayOf<T>>();
  };
  static inline Napi::Value Marshal(Napi::Env, ArrayBuffer value) {
    return value;
  }
};

template <>
class Serializer<bool> {
 public:
  static bool Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, bool);
};

template <>
class Serializer<std::string> {
 public:
  static std::string Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, std::string);
};

template <>
class Serializer<int8_t> {
 public:
  static int8_t Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, int8_t);
};

template <>
class Serializer<uint8_t> {
 public:
  static uint8_t Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, uint8_t);
};

template <>
class Serializer<int16_t> {
 public:
  static int16_t Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, int16_t);
};

template <>
class Serializer<uint16_t> {
 public:
  static uint16_t Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, uint16_t);
};

template <>
class Serializer<int32_t> {
 public:
  static int32_t Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, int32_t);
};

template <>
class Serializer<uint32_t> {
 public:
  static uint32_t Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, uint32_t);
};

template <>
class Serializer<int64_t> {
 public:
  static int64_t Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, int64_t);
};

template <>
class Serializer<uint64_t> {
 public:
  static uint64_t Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, uint64_t);
};

template <>
class Serializer<long long> {
 public:
  static long long Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, long long);
};

template <>
class Serializer<unsigned long long> {
 public:
  static unsigned long long Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, unsigned long long);
};

template <>
class Serializer<float> {
 public:
  static float Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, float);
};

template <>
class Serializer<double> {
 public:
  static double Unmarshal(Napi::Value);
  static Napi::Value Marshal(Napi::Env, double);
};

template <typename T>
class Serializer<Interface<T>> {
 public:
  static Interface<T> Unmarshal(Napi::Value value) {
    return {value.ToObject()};
  }
  static Napi::Value Marshal(Napi::Env env, const Interface<T>& value) {
    return {env, value};
  }
};

template <typename T>
class Serializer<std::optional<T>> {
 public:
  static std::optional<T> Unmarshal(Napi::Value value) {
    if (value.IsNull() || value.IsUndefined()) {
      return {};
    }
    return Serializer<T>::Unmarshal(value);
  }
  static Napi::Value Marshal(Napi::Env env, std::optional<T> value) {
    if (value.has_value()) {
      return Serializer<T>::Marshal(env, value.value());
    }
    return env.Null();
  }
};

template <typename T>
class Serializer<std::vector<T>> {
 public:
  static inline std::vector<T> Unmarshal(Napi::Value value) {
    auto arr = value.As<Napi::Array>();
    std::vector<T> vec(arr.Length());
    for (size_t i = 0; i < vec.size(); i++) {
      vec[i] = Serializer<T>::Unmarshal(arr[i]);
    }
    return vec;
  }
  static inline Napi::Value Marshal(Napi::Env env, const std::vector<T>& vec) {
    auto arr = Napi::Array::New(env, vec.size());
    for (size_t i = 0; i < vec.size(); i++) {
      arr.Set(i, Serializer<T>::Marshal(env, vec[i]));
    }
    return arr;
  }
};

template <typename K, typename V>
class Serializer<std::unordered_map<K, V>> {
 public:
  static inline std::unordered_map<K, V> Unmarshal(Napi::Value value) {
    auto obj = value.ToObject();
    auto keys = obj.GetPropertyNames();
    std::unordered_map<K, V> map(keys.Length());
    for (size_t i = 0; i < map.size(); i++) {
      auto key = Serializer<K>::Unmarshal(keys[i]);
      auto value = Serializer<V>::Unmarshal(obj.Get(keys[i]));
      map[key] = value;
    }
    return map;
  }
  static inline Napi::Value Marshal(Napi::Env env,
                                    std::unordered_map<K, V> value) {
    auto obj = Napi::Object::New(env);
    for (auto it : value) {
      obj.Set(Serializer<K>::Marshal(env, it.first),
              Serializer<V>::Marshal(env, it.second));
    }
    return obj;
  }
};

template <typename... TYPES>
class Serializer<std::variant<TYPES...>> {
  template <typename TY>
  static inline std::variant<TYPES...> TryUnmarshal(Napi::Value value) {
    return {Serializer<TY>::Unmarshal(value)};
  }

  template <typename T0, typename T1, typename... TN>
  static inline std::variant<TYPES...> TryUnmarshal(Napi::Value value) {
    try {
      return TryUnmarshal<T0>(value);
    } catch (Napi::Error) {
      return TryUnmarshal<T1, TN...>(value);
    }
  }

 public:
  static inline std::variant<TYPES...> Unmarshal(Napi::Value value) {
    return TryUnmarshal<TYPES...>(value);
  }
  static inline Napi::Value Marshal(Napi::Env env,
                                    std::variant<TYPES...> value) {
    return std::visit(
        [&](auto&& v) {
          using T = std::remove_cv_t<std::remove_reference_t<decltype(v)>>;
          return Serializer<T>::Marshal(env, v);
        },
        value);
  }
};

template <typename T>
class Serializer<Promise<T>> {
 public:
  static inline Promise<T> Unmarshal(Napi::Value) { return {}; }
  static inline Napi::Value Marshal(Napi::Env, Promise<T> promise) {
    return promise.Value();
  }
};

template <typename T>
inline T Unmarshal(Napi::Value value) {
  return Serializer<T>::Unmarshal(value);
}

template <typename T>
inline Napi::Value Marshal(Napi::Env env, T&& value) {
  return Serializer<std::remove_cv_t<std::remove_reference_t<T>>>::Marshal(
      env, std::forward<T>(value));
}

}  // namespace interop
}  // namespace wgpu

#endif  //  DAWN_NODE_SRC_BIND_CORE_H_
