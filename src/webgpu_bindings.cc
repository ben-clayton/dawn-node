#include <cassert>
#include <iostream>

#include "dawn/dawn_proc.h"
#include "dawn/webgpu_cpp.h"
#include "dawn/webgpu_cpp_print.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "src/bindings/gpu.h"
#include "webgpu_interop.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  dawnProcSetProcs(&dawn_native::GetProcs());

  wgpu::interop::InitWebGPUInterfaces(env, exports);

  auto navigator = Napi::Object::New(env);

  auto gpu = wgpu::interop::GPU::Create<wgpu::bindings::GPU>(env);

  navigator.Set(Napi::String::New(env, "gpu"), gpu);
  env.Global().Set(Napi::String::New(env, "navigator"), navigator);

  return exports;
}

NODE_API_MODULE(addon, Init)
