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

#ifndef DAWN_NODE_SRC_BINDINGS_GPUSUPPORTEDLIMITS_H_
#define DAWN_NODE_SRC_BINDINGS_GPUSUPPORTEDLIMITS_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "interop/webgpu.h"
#include "napi.h"

namespace wgpu {
namespace bindings {

class GPUSupportedLimits : public interop::GPUSupportedLimits {
 public:
  uint32_t getMaxTextureDimension1D(Napi::Env) override;
  uint32_t getMaxTextureDimension2D(Napi::Env) override;
  uint32_t getMaxTextureDimension3D(Napi::Env) override;
  uint32_t getMaxTextureArrayLayers(Napi::Env) override;
  uint32_t getMaxBindGroups(Napi::Env) override;
  uint32_t getMaxDynamicUniformBuffersPerPipelineLayout(Napi::Env) override;
  uint32_t getMaxDynamicStorageBuffersPerPipelineLayout(Napi::Env) override;
  uint32_t getMaxSampledTexturesPerShaderStage(Napi::Env) override;
  uint32_t getMaxSamplersPerShaderStage(Napi::Env) override;
  uint32_t getMaxStorageBuffersPerShaderStage(Napi::Env) override;
  uint32_t getMaxStorageTexturesPerShaderStage(Napi::Env) override;
  uint32_t getMaxUniformBuffersPerShaderStage(Napi::Env) override;
  uint64_t getMaxUniformBufferBindingSize(Napi::Env) override;
  uint64_t getMaxStorageBufferBindingSize(Napi::Env) override;
  uint32_t getMinUniformBufferOffsetAlignment(Napi::Env) override;
  uint32_t getMinStorageBufferOffsetAlignment(Napi::Env) override;
  uint32_t getMaxVertexBuffers(Napi::Env) override;
  uint32_t getMaxVertexAttributes(Napi::Env) override;
  uint32_t getMaxVertexBufferArrayStride(Napi::Env) override;
  uint32_t getMaxInterStageShaderComponents(Napi::Env) override;
  uint32_t getMaxComputeWorkgroupStorageSize(Napi::Env) override;
  uint32_t getMaxComputeInvocationsPerWorkgroup(Napi::Env) override;
  uint32_t getMaxComputeWorkgroupSizeX(Napi::Env) override;
  uint32_t getMaxComputeWorkgroupSizeY(Napi::Env) override;
  uint32_t getMaxComputeWorkgroupSizeZ(Napi::Env) override;
  uint32_t getMaxComputeWorkgroupsPerDimension(Napi::Env) override;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUSUPPORTEDLIMITS_H_
