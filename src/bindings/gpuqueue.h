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

#ifndef DAWN_NODE_SRC_BINDINGS_GPU_H_
#define DAWN_NODE_SRC_BINDINGS_GPU_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "src/bindings/async_runner.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUQueue : public interop::GPUQueue {
 public:
  GPUQueue(wgpu::Queue queue, AsyncRunner async);

  void submit(Napi::Env,
              std::vector<interop::Interface<interop::GPUCommandBuffer>>
                  commandBuffers) override;
  interop::Promise<void> onSubmittedWorkDone(Napi::Env) override;
  void writeBuffer(Napi::Env, interop::Interface<interop::GPUBuffer> buffer,
                   interop::GPUSize64 bufferOffset, interop::BufferSource data,
                   std::optional<interop::GPUSize64> dataOffset,
                   std::optional<interop::GPUSize64> size) override;
  void writeTexture(Napi::Env, interop::GPUImageCopyTexture destination,
                    interop::BufferSource data,
                    interop::GPUImageDataLayout dataLayout,
                    interop::GPUExtent3D size) override;
  void copyExternalImageToTexture(
      Napi::Env, interop::GPUImageCopyExternalImage source,
      interop::GPUImageCopyTextureTagged destination,
      interop::GPUExtent3D copySize) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::Queue queue_;
  AsyncRunner async_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPU_H_
