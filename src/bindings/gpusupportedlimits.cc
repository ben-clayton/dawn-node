#include "src/bindings/gpusupportedlimits.h"

#include <cassert>

#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUSupportedLimits
////////////////////////////////////////////////////////////////////////////////

// Values taken from.
// https://source.chromium.org/chromium/chromium/src/+/main:third_party/blink/renderer/modules/webgpu/gpu_supported_limits.h
// TODO: Actually use limits reported by the device / adapter.

uint32_t GPUSupportedLimits::getMaxTextureDimension1D(Napi::Env) {
  return 8192;
}

uint32_t GPUSupportedLimits::getMaxTextureDimension2D(Napi::Env) {
  return 8192;
}

uint32_t GPUSupportedLimits::getMaxTextureDimension3D(Napi::Env) {
  return 2048;
}

uint32_t GPUSupportedLimits::getMaxTextureArrayLayers(Napi::Env) {
  return 2048;
}

uint32_t GPUSupportedLimits::getMaxBindGroups(Napi::Env) { return 4; }

uint32_t GPUSupportedLimits::getMaxDynamicUniformBuffersPerPipelineLayout(
    Napi::Env) {
  return 8;
}

uint32_t GPUSupportedLimits::getMaxDynamicStorageBuffersPerPipelineLayout(
    Napi::Env) {
  return 4;
}

uint32_t GPUSupportedLimits::getMaxSampledTexturesPerShaderStage(
    Napi::Env) {
  return 16;
}

uint32_t GPUSupportedLimits::getMaxSamplersPerShaderStage(Napi::Env) {
  return 16;
}

uint32_t GPUSupportedLimits::getMaxStorageBuffersPerShaderStage(
    Napi::Env) {
  return 4;
}

uint32_t GPUSupportedLimits::getMaxStorageTexturesPerShaderStage(
    Napi::Env) {
  return 4;
}

uint32_t GPUSupportedLimits::getMaxUniformBuffersPerShaderStage(
    Napi::Env) {
  return 12;
}

uint64_t GPUSupportedLimits::getMaxUniformBufferBindingSize(
    Napi::Env) {
  return 16384;
}

uint64_t GPUSupportedLimits::getMaxStorageBufferBindingSize(
    Napi::Env) {
  return 134217728;
}

uint32_t GPUSupportedLimits::getMinUniformBufferOffsetAlignment(
    Napi::Env) {
  return 256;
}

uint32_t GPUSupportedLimits::getMinStorageBufferOffsetAlignment(
    Napi::Env) {
  return 256;
}

uint32_t GPUSupportedLimits::getMaxVertexBuffers(Napi::Env) { return 8; }

uint32_t GPUSupportedLimits::getMaxVertexAttributes(Napi::Env) {
  return 16;
}

uint32_t GPUSupportedLimits::getMaxVertexBufferArrayStride(Napi::Env) {
  return 2048;
}

uint32_t GPUSupportedLimits::getMaxInterStageShaderComponents(Napi::Env) {
  return 60;
}

uint32_t GPUSupportedLimits::getMaxComputeWorkgroupStorageSize(Napi::Env) {
  return 16352;
}

uint32_t GPUSupportedLimits::getMaxComputeInvocationsPerWorkgroup(
    Napi::Env) {
  return 256;
}

uint32_t GPUSupportedLimits::getMaxComputeWorkgroupSizeX(Napi::Env) {
  return 256;
}

uint32_t GPUSupportedLimits::getMaxComputeWorkgroupSizeY(Napi::Env) {
  return 256;
}

uint32_t GPUSupportedLimits::getMaxComputeWorkgroupSizeZ(Napi::Env) {
  return 64;
}

uint32_t GPUSupportedLimits::getMaxComputeWorkgroupsPerDimension(
    Napi::Env) {
  return 65535;
}

}  // namespace bindings
}  // namespace wgpu
