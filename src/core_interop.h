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
#include "src/utils/debug.h"

#if 0
#define INTEROP_LOG(...) LOG(__VA_ARGS__)
#else
#define INTEROP_LOG(...)
#endif

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
  static inline bool Unmarshal(Napi::Env, Napi::Value value,
                               Napi::Object& out) {
    if (out.IsObject()) {
      out = value.ToObject();
      return true;
    }
    return false;
  }
  static inline Napi::Value Marshal(Napi::Env, Napi::Object value) {
    return value;
  }
};

template <>
class Serializer<ArrayBuffer> {
 public:
  static inline bool Unmarshal(Napi::Env, Napi::Value value, ArrayBuffer& out) {
    if (value.IsArrayBuffer()) {
      out = value.As<ArrayBuffer>();
      return true;
    }
    return false;
  };
  static inline Napi::Value Marshal(Napi::Env, ArrayBuffer value) {
    return value;
  }
};

template <>
class Serializer<Napi::TypedArray> {
 public:
  static inline bool Unmarshal(Napi::Env, Napi::Value value,
                               Napi::TypedArray& out) {
    if (value.IsTypedArray()) {
      out = value.As<Napi::TypedArray>();
      return true;
    }
    return false;
  };
  static inline Napi::Value Marshal(Napi::Env, ArrayBuffer value) {
    return value;
  }
};

template <typename T>
class Serializer<Napi::TypedArrayOf<T>> {
 public:
  static inline bool Unmarshal(Napi::Env, Napi::Value value,
                               Napi::TypedArrayOf<T>& out) {
    if (value.IsTypedArray()) {
      out = value.As<Napi::TypedArrayOf<T>>();
      return true;
    }
    return false;
  };
  static inline Napi::Value Marshal(Napi::Env, ArrayBuffer value) {
    return value;
  }
};

template <>
class Serializer<bool> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, bool&);
  static Napi::Value Marshal(Napi::Env, bool);
};

template <>
class Serializer<std::string> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, std::string&);
  static Napi::Value Marshal(Napi::Env, std::string);
};

template <>
class Serializer<int8_t> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, int8_t&);
  static Napi::Value Marshal(Napi::Env, int8_t);
};

template <>
class Serializer<uint8_t> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, uint8_t&);
  static Napi::Value Marshal(Napi::Env, uint8_t);
};

template <>
class Serializer<int16_t> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, int16_t&);
  static Napi::Value Marshal(Napi::Env, int16_t);
};

template <>
class Serializer<uint16_t> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, uint16_t&);
  static Napi::Value Marshal(Napi::Env, uint16_t);
};

template <>
class Serializer<int32_t> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, int32_t&);
  static Napi::Value Marshal(Napi::Env, int32_t);
};

template <>
class Serializer<uint32_t> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, uint32_t&);
  static Napi::Value Marshal(Napi::Env, uint32_t);
};

template <>
class Serializer<int64_t> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, int64_t&);
  static Napi::Value Marshal(Napi::Env, int64_t);
};

template <>
class Serializer<uint64_t> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, uint64_t&);
  static Napi::Value Marshal(Napi::Env, uint64_t);
};

template <>
class Serializer<long long> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, long long&);
  static Napi::Value Marshal(Napi::Env, long long);
};

template <>
class Serializer<unsigned long long> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, unsigned long long&);
  static Napi::Value Marshal(Napi::Env, unsigned long long);
};

template <>
class Serializer<float> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, float&);
  static Napi::Value Marshal(Napi::Env, float);
};

template <>
class Serializer<double> {
 public:
  static bool Unmarshal(Napi::Env, Napi::Value, double&);
  static Napi::Value Marshal(Napi::Env, double);
};

template <typename T>
class Serializer<Interface<T>> {
 public:
  static bool Unmarshal(Napi::Env env, Napi::Value value, Interface<T>& out) {
    if (value.IsObject()) {
      auto obj = value.As<Napi::Object>();
      if (T::Unwrap(obj)) {
        out = Interface<T>(obj);
        return true;
      }
    }
    return false;
  }
  static Napi::Value Marshal(Napi::Env env, const Interface<T>& value) {
    return {env, value};
  }
};

