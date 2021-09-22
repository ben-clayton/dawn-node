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

#include "interop/webgpu.h"

#include "dawn/dawn_proc.h"
#include "src/bindings/gpu.h"

// Initialize() initializes the Dawn node module, registering all the WebGPU
// types and the 'navigator' global.
Napi::Object Initialize(Napi::Env env, Napi::Object exports) {
  // Begin by setting the Dawn procedure function pointers.
  dawnProcSetProcs(&dawn_native::GetProcs());

  // Register all the interop types
  wgpu::interop::Initialize(env);

  // Construct a wgpu::interop::GPU interface, implemented by
  // wgpu::bindings::GPU. This will be the 'gpu' field of the global 'navigator'
  // object.
  auto gpu = wgpu::interop::GPU::Create<wgpu::bindings::GPU>(env);

  // Construct the 'navigator' object, assign the 'gpu' field, and expose as a
  // global.
  auto navigator = Napi::Object::New(env);
  navigator.Set(Napi::String::New(env, "gpu"), gpu);
  env.Global().Set(Napi::String::New(env, "navigator"), navigator);

  return exports;  // Note: The module does not actually export anything.
}

NODE_API_MODULE(addon, Initialize)
