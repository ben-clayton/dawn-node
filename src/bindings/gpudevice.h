#ifndef DAWN_NODE_SRC_BINDINGS_GPUDEVICE_H_
#define DAWN_NODE_SRC_BINDINGS_GPUDEVICE_H_

#include "dawn/webgpu_cpp.h"
#include "napi.h"
#include "src/bindings/async_runner.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUDevice : public interop::GPUDevice {
 public:
  GPUDevice(Napi::Env env, wgpu::Device device);
  ~GPUDevice();

  interop::Interface<interop::GPUSupportedFeatures> getFeatures(
      Napi::Env) override;
  interop::Interface<interop::GPUSupportedLimits> getLimits(Napi::Env) override;
  interop::Interface<interop::GPUQueue> getQueue(Napi::Env env) override;
  void destroy(Napi::Env) override;
  interop::Interface<interop::GPUBuffer> createBuffer(
      Napi::Env env, interop::GPUBufferDescriptor descriptor) override;
  interop::Interface<interop::GPUTexture> createTexture(
      Napi::Env, interop::GPUTextureDescriptor descriptor) override;
  interop::Interface<interop::GPUSampler> createSampler(
      Napi::Env,
      std::optional<interop::GPUSamplerDescriptor> descriptor) override;
  interop::Interface<interop::GPUExternalTexture> importExternalTexture(
      Napi::Env, interop::GPUExternalTextureDescriptor descriptor) override;
  interop::Interface<interop::GPUBindGroupLayout> createBindGroupLayout(
      Napi::Env, interop::GPUBindGroupLayoutDescriptor descriptor) override;
  interop::Interface<interop::GPUPipelineLayout> createPipelineLayout(
      Napi::Env, interop::GPUPipelineLayoutDescriptor descriptor) override;
  interop::Interface<interop::GPUBindGroup> createBindGroup(
      Napi::Env, interop::GPUBindGroupDescriptor descriptor) override;
  interop::Interface<interop::GPUShaderModule> createShaderModule(
      Napi::Env, interop::GPUShaderModuleDescriptor descriptor) override;
  interop::Interface<interop::GPUComputePipeline> createComputePipeline(
      Napi::Env, interop::GPUComputePipelineDescriptor descriptor) override;
  interop::Interface<interop::GPURenderPipeline> createRenderPipeline(
      Napi::Env, interop::GPURenderPipelineDescriptor descriptor) override;
  interop::Promise<interop::Interface<interop::GPUComputePipeline>>
  createComputePipelineAsync(
      Napi::Env env, interop::GPUComputePipelineDescriptor descriptor) override;
  interop::Promise<interop::Interface<interop::GPURenderPipeline>>
  createRenderPipelineAsync(
      Napi::Env env, interop::GPURenderPipelineDescriptor descriptor) override;
  interop::Interface<interop::GPUCommandEncoder> createCommandEncoder(
      Napi::Env env,
      std::optional<interop::GPUCommandEncoderDescriptor> descriptor) override;
  interop::Interface<interop::GPURenderBundleEncoder> createRenderBundleEncoder(
      Napi::Env, interop::GPURenderBundleEncoderDescriptor descriptor) override;
  interop::Interface<interop::GPUQuerySet> createQuerySet(
      Napi::Env, interop::GPUQuerySetDescriptor descriptor) override;
  interop::Promise<interop::Interface<interop::GPUDeviceLostInfo>> getLost(
      Napi::Env env) override;
  void pushErrorScope(Napi::Env, interop::GPUErrorFilter filter) override;
  interop::Promise<std::optional<interop::GPUError>> popErrorScope(
      Napi::Env env) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;
  interop::Interface<interop::EventHandler> getOnuncapturederror(
      Napi::Env) override;
  void setOnuncapturederror(
      Napi::Env, interop::Interface<interop::EventHandler> value) override;
  void addEventListener(
      Napi::Env, std::string type,
      std::optional<interop::Interface<interop::EventListener>> callback,
      std::optional<std::variant<interop::AddEventListenerOptions, bool>>
          options) override;
  void removeEventListener(
      Napi::Env, std::string type,
      std::optional<interop::Interface<interop::EventListener>> callback,
      std::optional<std::variant<interop::EventListenerOptions, bool>> options)
      override;
  bool dispatchEvent(Napi::Env,
                     interop::Interface<interop::Event> event) override;

 private:
  void QueueTick();

  Napi::Env env_;
  wgpu::Device device_;
  AsyncRunner async_;
  std::vector<interop::Promise<interop::Interface<interop::GPUDeviceLostInfo>>>
      lost_promises_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUDEVICE_H_
