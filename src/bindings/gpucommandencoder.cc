#include "src/bindings/gpucommandencoder.h"

#include <cassert>

#include "src/bindings/convert.h"
#include "src/bindings/gpu.h"
#include "src/bindings/gpubuffer.h"
#include "src/bindings/gpucommandbuffer.h"
#include "src/bindings/gputexture.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUCommandEncoder
////////////////////////////////////////////////////////////////////////////////
GPUCommandEncoder::GPUCommandEncoder(wgpu::CommandEncoder cmd_enc)
    : cmd_enc_(cmd_enc) {}

interop::Interface<interop::GPURenderPassEncoder>
GPUCommandEncoder::beginRenderPass(
    Napi::Env, interop::GPURenderPassDescriptor descriptor) {
  UNIMPLEMENTED();
}

interop::Interface<interop::GPUComputePassEncoder>
GPUCommandEncoder::beginComputePass(
    Napi::Env, std::optional<interop::GPUComputePassDescriptor> descriptor) {
  UNIMPLEMENTED();
}

void GPUCommandEncoder::copyBufferToBuffer(
    Napi::Env, interop::Interface<interop::GPUBuffer> source,
    interop::GPUSize64 sourceOffset,
    interop::Interface<interop::GPUBuffer> destination,
    interop::GPUSize64 destinationOffset, interop::GPUSize64 size) {
  auto* src = source.As<GPUBuffer>();
  auto* dst = destination.As<GPUBuffer>();
  cmd_enc_.CopyBufferToBuffer(*src, sourceOffset, *dst, destinationOffset,
                              size);
}

void GPUCommandEncoder::copyBufferToTexture(
    Napi::Env env, interop::GPUImageCopyBuffer source,
    interop::GPUImageCopyTexture destination, interop::GPUExtent3D copySize) {
  wgpu::ImageCopyBuffer src{};
  src.buffer = *source.buffer.As<GPUBuffer>();
  src.layout.offset = source.offset;
  src.layout.bytesPerRow = source.bytesPerRow;
  src.layout.rowsPerImage = source.rowsPerImage;
  wgpu::ImageCopyTexture dst{};
  dst.texture = *destination.texture.As<GPUTexture>();
  dst.mipLevel = destination.mipLevel;
  wgpu::Extent3D size{};
  if (!Convert(dst.origin, destination.origin) ||  //
      !Convert(dst.aspect, destination.aspect) ||  //
      !Convert(size, copySize)) {
    Napi::Error::New(env, "invalid argument").ThrowAsJavaScriptException();
    return;
  }
  cmd_enc_.CopyBufferToTexture(&src, &dst, &size);
}

void GPUCommandEncoder::copyTextureToBuffer(
    Napi::Env, interop::GPUImageCopyTexture source,
    interop::GPUImageCopyBuffer destination, interop::GPUExtent3D copySize) {
  UNIMPLEMENTED();
}

void GPUCommandEncoder::copyTextureToTexture(
    Napi::Env, interop::GPUImageCopyTexture source,
    interop::GPUImageCopyTexture destination, interop::GPUExtent3D copySize) {
  UNIMPLEMENTED();
}

void GPUCommandEncoder::pushDebugGroup(Napi::Env, std::string groupLabel) {
  UNIMPLEMENTED();
}

void GPUCommandEncoder::popDebugGroup(Napi::Env) { UNIMPLEMENTED(); }

void GPUCommandEncoder::insertDebugMarker(Napi::Env, std::string markerLabel) {
  UNIMPLEMENTED();
}

void GPUCommandEncoder::writeTimestamp(
    Napi::Env, interop::Interface<interop::GPUQuerySet> querySet,
    interop::GPUSize32 queryIndex) {
  UNIMPLEMENTED();
}

void GPUCommandEncoder::resolveQuerySet(
    Napi::Env, interop::Interface<interop::GPUQuerySet> querySet,
    interop::GPUSize32 firstQuery, interop::GPUSize32 queryCount,
    interop::Interface<interop::GPUBuffer> destination,
    interop::GPUSize64 destinationOffset) {
  UNIMPLEMENTED();
}

interop::Interface<interop::GPUCommandBuffer> GPUCommandEncoder::finish(
    Napi::Env env,
    std::optional<interop::GPUCommandBufferDescriptor> descriptor) {
  wgpu::CommandBufferDescriptor desc{};
  return interop::GPUCommandBuffer::Create<GPUCommandBuffer>(
      env, cmd_enc_.Finish(&desc));
}

}  // namespace bindings
}  // namespace wgpu
