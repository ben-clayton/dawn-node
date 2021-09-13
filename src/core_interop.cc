#include "src/core_interop.h"

namespace wgpu {
namespace interop {

bool Serializer<bool>::Unmarshal(Napi::Value value) {
  return value.ToBoolean();
}
Napi::Value Serializer<bool>::Marshal(Napi::Env env, bool value) {
  return Napi::Value::From(env, value);
}

std::string Serializer<std::string>::Unmarshal(Napi::Value value) {
  return value.ToString();
}
Napi::Value Serializer<std::string>::Marshal(Napi::Env env, std::string value) {
  return Napi::Value::From(env, value);
}

int8_t Serializer<int8_t>::Unmarshal(Napi::Value value) {
  return value.ToNumber().Int32Value();
}
Napi::Value Serializer<int8_t>::Marshal(Napi::Env env, int8_t value) {
  return Napi::Value::From(env, value);
}

uint8_t Serializer<uint8_t>::Unmarshal(Napi::Value value) {
  return value.ToNumber().Uint32Value();
}
Napi::Value Serializer<uint8_t>::Marshal(Napi::Env env, uint8_t value) {
  return Napi::Value::From(env, value);
}

int16_t Serializer<int16_t>::Unmarshal(Napi::Value value) {
  return value.ToNumber().Int32Value();
}
Napi::Value Serializer<int16_t>::Marshal(Napi::Env env, int16_t value) {
  return Napi::Value::From(env, value);
}

uint16_t Serializer<uint16_t>::Unmarshal(Napi::Value value) {
  return value.ToNumber().Uint32Value();
}
Napi::Value Serializer<uint16_t>::Marshal(Napi::Env env, uint16_t value) {
  return Napi::Value::From(env, value);
}

int32_t Serializer<int32_t>::Unmarshal(Napi::Value value) {
  return value.ToNumber().Int32Value();
}
Napi::Value Serializer<int32_t>::Marshal(Napi::Env env, int32_t value) {
  return Napi::Value::From(env, value);
}

uint32_t Serializer<uint32_t>::Unmarshal(Napi::Value value) {
  return value.ToNumber().Uint32Value();
}
Napi::Value Serializer<uint32_t>::Marshal(Napi::Env env, uint32_t value) {
  return Napi::Value::From(env, value);
}

int64_t Serializer<int64_t>::Unmarshal(Napi::Value value) {
  return value.ToNumber().Int64Value();
}
Napi::Value Serializer<int64_t>::Marshal(Napi::Env env, int64_t value) {
  return Napi::Value::From(env, value);
}

uint64_t Serializer<uint64_t>::Unmarshal(Napi::Value value) {
  return value.ToNumber().Uint32Value();  // No Uint64Value?
}
Napi::Value Serializer<uint64_t>::Marshal(Napi::Env env, uint64_t value) {
  return Napi::Value::From(env, value);
}
long long Serializer<long long>::Unmarshal(Napi::Value value) {
  return value.ToNumber().Int64Value();
}
Napi::Value Serializer<long long>::Marshal(Napi::Env env, long long value) {
  return Napi::Value::From(env, value);
}

unsigned long long Serializer<unsigned long long>::Unmarshal(
    Napi::Value value) {
  return value.ToNumber().Uint32Value();  // No Uint64Value?
}
Napi::Value Serializer<unsigned long long>::Marshal(Napi::Env env,
                                                    unsigned long long value) {
  return Napi::Value::From(env, value);
}

float Serializer<float>::Unmarshal(Napi::Value value) {
  return value.ToNumber().FloatValue();
}
Napi::Value Serializer<float>::Marshal(Napi::Env env, float value) {
  return Napi::Value::From(env, value);
}

double Serializer<double>::Unmarshal(Napi::Value value) {
  return value.ToNumber().DoubleValue();
}
Napi::Value Serializer<double>::Marshal(Napi::Env env, double value) {
  return Napi::Value::From(env, value);
}

}  // namespace interop
}  // namespace wgpu
