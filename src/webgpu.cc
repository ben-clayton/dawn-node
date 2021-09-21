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

#include <cassert>
#include <iostream>

#include "dawn/dawn_proc.h"
#include "dawn/webgpu_cpp.h"
#include "dawn/webgpu_cpp_print.h"
#include "dawn_native/DawnNative.h"
#include "interop/webgpu.h"
#include "napi.h"
#include "src/bindings/gpu.h"

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