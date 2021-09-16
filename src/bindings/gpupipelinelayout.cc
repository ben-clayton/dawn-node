#include "src/bindings/gpupipelinelayout.h"

#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUPipelineLayout
////////////////////////////////////////////////////////////////////////////////
GPUPipelineLayout::GPUPipelineLayout(wgpu::PipelineLayout layout)
    : layout_(layout) {}

std::optional<std::string> GPUPipelineLayout::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPUPipelineLayout::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
