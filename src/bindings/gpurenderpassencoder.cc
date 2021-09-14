#include "src/bindings/gpurenderpassencoder.h"

#include <cassert>

#include "src/bindings/gpubuffer.h"
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

void GPURenderPassEncoder::endPass(Napi::Env) { UNIMPLEMENTED(); }

}  // namespace bindings
}  // namespace wgpu
