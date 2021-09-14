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

}  // namespace bindings
}  // namespace wgpu
