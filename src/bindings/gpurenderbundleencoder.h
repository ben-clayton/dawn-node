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

#ifndef DAWN_NODE_SRC_BINDINGS_GPURENDERBUNDLEENCODER_H_
#define DAWN_NODE_SRC_BINDINGS_GPURENDERBUNDLEENCODER_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "interop/webgpu.h"
#include "napi.h"

namespace wgpu {
namespace bindings {

class GPURenderBundleEncoder : public interop::GPURenderBundleEncoder {
 public:
  GPURenderBundleEncoder(wgpu::RenderBundleEncoder enc);

  interop::Interface<interop::GPURenderBundle> finish(
      Napi::Env,
      std::optional<interop::GPURenderBundleDescriptor> descriptor) override;
  void setBindGroup(Napi::Env, interop::GPUIndex32 index,
                    interop::Interface<interop::GPUBindGroup> bindGroup,
                    std::optional<std::vector<interop::GPUBufferDynamicOffset>>
                        dynamicOffsets) override;
  void setBindGroup(Napi::Env, interop::GPUIndex32 index,
                    interop::Interface<interop::GPUBindGroup> bindGroup,
                    interop::Uint32Array dynamicOffsetsData,
                    interop::GPUSize64 dynamicOffsetsDataStart,
                    interop::GPUSize32 dynamicOffsetsDataLength) override;
  void pushDebugGroup(Napi::Env, std::string groupLabel) override;
  void popDebugGroup(Napi::Env) override;
  void insertDebugMarker(Napi::Env, std::string markerLabel) override;
  void setPipeline(
      Napi::Env,
      interop::Interface<interop::GPURenderPipeline> pipeline) override;
  void setIndexBuffer(Napi::Env, interop::Interface<interop::GPUBuffer> buffer,
                      interop::GPUIndexFormat indexFormat,
                      std::optional<interop::GPUSize64> offset,
                      std::optional<interop::GPUSize64> size) override;
  void setVertexBuffer(Napi::Env, interop::GPUIndex32 slot,
                       interop::Interface<interop::GPUBuffer> buffer,
                       std::optional<interop::GPUSize64> offset,
                       std::optional<interop::GPUSize64> size) override;
  void draw(Napi::Env, interop::GPUSize32 vertexCount,
            std::optional<interop::GPUSize32> instanceCount,
            std::optional<interop::GPUSize32> firstVertex,
            std::optional<interop::GPUSize32> firstInstance) override;
  void drawIndexed(Napi::Env, interop::GPUSize32 indexCount,
                   std::optional<interop::GPUSize32> instanceCount,
                   std::optional<interop::GPUSize32> firstIndex,
                   std::optional<interop::GPUSignedOffset32> baseVertex,
                   std::optional<interop::GPUSize32> firstInstance) override;
  void drawIndirect(Napi::Env,
                    interop::Interface<interop::GPUBuffer> indirectBuffer,
                    interop::GPUSize64 indirectOffset) override;
  void drawIndexedIndirect(
      Napi::Env, interop::Interface<interop::GPUBuffer> indirectBuffer,
      interop::GPUSize64 indirectOffset) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::RenderBundleEncoder enc_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPURENDERBUNDLEENCODER_H_
