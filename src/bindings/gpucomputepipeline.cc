#include "src/bindings/gpucomputepipeline.h"

#include <cassert>

#include "src/bindings/gpubuffer.h"
#include "src/bindings/gpucomputepipeline.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUComputePipeline
////////////////////////////////////////////////////////////////////////////////
GPUComputePipeline::GPUComputePipeline(wgpu::ComputePipeline pipeline)
    : pipeline_(pipeline) {}

}  // namespace bindings
}  // namespace wgpu
