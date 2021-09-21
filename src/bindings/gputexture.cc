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

#include "src/bindings/gputexture.h"

#include <cassert>

#include "src/bindings/convert.h"
#include "src/bindings/errors.h"
#include "src/bindings/gputextureview.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUTexture
////////////////////////////////////////////////////////////////////////////////
GPUTexture::GPUTexture(wgpu::Texture texture) : texture_(texture) {}

interop::Interface<interop::GPUTextureView> GPUTexture::createView(
    Napi::Env env,
    std::optional<interop::GPUTextureViewDescriptor> descriptor) {
  if (!texture_) {
    Errors::OperationError(env).ThrowAsJavaScriptException();
    return {};
  }

  if (descriptor.has_value()) {
    wgpu::TextureViewDescriptor desc{};
    Converter conv(env);
    if (!conv(desc.baseMipLevel, descriptor->baseMipLevel) ||
        !conv(desc.mipLevelCount, descriptor->mipLevelCount) ||
        !conv(desc.baseArrayLayer, descriptor->baseArrayLayer) ||
        !conv(desc.arrayLayerCount, descriptor->arrayLayerCount) ||
        !conv(desc.format, descriptor->format) ||
        !conv(desc.dimension, descriptor->dimension) ||
        !conv(desc.aspect, descriptor->aspect)) {
      return {};
    }
    return interop::GPUTextureView::Create<GPUTextureView>(
        env, texture_.CreateView(&desc));
  }

  return interop::GPUTextureView::Create<GPUTextureView>(env,
                                                         texture_.CreateView());
}

void GPUTexture::destroy(Napi::Env) { texture_.Release(); }

std::optional<std::string> GPUTexture::getLabel(Napi::Env) { UNIMPLEMENTED(); }

void GPUTexture::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
