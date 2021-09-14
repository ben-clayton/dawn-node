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

  interop::Interface<interop::GPUBindGroupLayout> getBindGroupLayout(
      Napi::Env, unsigned long index) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::ComputePipeline pipeline_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUCOMPUTEPIPELINE_H_
