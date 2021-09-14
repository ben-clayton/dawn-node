#include "src/bindings/gpucommandencoder.h"

#include <cassert>

#include "src/bindings/convert.h"
#include "src/bindings/gpu.h"
#include "src/bindings/gpubuffer.h"
#include "src/bindings/gpucommandbuffer.h"
#include "src/bindings/gpucomputepassencoder.h"
#include "src/bindings/gpuqueryset.h"
#include "src/bindings/gpurenderpassencoder.h"
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
    Napi::Env env, interop::GPURenderPassDescriptor descriptor) {
  wgpu::RenderPassDescriptor desc{};
  Converter conv(env);

  std::vector<wgpu::RenderPassColorAttachment> colorAttachments(
      descriptor.colorAttachments.size());
  for (size_t i = 0; i < colorAttachments.size(); i++) {
    if (!conv(colorAttachments[i], descriptor.colorAttachments[i])) {
      return {};
    }
  }
  desc.colorAttachments = colorAttachments.data();
  desc.colorAttachmentCount = colorAttachments.size();

  wgpu::RenderPassDepthStencilAttachment depthStencilAttachment{};
  if (descriptor.depthStencilAttachment.has_value()) {
    if (!conv(depthStencilAttachment, descriptor.depthStencilAttachment)) {
      return {};
    }
    desc.depthStencilAttachment = &depthStencilAttachment;
  }

  if (!conv(desc.label, descriptor.label) ||
      !conv(desc.occlusionQuerySet, descriptor.occlusionQuerySet)) {
    return {};
  }
  return interop::GPURenderPassEncoder::Create<GPURenderPassEncoder>(
      env, cmd_enc_.BeginRenderPass(&desc));
}

interop::Interface<interop::GPUComputePassEncoder>
GPUCommandEncoder::beginComputePass(
    Napi::Env env,
    std::optional<interop::GPUComputePassDescriptor> descriptor) {
  wgpu::ComputePassDescriptor desc{};
  return interop::GPUComputePassEncoder::Create<GPUComputePassEncoder>(
      env, cmd_enc_.BeginComputePass(&desc));
}

void GPUCommandEncoder::copyBufferToBuffer(
    Napi::Env env, interop::Interface<interop::GPUBuffer> source,
    interop::GPUSize64 sourceOffset,
    interop::Interface<interop::GPUBuffer> destination,
    interop::GPUSize64 destinationOffset, interop::GPUSize64 size) {
  wgpu::Buffer src{};
  wgpu::Buffer dst{};
  Converter conv(env);
  if (!conv(src, source) ||  //
      !conv(dst, destination)) {
    return;
  }
  cmd_enc_.CopyBufferToBuffer(src, sourceOffset, dst, destinationOffset, size);
}

void GPUCommandEncoder::copyBufferToTexture(
    Napi::Env env, interop::GPUImageCopyBuffer source,
    interop::GPUImageCopyTexture destination, interop::GPUExtent3D copySize) {
  wgpu::ImageCopyBuffer src{};
  wgpu::ImageCopyTexture dst{};
  wgpu::Extent3D size{};
  Converter conv(env);
  if (!conv(src, source) ||       //
      !conv(dst, destination) ||  //
      !conv(size, copySize)) {
    return;
  }
  cmd_enc_.CopyBufferToTexture(&src, &dst, &size);
}

void GPUCommandEncoder::copyTextureToBuffer(
    Napi::Env env, interop::GPUImageCopyTexture source,
    interop::GPUImageCopyBuffer destination, interop::GPUExtent3D copySize) {
  wgpu::ImageCopyTexture src{};
  wgpu::ImageCopyBuffer dst{};
  wgpu::Extent3D size{};
  Converter conv(env);
  if (!conv(src, source) ||       //
      !conv(dst, destination) ||  //
      !conv(size, copySize)) {
    return;
  }
  cmd_enc_.CopyTextureToBuffer(&src, &dst, &size);
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

std::optional<std::string> GPUCommandEncoder::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPUCommandEncoder::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
