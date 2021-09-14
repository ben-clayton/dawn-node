#ifndef DAWN_NODE_SRC_BINDINGS_GPURENDERPASSENCODER_H_
#define DAWN_NODE_SRC_BINDINGS_GPURENDERPASSENCODER_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPURenderPassEncoder : public interop::GPURenderPassEncoder {
 public:
  GPURenderPassEncoder(wgpu::RenderPassEncoder enc);

  operator wgpu::RenderPassEncoder &() { return enc_; }

  void setViewport(Napi::Env, float x, float y, float width, float height,
                   float minDepth, float maxDepth) override;
  void setScissorRect(Napi::Env, interop::GPUIntegerCoordinate x,
                      interop::GPUIntegerCoordinate y,
                      interop::GPUIntegerCoordinate width,
                      interop::GPUIntegerCoordinate height) override;
  void setBlendConstant(Napi::Env, interop::GPUColor color) override;
  void setStencilReference(Napi::Env,
                           interop::GPUStencilValue reference) override;
  void beginOcclusionQuery(Napi::Env, interop::GPUSize32 queryIndex) override;
  void endOcclusionQuery(Napi::Env) override;
  void beginPipelineStatisticsQuery(
      Napi::Env, interop::Interface<interop::GPUQuerySet> querySet,
      interop::GPUSize32 queryIndex) override;
  void endPipelineStatisticsQuery(Napi::Env) override;
  void writeTimestamp(Napi::Env,
                      interop::Interface<interop::GPUQuerySet> querySet,
                      interop::GPUSize32 queryIndex) override;
  void executeBundles(Napi::Env,
                      std::vector<interop::Interface<interop::GPURenderBundle>>
                          bundles) override;
  void endPass(Napi::Env) override;

 private:
  wgpu::RenderPassEncoder enc_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPURENDERPASSENCODER_H_
