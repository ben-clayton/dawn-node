#include "src/bindings/gpucommandbuffer.h"

#include <cassert>

#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUCommandBuffer
////////////////////////////////////////////////////////////////////////////////

GPUCommandBuffer::GPUCommandBuffer(wgpu::CommandBuffer cmd_buf)
    : cmd_buf_(cmd_buf) {}

interop::Promise<double> GPUCommandBuffer::getExecutionTime(Napi::Env) {
  UNIMPLEMENTED();
};

std::optional<std::string> GPUCommandBuffer::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPUCommandBuffer::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
