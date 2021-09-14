#ifndef DAWN_NODE_SRC_BINDINGS_GPUCOMPUTEPIPELINE_H_
#define DAWN_NODE_SRC_BINDINGS_GPUCOMPUTEPIPELINE_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUComputePipeline : public interop::GPUComputePipeline {
 public:
  GPUComputePipeline(wgpu::ComputePipeline pipeline);

  operator wgpu::ComputePipeline &() { return pipeline_; }

 private:
  wgpu::ComputePipeline pipeline_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUCOMPUTEPIPELINE_H_
