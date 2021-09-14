#ifndef DAWN_NODE_SRC_BINDINGS_GPUCOMPUTEPASSENCODER_H_
#define DAWN_NODE_SRC_BINDINGS_GPUCOMPUTEPASSENCODER_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUComputePassEncoder : public interop::GPUComputePassEncoder {
 public:
  GPUComputePassEncoder(wgpu::ComputePassEncoder enc);

  operator wgpu::ComputePassEncoder &() { return enc_; }

  void setPipeline(
      Napi::Env,
      interop::Interface<interop::GPUComputePipeline> pipeline) override;
  void dispatch(Napi::Env, interop::GPUSize32 x,
                std::optional<interop::GPUSize32> y,
                std::optional<interop::GPUSize32> z) override;
  void dispatchIndirect(Napi::Env,
                        interop::Interface<interop::GPUBuffer> indirectBuffer,
                        interop::GPUSize64 indirectOffset) override;
  void beginPipelineStatisticsQuery(
      Napi::Env, interop::Interface<interop::GPUQuerySet> querySet,
      interop::GPUSize32 queryIndex) override;
  void endPipelineStatisticsQuery(Napi::Env) override;
  void writeTimestamp(Napi::Env,
                      interop::Interface<interop::GPUQuerySet> querySet,
                      interop::GPUSize32 queryIndex) override;
  void endPass(Napi::Env) override;

 private:
  wgpu::ComputePassEncoder enc_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUCOMPUTEPASSENCODER_H_
