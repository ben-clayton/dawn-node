#include "src/bindings/gpurenderpassencoder.h"

#include <cassert>

#include "src/bindings/convert.h"
#include "src/bindings/gpubindgroup.h"
#include "src/bindings/gpubuffer.h"
#include "src/bindings/gpurenderpipeline.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPURenderPassEncoder
////////////////////////////////////////////////////////////////////////////////
GPURenderPassEncoder::GPURenderPassEncoder(wgpu::RenderPassEncoder enc)
    : enc_(enc) {}

void GPURenderPassEncoder::setViewport(Napi::Env, float x, float y, float width,
                                       float height, float minDepth,
                                       float maxDepth) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::setScissorRect(
    Napi::Env, interop::GPUIntegerCoordinate x, interop::GPUIntegerCoordinate y,
    interop::GPUIntegerCoordinate width, interop::GPUIntegerCoordinate height) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::setBlendConstant(Napi::Env,
                                            interop::GPUColor color) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::setStencilReference(
    Napi::Env, interop::GPUStencilValue reference) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::beginOcclusionQuery(Napi::Env,
                                               interop::GPUSize32 queryIndex) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::endOcclusionQuery(Napi::Env) { UNIMPLEMENTED(); }

void GPURenderPassEncoder::beginPipelineStatisticsQuery(
    Napi::Env, interop::Interface<interop::GPUQuerySet> querySet,
    interop::GPUSize32 queryIndex) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::endPipelineStatisticsQuery(Napi::Env) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::writeTimestamp(
    Napi::Env, interop::Interface<interop::GPUQuerySet> querySet,
    interop::GPUSize32 queryIndex) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::executeBundles(
    Napi::Env,
    std::vector<interop::Interface<interop::GPURenderBundle>> bundles) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::endPass(Napi::Env) { enc_.EndPass(); }

void GPURenderPassEncoder::setBindGroup(
    Napi::Env env, interop::GPUIndex32 index,
    interop::Interface<interop::GPUBindGroup> bindGroup,
    std::optional<std::vector<interop::GPUBufferDynamicOffset>>
        dynamicOffsets) {
  Converter conv(env);
  wgpu::BindGroup bg{};
  if (!conv(bg, bindGroup)) {
    return;
  }
  std::vector<uint32_t> offsets;
  if (dynamicOffsets.has_value() && dynamicOffsets->size() > 0) {
    offsets.resize(dynamicOffsets->size());
    for (size_t i = 0; i < offsets.size(); i++) {
      if (!conv(offsets[i], dynamicOffsets.value()[i])) {
        return;
      }
    }
    enc_.SetBindGroup(index, bg, offsets.size(), offsets.data());
  } else {
    enc_.SetBindGroup(index, bg);
  }
}

void GPURenderPassEncoder::setBindGroup(
    Napi::Env, interop::GPUIndex32 index,
    interop::Interface<interop::GPUBindGroup> bindGroup,
    interop::Uint32Array dynamicOffsetsData,
    interop::GPUSize64 dynamicOffsetsDataStart,
    interop::GPUSize32 dynamicOffsetsDataLength) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::pushDebugGroup(Napi::Env, std::string groupLabel) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::popDebugGroup(Napi::Env) { UNIMPLEMENTED(); }

void GPURenderPassEncoder::insertDebugMarker(Napi::Env,
                                             std::string markerLabel) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::setPipeline(
    Napi::Env env, interop::Interface<interop::GPURenderPipeline> pipeline) {
  Converter conv(env);
  wgpu::RenderPipeline rp{};
  if (!conv(rp, pipeline)) {
    return;
  }
  enc_.SetPipeline(rp);
}

void GPURenderPassEncoder::setIndexBuffer(
    Napi::Env env, interop::Interface<interop::GPUBuffer> buffer,
    interop::GPUIndexFormat indexFormat,
    std::optional<interop::GPUSize64> offset,
    std::optional<interop::GPUSize64> size) {
  Converter conv(env);

  wgpu::Buffer buf{};
  wgpu::IndexFormat fmt;
  uint64_t off = 0;
  uint64_t sz = 0;
  if (!conv(buf, buffer) ||       //
      !conv(fmt, indexFormat) ||  //
      !conv(off, offset) ||       //
      !conv(sz, size)) {
    return;
  }
  enc_.SetIndexBuffer(buf, fmt, off, sz);
}

void GPURenderPassEncoder::setVertexBuffer(
    Napi::Env env, interop::GPUIndex32 slot,
    interop::Interface<interop::GPUBuffer> buffer,
    std::optional<interop::GPUSize64> offset,
    std::optional<interop::GPUSize64> size) {
  Converter conv(env);

  wgpu::Buffer buf{};
  wgpu::IndexFormat fmt;
  uint64_t off = 0;
  uint64_t sz = 0;
  if (!conv(buf, buffer) ||  //
      !conv(off, offset) ||  //
      !conv(sz, size)) {
    return;
  }
  enc_.SetVertexBuffer(slot, buf, off, sz);
}

void GPURenderPassEncoder::draw(
    Napi::Env env, interop::GPUSize32 vertexCount,
    std::optional<interop::GPUSize32> instanceCount,
    std::optional<interop::GPUSize32> firstVertex,
    std::optional<interop::GPUSize32> firstInstance) {
  Converter conv(env);

  uint32_t vert_cnt = 0;
  uint32_t ins_cnt = 1;
  uint32_t first_vert = 0;
  uint32_t first_inst = 0;

  if (!conv(vert_cnt, vertexCount) ||    //
      !conv(ins_cnt, instanceCount) ||   //
      !conv(first_vert, firstVertex) ||  //
      !conv(first_inst, firstInstance)) {
    return;
  }
  enc_.Draw(vert_cnt, ins_cnt, first_vert, first_inst);
}

void GPURenderPassEncoder::drawIndexed(
    Napi::Env env, interop::GPUSize32 indexCount,
    std::optional<interop::GPUSize32> instanceCount,
    std::optional<interop::GPUSize32> firstIndex,
    std::optional<interop::GPUSignedOffset32> baseVertex,
    std::optional<interop::GPUSize32> firstInstance) {
  Converter conv(env);

  uint32_t idx_cnt = 0;
  uint32_t ins_cnt = 1;
  uint32_t first_idx = 0;
  int32_t base = 0;
  uint32_t first_inst = 0;

  if (!conv(idx_cnt, indexCount) ||     //
      !conv(ins_cnt, instanceCount) ||  //
      !conv(first_idx, firstIndex) ||   //
      !conv(base, baseVertex) ||        //
      !conv(first_inst, firstInstance)) {
    return;
  }
  enc_.DrawIndexed(idx_cnt, ins_cnt, first_idx, base, first_inst);
}

void GPURenderPassEncoder::drawIndirect(
    Napi::Env, interop::Interface<interop::GPUBuffer> indirectBuffer,
    interop::GPUSize64 indirectOffset) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::drawIndexedIndirect(
    Napi::Env, interop::Interface<interop::GPUBuffer> indirectBuffer,
    interop::GPUSize64 indirectOffset) {
  UNIMPLEMENTED();
}

std::optional<std::string> GPURenderPassEncoder::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPURenderPassEncoder::setLabel(Napi::Env,
                                    std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
