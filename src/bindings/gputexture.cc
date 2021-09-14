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
    desc.baseMipLevel = descriptor->baseMipLevel;
    desc.mipLevelCount = descriptor->mipLevelCount;
    desc.baseArrayLayer = descriptor->baseArrayLayer;
    desc.arrayLayerCount = descriptor->arrayLayerCount;
    if (!Convert(env, desc.format, descriptor->format) ||
        !Convert(env, desc.dimension, descriptor->dimension) ||
        !Convert(env, desc.aspect, descriptor->aspect)) {
      return {};
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
