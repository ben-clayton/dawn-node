#include "src/bindings/gpupipelinelayout.h"

#include "src/bindings/gpudevice.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUPipelineLayout
////////////////////////////////////////////////////////////////////////////////
GPUPipelineLayout::GPUPipelineLayout(wgpu::PipelineLayout layout)
    : layout_(layout) {}

}  // namespace bindings
}  // namespace wgpu
