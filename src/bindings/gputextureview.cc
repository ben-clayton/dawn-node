#include "src/bindings/gputextureview.h"

#include <cassert>

#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUTextureView
////////////////////////////////////////////////////////////////////////////////
GPUTextureView::GPUTextureView(wgpu::TextureView view) : view_(view) {}

}  // namespace bindings
}  // namespace wgpu
