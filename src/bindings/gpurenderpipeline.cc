#include "src/bindings/gpurenderpipeline.h"

#include <cassert>

#include "src/bindings/gpubuffer.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPURenderPipeline
////////////////////////////////////////////////////////////////////////////////
GPURenderPipeline::GPURenderPipeline(wgpu::RenderPipeline pipeline)
    : pipeline_(pipeline) {}

interop::Interface<interop::GPUBindGroupLayout>
GPURenderPipeline::getBindGroupLayout(Napi::Env, unsigned long index) {
  UNIMPLEMENTED();
}

std::optional<std::string> GPURenderPipeline::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPURenderPipeline::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
