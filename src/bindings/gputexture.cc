#include "src/bindings/gputexture.h"

#include <cassert>

#include "src/bindings/convert.h"
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
  if (descriptor.has_value()) {
    wgpu::TextureViewDescriptor desc{};
    desc.baseMipLevel = descriptor->baseMipLevel.value_or(0);
    desc.mipLevelCount = descriptor->mipLevelCount.value_or(0);
    desc.baseArrayLayer = descriptor->baseArrayLayer.value_or(0);
    desc.arrayLayerCount = descriptor->arrayLayerCount.value_or(0);
    if (descriptor->format.has_value()) {
      if (!Convert(env, desc.format, descriptor->format.value())) {
        return {};
      }
    }
    if (descriptor->dimension.has_value()) {
      if (!Convert(env, desc.dimension, descriptor->dimension.value())) {
        return {};
      }
    }
    if (descriptor->aspect.has_value()) {
      if (!Convert(env, desc.aspect, descriptor->aspect.value())) {
        return {};
      }
    }
    return interop::GPUTextureView::Create<GPUTextureView>(
        env, texture_.CreateView(&desc));
  }

  return interop::GPUTextureView::Create<GPUTextureView>(env,
                                                         texture_.CreateView());
}

void GPUTexture::destroy(Napi::Env) { texture_.Release(); }

}  // namespace bindings
}  // namespace wgpu
