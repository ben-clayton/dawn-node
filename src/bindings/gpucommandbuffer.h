#ifndef DAWN_NODE_SRC_BINDINGS_GPUCOMMANDBUFFER_H_
#define DAWN_NODE_SRC_BINDINGS_GPUCOMMANDBUFFER_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUCommandBuffer : public interop::GPUCommandBuffer {
 public:
  GPUCommandBuffer(wgpu::CommandBuffer cmd_buf);

  operator wgpu::CommandBuffer &() { return cmd_buf_; }

  interop::Promise<double> getExecutionTime(Napi::Env) override;

 private:
  wgpu::CommandBuffer cmd_buf_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUCOMMANDBUFFER_H_
