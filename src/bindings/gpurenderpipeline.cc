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

#include "src/bindings/gpurenderpipeline.h"

#include <cassert>

#include "src/bindings/gpubindgrouplayout.h"
#include "src/bindings/gpubuffer.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPURenderPipeline
////////////////////////////////////////////////////////////////////////////////
GPURenderPipeline::GPURenderPipeline(wgpu::RenderPipeline pipeline)
    : pipeline_(pipeline) {}

interop::Interface<interop::GPUBindGroupLayout>
GPURenderPipeline::getBindGroupLayout(Napi::Env env, uint32_t index) {
  return interop::GPUBindGroupLayout::Create<GPUBindGroupLayout>(
      env, pipeline_.GetBindGroupLayout(index));
}

std::optional<std::string> GPURenderPipeline::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPURenderPipeline::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
