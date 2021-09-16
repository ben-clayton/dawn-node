#include "src/bindings/gpurenderpipeline.h"

#include <cassert>

#include "src/bindings/gpubindgrouplayout.h"
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
GPURenderPipeline::getBindGroupLayout(Napi::Env env, unsigned long index) {
  return interop::GPUBindGroupLayout::Create<GPUBindGroupLayout>(
      env, pipeline_.GetBindGroupLayout(index));
}

std::optional<std::string> GPURenderPipeline::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPURenderPipeline::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
