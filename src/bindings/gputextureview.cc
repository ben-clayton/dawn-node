#include "src/bindings/gputextureview.h"

#include <cassert>

#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUTextureView
////////////////////////////////////////////////////////////////////////////////
GPUTextureView::GPUTextureView(wgpu::TextureView view) : view_(view) {}

std::optional<std::string> GPUTextureView::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPUTextureView::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
};

}  // namespace bindings
}  // namespace wgpu
