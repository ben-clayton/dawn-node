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

#ifndef DAWN_NODE_SRC_BINDINGS_GPUCOMPUTEPIPELINE_H_
#define DAWN_NODE_SRC_BINDINGS_GPUCOMPUTEPIPELINE_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUComputePipeline : public interop::GPUComputePipeline {
 public:
  GPUComputePipeline(wgpu::ComputePipeline pipeline);

  operator wgpu::ComputePipeline() const { return pipeline_; }

  interop::Interface<interop::GPUBindGroupLayout> getBindGroupLayout(
      Napi::Env, uint32_t index) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::ComputePipeline pipeline_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUCOMPUTEPIPELINE_H_
