#ifndef DAWN_NODE_SRC_BINDINGS_GPUSUPPORTEDLIMITS_H_
#define DAWN_NODE_SRC_BINDINGS_GPUSUPPORTEDLIMITS_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUSupportedLimits : public interop::GPUSupportedLimits {
 public:
  unsigned long getMaxTextureDimension1D(Napi::Env) override;
  unsigned long getMaxTextureDimension2D(Napi::Env) override;
  unsigned long getMaxTextureDimension3D(Napi::Env) override;
  unsigned long getMaxTextureArrayLayers(Napi::Env) override;
  unsigned long getMaxBindGroups(Napi::Env) override;
  unsigned long getMaxDynamicUniformBuffersPerPipelineLayout(
      Napi::Env) override;
  unsigned long getMaxDynamicStorageBuffersPerPipelineLayout(
      Napi::Env) override;
  unsigned long getMaxSampledTexturesPerShaderStage(Napi::Env) override;
  unsigned long getMaxSamplersPerShaderStage(Napi::Env) override;
  unsigned long getMaxStorageBuffersPerShaderStage(Napi::Env) override;
  unsigned long getMaxStorageTexturesPerShaderStage(Napi::Env) override;
  unsigned long getMaxUniformBuffersPerShaderStage(Napi::Env) override;
  unsigned long long getMaxUniformBufferBindingSize(Napi::Env) override;
  unsigned long long getMaxStorageBufferBindingSize(Napi::Env) override;
  unsigned long getMinUniformBufferOffsetAlignment(Napi::Env) override;
  unsigned long getMinStorageBufferOffsetAlignment(Napi::Env) override;
  unsigned long getMaxVertexBuffers(Napi::Env) override;
  unsigned long getMaxVertexAttributes(Napi::Env) override;
  unsigned long getMaxVertexBufferArrayStride(Napi::Env) override;
  unsigned long getMaxInterStageShaderComponents(Napi::Env) override;
  unsigned long getMaxComputeWorkgroupStorageSize(Napi::Env) override;
  unsigned long getMaxComputeInvocationsPerWorkgroup(Napi::Env) override;
  unsigned long getMaxComputeWorkgroupSizeX(Napi::Env) override;
  unsigned long getMaxComputeWorkgroupSizeY(Napi::Env) override;
  unsigned long getMaxComputeWorkgroupSizeZ(Napi::Env) override;
  unsigned long getMaxComputeWorkgroupsPerDimension(Napi::Env) override;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUSUPPORTEDLIMITS_H_
