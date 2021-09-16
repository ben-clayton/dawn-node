#include "src/bindings/gpubindgroup.h"

#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUBindGroup
////////////////////////////////////////////////////////////////////////////////
GPUBindGroup::GPUBindGroup(wgpu::BindGroup group) : group_(group) {}

std::optional<std::string> GPUBindGroup::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPUBindGroup::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
