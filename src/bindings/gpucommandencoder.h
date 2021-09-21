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

#ifndef DAWN_NODE_SRC_BINDINGS_GPUCOMMANDENCODER_H_
#define DAWN_NODE_SRC_BINDINGS_GPUCOMMANDENCODER_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "interop/webgpu.h"
#include "napi.h"

namespace wgpu {
namespace bindings {

class GPUCommandEncoder : public interop::GPUCommandEncoder {
 public:
  GPUCommandEncoder(wgpu::CommandEncoder enc);

  interop::Interface<interop::GPURenderPassEncoder> beginRenderPass(
      Napi::Env, interop::GPURenderPassDescriptor descriptor) override;
  interop::Interface<interop::GPUComputePassEncoder> beginComputePass(
      Napi::Env,
      std::optional<interop::GPUComputePassDescriptor> descriptor) override;
  void copyBufferToBuffer(Napi::Env,
                          interop::Interface<interop::GPUBuffer> source,
                          interop::GPUSize64 sourceOffset,
                          interop::Interface<interop::GPUBuffer> destination,
                          interop::GPUSize64 destinationOffset,
                          interop::GPUSize64 size) override;
  void copyBufferToTexture(Napi::Env, interop::GPUImageCopyBuffer source,
                           interop::GPUImageCopyTexture destination,
                           interop::GPUExtent3D copySize) override;
  void copyTextureToBuffer(Napi::Env, interop::GPUImageCopyTexture source,
                           interop::GPUImageCopyBuffer destination,
                           interop::GPUExtent3D copySize) override;
  void copyTextureToTexture(Napi::Env, interop::GPUImageCopyTexture source,
                            interop::GPUImageCopyTexture destination,
                            interop::GPUExtent3D copySize) override;
  void pushDebugGroup(Napi::Env, std::string groupLabel) override;
  void popDebugGroup(Napi::Env) override;
  void insertDebugMarker(Napi::Env, std::string markerLabel) override;
  void writeTimestamp(Napi::Env,
                      interop::Interface<interop::GPUQuerySet> querySet,
                      interop::GPUSize32 queryIndex) override;
  void resolveQuerySet(Napi::Env,
                       interop::Interface<interop::GPUQuerySet> querySet,
                       interop::GPUSize32 firstQuery,
                       interop::GPUSize32 queryCount,
                       interop::Interface<interop::GPUBuffer> destination,
                       interop::GPUSize64 destinationOffset) override;
  interop::Interface<interop::GPUCommandBuffer> finish(
      Napi::Env env,
      std::optional<interop::GPUCommandBufferDescriptor> descriptor) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::CommandEncoder enc_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUCOMMANDENCODER_H_
