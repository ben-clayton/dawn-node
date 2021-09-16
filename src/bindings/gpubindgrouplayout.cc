#include "src/bindings/gpubindgrouplayout.h"

#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUBindGroupLayout
////////////////////////////////////////////////////////////////////////////////
GPUBindGroupLayout::GPUBindGroupLayout(wgpu::BindGroupLayout layout)
    : layout_(layout) {}

std::optional<std::string> GPUBindGroupLayout::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPUBindGroupLayout::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
