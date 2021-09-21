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

#ifndef DAWN_NODE_SRC_BINDINGS_GPUBINDGROUPLAYOUT_H_
#define DAWN_NODE_SRC_BINDINGS_GPUBINDGROUPLAYOUT_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUBindGroupLayout : public interop::GPUBindGroupLayout {
 public:
  GPUBindGroupLayout(wgpu::BindGroupLayout layout);

  operator wgpu::BindGroupLayout() const { return layout_; }

  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::BindGroupLayout layout_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUBINDGROUPLAYOUT_H_
