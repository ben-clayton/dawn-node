#include "src/bindings/gpucomputepipeline.h"

#include <cassert>

#include "src/bindings/gpubindgrouplayout.h"
#include "src/bindings/gpubuffer.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUComputePipeline
////////////////////////////////////////////////////////////////////////////////
GPUComputePipeline::GPUComputePipeline(wgpu::ComputePipeline pipeline)
    : pipeline_(pipeline) {}

interop::Interface<interop::GPUBindGroupLayout>
GPUComputePipeline::getBindGroupLayout(Napi::Env env, unsigned long index) {
  return interop::GPUBindGroupLayout::Create<GPUBindGroupLayout>(
      env, pipeline_.GetBindGroupLayout(index));
}

std::optional<std::string> GPUComputePipeline::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPUComputePipeline::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
