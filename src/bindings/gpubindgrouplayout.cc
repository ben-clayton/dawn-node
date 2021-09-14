#include "src/bindings/gpubindgrouplayout.h"

#include "src/bindings/gpudevice.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUBindGroupLayout
////////////////////////////////////////////////////////////////////////////////
GPUBindGroupLayout::GPUBindGroupLayout(wgpu::BindGroupLayout layout)
    : layout_(layout) {}

}  // namespace bindings
}  // namespace wgpu