template <typename T>
class Serializer<std::optional<T>> {
 public:
  static bool Unmarshal(Napi::Env env, Napi::Value value,
                        std::optional<T>& out) {
    if (value.IsNull() || value.IsUndefined()) {
      out.reset();
      return true;
    }
    T v{};
    if (!Serializer<T>::Unmarshal(env, value, v)) {
      return false;
    }
    out = std::move(v);
    return true;
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
  static inline bool Unmarshal(Napi::Env env, Napi::Value value,
                               std::vector<T>& out) {
    if (!value.IsArray()) {
      return false;
    }
    auto arr = value.As<Napi::Array>();
    std::vector<T> vec(arr.Length());
    for (size_t i = 0; i < vec.size(); i++) {
      if (!Serializer<T>::Unmarshal(env, arr[i], vec[i])) {
        return false;
      }
    }
    out = std::move(vec);
    return true;
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
  static inline bool Unmarshal(Napi::Env env, Napi::Value value,
                               std::unordered_map<K, V>& out) {
    if (!value.IsObject()) {
      return false;
    }
    auto obj = value.ToObject();
    auto keys = obj.GetPropertyNames();
    std::unordered_map<K, V> map(keys.Length());
    for (size_t i = 0; i < map.size(); i++) {
      K key{};
      V value{};
      if (!Serializer<K>::Unmarshal(env, keys[i], key) ||
          !Serializer<V>::Unmarshal(env, obj.Get(keys[i]), value)) {
        return false;
      }
      map[key] = value;
    }
    out = std::move(map);
    return true;
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
  static inline bool TryUnmarshal(Napi::Env env, Napi::Value value,
                                  std::variant<TYPES...>& out) {
    TY v{};
    if (Serializer<TY>::Unmarshal(env, value, v)) {
      out = std::move(v);
      return true;
    }
    return false;
  }

  template <typename T0, typename T1, typename... TN>
  static inline bool TryUnmarshal(Napi::Env env, Napi::Value value,
                                  std::variant<TYPES...>& out) {
    if (TryUnmarshal<T0>(env, value, out)) {
      return true;
    }
    return TryUnmarshal<T1, TN...>(env, value, out);
  }

 public:
  static inline bool Unmarshal(Napi::Env env, Napi::Value value,
                               std::variant<TYPES...>& out) {
    return TryUnmarshal<TYPES...>(env, value, out);
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
  static inline bool Unmarshal(Napi::Env, Napi::Value, Promise<T>&) {
    UNIMPLEMENTED();
  }
  static inline Napi::Value Marshal(Napi::Env, Promise<T> promise) {
    return promise.Value();
  }
};

template <typename T>
inline bool Unmarshal(Napi::Env env, Napi::Value value, T& out) {
  return Serializer<T>::Unmarshal(env, value, out);
}

template <typename T>
inline bool UnmarshalOptional(Napi::Env env, Napi::Value value, T& out) {
  if (value.IsNull() || value.IsUndefined()) {
    return true;
  }
  return Serializer<T>::Unmarshal(env, value, out);
}

template <typename T>
inline Napi::Value Marshal(Napi::Env env, T&& value) {
  return Serializer<std::remove_cv_t<std::remove_reference_t<T>>>::Marshal(
      env, std::forward<T>(value));
}

template <typename TUPLE_ARGS, int BASE_INDEX = 0>
inline bool UnmarshalArgs(const Napi::CallbackInfo& info, TUPLE_ARGS& args) {
  if constexpr (BASE_INDEX < std::tuple_size_v<TUPLE_ARGS>) {
    using T = std::tuple_element_t<BASE_INDEX, TUPLE_ARGS>;
    if (!Unmarshal<T>(info.Env(), info[BASE_INDEX],
                      std::get<BASE_INDEX>(args))) {
      return false;
    }
    return UnmarshalArgs<TUPLE_ARGS, BASE_INDEX + 1>(info, args);
  } else {
    return true;
  }
}

}  // namespace interop
}  // namespace wgpu

#endif  //  DAWN_NODE_SRC_BIND_CORE_H_
