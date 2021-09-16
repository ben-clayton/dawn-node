#include "src/bindings/gpurenderbundleencoder.h"

#include <cassert>

#include "src/bindings/convert.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPURenderBundleEncoder
////////////////////////////////////////////////////////////////////////////////
GPURenderBundleEncoder::GPURenderBundleEncoder(wgpu::RenderBundleEncoder enc)
    : enc_(enc) {}

interop::Interface<interop::GPURenderBundle> GPURenderBundleEncoder::finish(
    Napi::Env, std::optional<interop::GPURenderBundleDescriptor> descriptor) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::setBindGroup(
    Napi::Env, interop::GPUIndex32 index,
    interop::Interface<interop::GPUBindGroup> bindGroup,
    std::optional<std::vector<interop::GPUBufferDynamicOffset>>
        dynamicOffsets) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::setBindGroup(
    Napi::Env, interop::GPUIndex32 index,
    interop::Interface<interop::GPUBindGroup> bindGroup,
    interop::Uint32Array dynamicOffsetsData,
    interop::GPUSize64 dynamicOffsetsDataStart,
    interop::GPUSize32 dynamicOffsetsDataLength) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::pushDebugGroup(Napi::Env, std::string groupLabel) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::popDebugGroup(Napi::Env) { UNIMPLEMENTED(); }

void GPURenderBundleEncoder::insertDebugMarker(Napi::Env,
                                               std::string markerLabel) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::setPipeline(
    Napi::Env, interop::Interface<interop::GPURenderPipeline> pipeline) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::setIndexBuffer(
    Napi::Env, interop::Interface<interop::GPUBuffer> buffer,
    interop::GPUIndexFormat indexFormat,
    std::optional<interop::GPUSize64> offset,
    std::optional<interop::GPUSize64> size) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::setVertexBuffer(
    Napi::Env, interop::GPUIndex32 slot,
    interop::Interface<interop::GPUBuffer> buffer,
    std::optional<interop::GPUSize64> offset,
    std::optional<interop::GPUSize64> size) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::draw(
    Napi::Env, interop::GPUSize32 vertexCount,
    std::optional<interop::GPUSize32> instanceCount,
    std::optional<interop::GPUSize32> firstVertex,
    std::optional<interop::GPUSize32> firstInstance) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::drawIndexed(
    Napi::Env, interop::GPUSize32 indexCount,
    std::optional<interop::GPUSize32> instanceCount,
    std::optional<interop::GPUSize32> firstIndex,
    std::optional<interop::GPUSignedOffset32> baseVertex,
    std::optional<interop::GPUSize32> firstInstance) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::drawIndirect(
    Napi::Env, interop::Interface<interop::GPUBuffer> indirectBuffer,
    interop::GPUSize64 indirectOffset) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::drawIndexedIndirect(
    Napi::Env, interop::Interface<interop::GPUBuffer> indirectBuffer,
    interop::GPUSize64 indirectOffset) {
  UNIMPLEMENTED();
}

std::optional<std::string> GPURenderBundleEncoder::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPURenderBundleEncoder::setLabel(Napi::Env,
                                      std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
