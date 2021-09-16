#include "src/core_interop.h"

namespace wgpu {
namespace interop {

bool Serializer<bool>::Unmarshal(Napi::Env env, Napi::Value value, bool& out) {
  if (value.IsBoolean()) {
    out = value.ToBoolean();
    return true;
  }
  return false;
}
Napi::Value Serializer<bool>::Marshal(Napi::Env env, bool value) {
  return Napi::Value::From(env, value);
}

bool Serializer<std::string>::Unmarshal(Napi::Env env, Napi::Value value,
                                        std::string& out) {
  if (value.IsString()) {
    out = value.ToString();
    return true;
  }
  return false;
}
Napi::Value Serializer<std::string>::Marshal(Napi::Env env, std::string value) {
  return Napi::Value::From(env, value);
}

bool Serializer<int8_t>::Unmarshal(Napi::Env env, Napi::Value value,
                                   int8_t& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().Int32Value();
    return true;
  }
  return false;
}
Napi::Value Serializer<int8_t>::Marshal(Napi::Env env, int8_t value) {
  return Napi::Value::From(env, value);
}

bool Serializer<uint8_t>::Unmarshal(Napi::Env env, Napi::Value value,
                                    uint8_t& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().Uint32Value();
    return true;
  }
  return false;
}
Napi::Value Serializer<uint8_t>::Marshal(Napi::Env env, uint8_t value) {
  return Napi::Value::From(env, value);
}

bool Serializer<int16_t>::Unmarshal(Napi::Env env, Napi::Value value,
                                    int16_t& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().Int32Value();
    return true;
  }
  return false;
}
Napi::Value Serializer<int16_t>::Marshal(Napi::Env env, int16_t value) {
  return Napi::Value::From(env, value);
}

bool Serializer<uint16_t>::Unmarshal(Napi::Env env, Napi::Value value,
                                     uint16_t& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().Uint32Value();
    return true;
  }
  return false;
}
Napi::Value Serializer<uint16_t>::Marshal(Napi::Env env, uint16_t value) {
  return Napi::Value::From(env, value);
}

bool Serializer<int32_t>::Unmarshal(Napi::Env env, Napi::Value value,
                                    int32_t& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().Int32Value();
    return true;
  }
  return false;
}
Napi::Value Serializer<int32_t>::Marshal(Napi::Env env, int32_t value) {
  return Napi::Value::From(env, value);
}

bool Serializer<uint32_t>::Unmarshal(Napi::Env env, Napi::Value value,
                                     uint32_t& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().Uint32Value();
    return true;
  }
  return false;
}
Napi::Value Serializer<uint32_t>::Marshal(Napi::Env env, uint32_t value) {
  return Napi::Value::From(env, value);
}

bool Serializer<int64_t>::Unmarshal(Napi::Env env, Napi::Value value,
                                    int64_t& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().Int64Value();
    return true;
  }
  return false;
}
Napi::Value Serializer<int64_t>::Marshal(Napi::Env env, int64_t value) {
  return Napi::Value::From(env, value);
}

bool Serializer<uint64_t>::Unmarshal(Napi::Env env, Napi::Value value,
                                     uint64_t& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().Uint32Value();  // No Uint64Value?
    return true;
  }
  return false;
}
Napi::Value Serializer<uint64_t>::Marshal(Napi::Env env, uint64_t value) {
  return Napi::Value::From(env, value);
}
bool Serializer<long long>::Unmarshal(Napi::Env env, Napi::Value value,
                                      long long& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().Int64Value();
    return true;
  }
  return false;
}
Napi::Value Serializer<long long>::Marshal(Napi::Env env, long long value) {
  return Napi::Value::From(env, value);
}

bool Serializer<unsigned long long>::Unmarshal(Napi::Env, Napi::Value value,
                                               unsigned long long& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().Uint32Value();  // No Uint64Value?
    return true;
  }
  return false;
}
Napi::Value Serializer<unsigned long long>::Marshal(Napi::Env env,
                                                    unsigned long long value) {
  return Napi::Value::From(env, value);
}

bool Serializer<float>::Unmarshal(Napi::Env env, Napi::Value value,
                                  float& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().FloatValue();
    return true;
  }
  return false;
}
Napi::Value Serializer<float>::Marshal(Napi::Env env, float value) {
  return Napi::Value::From(env, value);
}

bool Serializer<double>::Unmarshal(Napi::Env env, Napi::Value value,
                                   double& out) {
  if (value.IsNumber()) {
    out = value.ToNumber().DoubleValue();
    return true;
  }
  return false;
}
Napi::Value Serializer<double>::Marshal(Napi::Env env, double value) {
  return Napi::Value::From(env, value);
}

}  // namespace interop
}  // namespace wgpu
