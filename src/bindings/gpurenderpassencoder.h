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

  operator wgpu::RenderPassEncoder() const { return enc_; }

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
  void setBindGroup(Napi::Env, interop::GPUIndex32 index,
                    interop::Interface<interop::GPUBindGroup> bindGroup,
                    std::optional<std::vector<interop::GPUBufferDynamicOffset>>
                        dynamicOffsets) override;
  void setBindGroup(Napi::Env, interop::GPUIndex32 index,
                    interop::Interface<interop::GPUBindGroup> bindGroup,
                    interop::Uint32Array dynamicOffsetsData,
                    interop::GPUSize64 dynamicOffsetsDataStart,
                    interop::GPUSize32 dynamicOffsetsDataLength) override;
  void pushDebugGroup(Napi::Env, std::string groupLabel) override;
  void popDebugGroup(Napi::Env) override;
  void insertDebugMarker(Napi::Env, std::string markerLabel) override;
  void setPipeline(
      Napi::Env,
      interop::Interface<interop::GPURenderPipeline> pipeline) override;
  void setIndexBuffer(Napi::Env, interop::Interface<interop::GPUBuffer> buffer,
                      interop::GPUIndexFormat indexFormat,
                      std::optional<interop::GPUSize64> offset,
                      std::optional<interop::GPUSize64> size) override;
  void setVertexBuffer(Napi::Env, interop::GPUIndex32 slot,
                       interop::Interface<interop::GPUBuffer> buffer,
                       std::optional<interop::GPUSize64> offset,
                       std::optional<interop::GPUSize64> size) override;
  void draw(Napi::Env, interop::GPUSize32 vertexCount,
            std::optional<interop::GPUSize32> instanceCount,
            std::optional<interop::GPUSize32> firstVertex,
            std::optional<interop::GPUSize32> firstInstance) override;
  void drawIndexed(Napi::Env, interop::GPUSize32 indexCount,
                   std::optional<interop::GPUSize32> instanceCount,
                   std::optional<interop::GPUSize32> firstIndex,
                   std::optional<interop::GPUSignedOffset32> baseVertex,
                   std::optional<interop::GPUSize32> firstInstance) override;
  void drawIndirect(Napi::Env,
                    interop::Interface<interop::GPUBuffer> indirectBuffer,
                    interop::GPUSize64 indirectOffset) override;
  void drawIndexedIndirect(
      Napi::Env, interop::Interface<interop::GPUBuffer> indirectBuffer,
      interop::GPUSize64 indirectOffset) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::RenderPassEncoder enc_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPURENDERPASSENCODER_H_
