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

unsigned long GPUSupportedLimits::getMaxTextureDimension1D(Napi::Env) {
  return 8192;
}

unsigned long GPUSupportedLimits::getMaxTextureDimension2D(Napi::Env) {
  return 8192;
}

unsigned long GPUSupportedLimits::getMaxTextureDimension3D(Napi::Env) {
  return 2048;
}

unsigned long GPUSupportedLimits::getMaxTextureArrayLayers(Napi::Env) {
  return 2048;
}

unsigned long GPUSupportedLimits::getMaxBindGroups(Napi::Env) { return 4; }

unsigned long GPUSupportedLimits::getMaxDynamicUniformBuffersPerPipelineLayout(
    Napi::Env) {
  return 8;
}

unsigned long GPUSupportedLimits::getMaxDynamicStorageBuffersPerPipelineLayout(
    Napi::Env) {
  return 4;
}

unsigned long GPUSupportedLimits::getMaxSampledTexturesPerShaderStage(
    Napi::Env) {
  return 16;
}

unsigned long GPUSupportedLimits::getMaxSamplersPerShaderStage(Napi::Env) {
  return 16;
}

unsigned long GPUSupportedLimits::getMaxStorageBuffersPerShaderStage(
    Napi::Env) {
  return 4;
}

unsigned long GPUSupportedLimits::getMaxStorageTexturesPerShaderStage(
    Napi::Env) {
  return 4;
}

unsigned long GPUSupportedLimits::getMaxUniformBuffersPerShaderStage(
    Napi::Env) {
  return 12;
}

unsigned long long GPUSupportedLimits::getMaxUniformBufferBindingSize(
    Napi::Env) {
  return 16384;
}

unsigned long long GPUSupportedLimits::getMaxStorageBufferBindingSize(
    Napi::Env) {
  return 134217728;
}

unsigned long GPUSupportedLimits::getMinUniformBufferOffsetAlignment(
    Napi::Env) {
  return 256;
}

unsigned long GPUSupportedLimits::getMinStorageBufferOffsetAlignment(
    Napi::Env) {
  return 256;
}

unsigned long GPUSupportedLimits::getMaxVertexBuffers(Napi::Env) { return 8; }

unsigned long GPUSupportedLimits::getMaxVertexAttributes(Napi::Env) {
  return 16;
}

unsigned long GPUSupportedLimits::getMaxVertexBufferArrayStride(Napi::Env) {
  return 2048;
}

unsigned long GPUSupportedLimits::getMaxInterStageShaderComponents(Napi::Env) {
  return 60;
}

unsigned long GPUSupportedLimits::getMaxComputeWorkgroupStorageSize(Napi::Env) {
  return 16352;
}

unsigned long GPUSupportedLimits::getMaxComputeInvocationsPerWorkgroup(
    Napi::Env) {
  return 256;
}

unsigned long GPUSupportedLimits::getMaxComputeWorkgroupSizeX(Napi::Env) {
  return 256;
}

unsigned long GPUSupportedLimits::getMaxComputeWorkgroupSizeY(Napi::Env) {
  return 256;
}

unsigned long GPUSupportedLimits::getMaxComputeWorkgroupSizeZ(Napi::Env) {
  return 64;
}

unsigned long GPUSupportedLimits::getMaxComputeWorkgroupsPerDimension(
    Napi::Env) {
  return 65535;
}

}  // namespace bindings
}  // namespace wgpu
