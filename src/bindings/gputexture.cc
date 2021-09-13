#include "src/bindings/gputexture.h"

#include <cassert>

#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUTexture
////////////////////////////////////////////////////////////////////////////////
GPUTexture::GPUTexture(wgpu::Texture texture) : texture_(texture) {}

interop::Interface<interop::GPUTextureView> GPUTexture::createView(
    Napi::Env, std::optional<interop::GPUTextureViewDescriptor> descriptor) {
  UNIMPLEMENTED();
}

void GPUTexture::destroy(Napi::Env) { texture_.Release(); }

}  // namespace bindings
}  // namespace wgpu
