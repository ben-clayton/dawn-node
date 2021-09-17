#include "src/bindings/gpudevice.h"

#include <cassert>

#include "src/bindings/convert.h"
#include "src/bindings/errors.h"
#include "src/bindings/gpubindgroup.h"
#include "src/bindings/gpubindgrouplayout.h"
#include "src/bindings/gpubuffer.h"
#include "src/bindings/gpucommandbuffer.h"
#include "src/bindings/gpucommandencoder.h"
#include "src/bindings/gpucomputepipeline.h"
#include "src/bindings/gpupipelinelayout.h"
#include "src/bindings/gpuqueryset.h"
#include "src/bindings/gpuqueue.h"
#include "src/bindings/gpurenderbundleencoder.h"
#include "src/bindings/gpurenderpipeline.h"
#include "src/bindings/gpusampler.h"
#include "src/bindings/gpushadermodule.h"
#include "src/bindings/gpusupportedlimits.h"
#include "src/bindings/gputexture.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

namespace {

class DeviceLostInfo : public interop::GPUDeviceLostInfo {
 public:
  DeviceLostInfo(interop::GPUDeviceLostReason reason, std::string message)
      : reason_(reason), message_(message) {}
  std::variant<interop::GPUDeviceLostReason> getReason(Napi::Env env) override {
    return reason_;
  }
  std::string getMessage(Napi::Env) override { return message_; }

 private:
  interop::GPUDeviceLostReason reason_;
  std::string message_;
};

class OOMError : public interop::GPUOutOfMemoryError {};
class ValidationError : public interop::GPUValidationError {
 public:
  ValidationError(std::string message) : message_(std::move(message)) {}

  std::string getMessage(Napi::Env) override { return message_; };

 private:
  std::string message_;
};

}  // namespace

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUDevice
////////////////////////////////////////////////////////////////////////////////
GPUDevice::GPUDevice(Napi::Env env, wgpu::Device device)
    : env_(env),
      device_(device),
      async_(std::make_shared<AsyncRunnerState>(env, device)) {
  device.SetLoggingCallback(
      [](WGPULoggingType type, char const* message, void* userdata) {
        std::cout << type << ": " << message << std::endl;
      },
      nullptr);
  device.SetUncapturedErrorCallback(
      [](WGPUErrorType type, char const* message, void* userdata) {
        std::cout << type << ": " << message << std::endl;
      },
      nullptr);

  device.SetDeviceLostCallback(
      [](char const* message, void* userdata) {
        auto* self = static_cast<GPUDevice*>(userdata);
        auto reason = interop::GPUDeviceLostReason::kDestroyed;
        for (auto promise : self->lost_promises_) {
          promise.Resolve(interop::GPUDeviceLostInfo::Create<DeviceLostInfo>(
              self->env_, reason, message));
        }
      },
      this);
}

GPUDevice::~GPUDevice() { LOG("~GPUDevice()"); }

interop::Interface<interop::GPUSupportedFeatures> GPUDevice::getFeatures(
    Napi::Env env) {
  class Features : public interop::GPUSupportedFeatures {};
  return interop::GPUSupportedFeatures::Create<Features>(env);
}

interop::Interface<interop::GPUSupportedLimits> GPUDevice::getLimits(
    Napi::Env env) {
  return interop::GPUSupportedLimits::Create<GPUSupportedLimits>(env);
}

interop::Interface<interop::GPUQueue> GPUDevice::getQueue(Napi::Env env) {
  // TODO: Should probably return the same Queue JS object.
  return interop::GPUQueue::Create<GPUQueue>(env, device_.GetQueue(), async_);
}

void GPUDevice::destroy(Napi::Env) { device_.Release(); }

interop::Interface<interop::GPUBuffer> GPUDevice::createBuffer(
    Napi::Env env, interop::GPUBufferDescriptor descriptor) {
  Converter conv(env);

  wgpu::BufferDescriptor desc{};
  if (!conv(desc.label, descriptor.label) ||
      !conv(desc.mappedAtCreation, descriptor.mappedAtCreation) ||
      !conv(desc.size, descriptor.size) ||
      !conv(desc.usage, descriptor.usage)) {
    return {};
  }
  //  LOG("label: ", desc.label, ", mappedAtCreation: ", desc.mappedAtCreation,
  //      ", size: ", desc.size, ", usage: ", desc.usage);
  return interop::GPUBuffer::Create<GPUBuffer>(env, device_.CreateBuffer(&desc),
                                               desc, device_, async_);
}

interop::Interface<interop::GPUTexture> GPUDevice::createTexture(
    Napi::Env env, interop::GPUTextureDescriptor descriptor) {
  Converter conv(env);

  wgpu::TextureDescriptor desc{};
  if (!conv(desc.label, descriptor.label) ||
      !conv(desc.usage, descriptor.usage) ||                  //
      !conv(desc.size, descriptor.size) ||                    //
      !conv(desc.dimension, descriptor.dimension) ||          //
      !conv(desc.mipLevelCount, descriptor.mipLevelCount) ||  //
      !conv(desc.sampleCount, descriptor.sampleCount) ||      //
      !conv(desc.format, descriptor.format)) {
    return {};
  }
  return interop::GPUTexture::Create<GPUTexture>(env,
                                                 device_.CreateTexture(&desc));
}

interop::Interface<interop::GPUSampler> GPUDevice::createSampler(
    Napi::Env env, std::optional<interop::GPUSamplerDescriptor> descriptor) {
  Converter conv(env);

  if (descriptor.has_value()) {
    wgpu::SamplerDescriptor desc{};
    if (!conv(desc.label, descriptor->label) ||                //
        !conv(desc.addressModeU, descriptor->addressModeU) ||  //
        !conv(desc.addressModeV, descriptor->addressModeV) ||  //
        !conv(desc.addressModeW, descriptor->addressModeW) ||  //
        !conv(desc.magFilter, descriptor->magFilter) ||        //
        !conv(desc.minFilter, descriptor->minFilter) ||        //
        !conv(desc.mipmapFilter, descriptor->mipmapFilter) ||  //
        !conv(desc.lodMinClamp, descriptor->lodMinClamp) ||    //
        !conv(desc.lodMaxClamp, descriptor->lodMaxClamp) ||    //
        !conv(desc.compare, descriptor->compare) ||            //
        !conv(desc.maxAnisotropy, descriptor->maxAnisotropy)) {
      return {};
    }
    return interop::GPUSampler::Create<GPUSampler>(
        env, device_.CreateSampler(&desc));
  }

  return interop::GPUSampler::Create<GPUSampler>(env, device_.CreateSampler());
}

interop::Interface<interop::GPUExternalTexture>
GPUDevice::importExternalTexture(
    Napi::Env, interop::GPUExternalTextureDescriptor descriptor) {
  UNIMPLEMENTED();
}

interop::Interface<interop::GPUBindGroupLayout>
GPUDevice::createBindGroupLayout(
    Napi::Env env, interop::GPUBindGroupLayoutDescriptor descriptor) {
  Converter conv(env);

  wgpu::BindGroupLayoutDescriptor desc{};
  if (!conv(desc.label, descriptor.label) ||
      !conv(desc.entries, desc.entryCount, descriptor.entries)) {
    return {};
  }

  return interop::GPUBindGroupLayout::Create<GPUBindGroupLayout>(
      env, device_.CreateBindGroupLayout(&desc));
}

interop::Interface<interop::GPUPipelineLayout> GPUDevice::createPipelineLayout(
    Napi::Env env, interop::GPUPipelineLayoutDescriptor descriptor) {
  Converter conv(env);

  wgpu::PipelineLayoutDescriptor desc{};
  if (!conv(desc.label, descriptor.label) ||
      !conv(desc.bindGroupLayouts, desc.bindGroupLayoutCount,
            descriptor.bindGroupLayouts)) {
    return {};
  }

  return interop::GPUPipelineLayout::Create<GPUPipelineLayout>(
      env, device_.CreatePipelineLayout(&desc));
}

interop::Interface<interop::GPUBindGroup> GPUDevice::createBindGroup(
    Napi::Env env, interop::GPUBindGroupDescriptor descriptor) {
  Converter conv(env);

  wgpu::BindGroupDescriptor desc{};
  if (!conv(desc.label, descriptor.label) ||
      !conv(desc.layout, descriptor.layout) ||
      !conv(desc.entries, desc.entryCount, descriptor.entries)) {
    return {};
  }

  return interop::GPUBindGroup::Create<GPUBindGroup>(
      env, device_.CreateBindGroup(&desc));
}

interop::Interface<interop::GPUShaderModule> GPUDevice::createShaderModule(
    Napi::Env env, interop::GPUShaderModuleDescriptor descriptor) {
  Converter conv(env);

  wgpu::ShaderModuleWGSLDescriptor wgsl_desc{};
  wgpu::ShaderModuleDescriptor sm_desc{};
  if (!conv(wgsl_desc.source, descriptor.code) ||
      !conv(sm_desc.label, descriptor.label)) {
    return {};
  }
  sm_desc.nextInChain = &wgsl_desc;

  return interop::GPUShaderModule::Create<GPUShaderModule>(
      env, device_.CreateShaderModule(&sm_desc), async_);
}

interop::Interface<interop::GPUComputePipeline>
GPUDevice::createComputePipeline(
    Napi::Env env, interop::GPUComputePipelineDescriptor descriptor) {
  Converter conv(env);

  wgpu::ComputePipelineDescriptor desc{};
  if (!conv(desc, descriptor)) {
    return {};
  }

  return interop::GPUComputePipeline::Create<GPUComputePipeline>(
      env, device_.CreateComputePipeline(&desc));
}

interop::Interface<interop::GPURenderPipeline> GPUDevice::createRenderPipeline(
    Napi::Env env, interop::GPURenderPipelineDescriptor descriptor) {
  Converter conv(env);

  wgpu::RenderPipelineDescriptor desc{};
  if (!conv(desc, descriptor)) {
    return {};
  }

  return interop::GPURenderPipeline::Create<GPURenderPipeline>(
      env, device_.CreateRenderPipeline(&desc));
}

interop::Promise<interop::Interface<interop::GPUComputePipeline>>
GPUDevice::createComputePipelineAsync(
    Napi::Env env, interop::GPUComputePipelineDescriptor descriptor) {
  Converter conv(env);

  wgpu::ComputePipelineDescriptor desc{};
  if (!conv(desc, descriptor)) {
    return {env};
  }

  using Promise =
      interop::Promise<interop::Interface<interop::GPUComputePipeline>>;

  struct Context {
    Napi::Env env;
    Promise promise;
    AsyncTask task;
  };
  auto ctx = new Context{env, env, async_};

  device_.CreateComputePipelineAsync(
      &desc,
      [](WGPUCreatePipelineAsyncStatus status, WGPUComputePipeline pipeline,
         char const* message, void* userdata) {
        auto* c = static_cast<Context*>(userdata);

        switch (status) {
          case WGPUCreatePipelineAsyncStatus::
              WGPUCreatePipelineAsyncStatus_Success:
            c->promise.Resolve(
                interop::GPUComputePipeline::Create<GPUComputePipeline>(
                    c->env, pipeline));
            break;
          default:
            c->promise.Reject(Errors::OperationError(c->env));
            break;
        }

        delete c;
      },
      ctx);

  return ctx->promise;
}

interop::Promise<interop::Interface<interop::GPURenderPipeline>>
GPUDevice::createRenderPipelineAsync(
    Napi::Env env, interop::GPURenderPipelineDescriptor descriptor) {
  Converter conv(env);

  wgpu::RenderPipelineDescriptor desc{};
  if (!conv(desc, descriptor)) {
    return {env};
  }

  using Promise =
      interop::Promise<interop::Interface<interop::GPURenderPipeline>>;

  struct Context {
    Napi::Env env;
    Promise promise;
    AsyncTask task;
  };
  auto ctx = new Context{env, env, async_};

  device_.CreateRenderPipelineAsync(
      &desc,
      [](WGPUCreatePipelineAsyncStatus status, WGPURenderPipeline pipeline,
         char const* message, void* userdata) {
        auto* c = static_cast<Context*>(userdata);

        switch (status) {
          case WGPUCreatePipelineAsyncStatus::
              WGPUCreatePipelineAsyncStatus_Success:
            c->promise.Resolve(
                interop::GPURenderPipeline::Create<GPURenderPipeline>(
                    c->env, pipeline));
            break;
          default:
            c->promise.Reject(Errors::OperationError(c->env));
            break;
        }

        delete c;
      },
      ctx);

  return ctx->promise;
}

interop::Interface<interop::GPUCommandEncoder> GPUDevice::createCommandEncoder(
    Napi::Env env,
    std::optional<interop::GPUCommandEncoderDescriptor> descriptor) {
  wgpu::CommandEncoderDescriptor desc{};
  return interop::GPUCommandEncoder::Create<GPUCommandEncoder>(
      env, device_.CreateCommandEncoder(&desc));
}

interop::Interface<interop::GPURenderBundleEncoder>
GPUDevice::createRenderBundleEncoder(
    Napi::Env env, interop::GPURenderBundleEncoderDescriptor descriptor) {
  Converter conv(env);

  wgpu::RenderBundleEncoderDescriptor desc{};
  if (!conv(desc.label, descriptor.label) ||
      !conv(desc.colorFormats, desc.colorFormatsCount,
            descriptor.colorFormats) ||
      !conv(desc.depthStencilFormat, descriptor.depthStencilFormat) ||
      !conv(desc.sampleCount, descriptor.sampleCount)) {
    return {};
  }

  return interop::GPURenderBundleEncoder::Create<GPURenderBundleEncoder>(
      env, device_.CreateRenderBundleEncoder(&desc));
}

interop::Interface<interop::GPUQuerySet> GPUDevice::createQuerySet(
    Napi::Env env, interop::GPUQuerySetDescriptor descriptor) {
  Converter conv(env);

  wgpu::QuerySetDescriptor desc{};
  if (!conv(desc.label, descriptor.label) ||
      !conv(desc.type, descriptor.type) ||
      !conv(desc.count, descriptor.count) ||
      !conv(desc.pipelineStatistics, desc.pipelineStatisticsCount,
            descriptor.pipelineStatistics)) {
    return {};
  }

  return interop::GPUQuerySet::Create<GPUQuerySet>(
      env, device_.CreateQuerySet(&desc));
}

interop::Promise<interop::Interface<interop::GPUDeviceLostInfo>>
GPUDevice::getLost(Napi::Env env) {
  auto promise =
      interop::Promise<interop::Interface<interop::GPUDeviceLostInfo>>(env);
  lost_promises_.emplace_back(promise);
  return promise;
}

void GPUDevice::pushErrorScope(Napi::Env env, interop::GPUErrorFilter filter) {
  wgpu::ErrorFilter f = wgpu::ErrorFilter::None;
  switch (filter) {
    case interop::GPUErrorFilter::kOutOfMemory:
      f = wgpu::ErrorFilter::OutOfMemory;
      break;
    case interop::GPUErrorFilter::kValidation:
      f = wgpu::ErrorFilter::Validation;
      break;
    default:
      Napi::Error::New(env, "unhandled GPUErrorFilter value")
          .ThrowAsJavaScriptException();
      return;
  }
  device_.PushErrorScope(f);
}

interop::Promise<std::optional<interop::GPUError>> GPUDevice::popErrorScope(
    Napi::Env env) {
  using Promise = interop::Promise<std::optional<interop::GPUError>>;
  struct Context {
    Napi::Env env;
    Promise promise;
    AsyncTask task;
  };
  auto* ctx = new Context{env, env, async_};

  bool ok = device_.PopErrorScope(
      [](WGPUErrorType type, char const* message, void* userdata) {
        auto* c = static_cast<Context*>(userdata);
        auto env = c->env;
        switch (type) {
          case WGPUErrorType::WGPUErrorType_NoError:
            c->promise.Resolve({});
            break;
          case WGPUErrorType::WGPUErrorType_OutOfMemory:
            c->promise.Resolve(
                interop::GPUOutOfMemoryError::Create<OOMError>(env));
            break;
          case WGPUErrorType::WGPUErrorType_Unknown:
          case WGPUErrorType::WGPUErrorType_DeviceLost:
          case WGPUErrorType::WGPUErrorType_Validation:
            c->promise.Resolve(
                interop::GPUValidationError::Create<ValidationError>(env,
                                                                     message));
            break;
          default:
            c->promise.Reject("unhandled error type");
            break;
        }
        delete c;
      },
      ctx);

  if (ok) {
    return ctx->promise;
  }

  delete ctx;
  Promise p(env);
  p.Resolve(interop::GPUValidationError::Create<ValidationError>(
      env, "failed to pop error scope"));
  return p;
}

std::optional<std::string> GPUDevice::getLabel(Napi::Env) { UNIMPLEMENTED(); };

void GPUDevice::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
};

interop::Interface<interop::EventHandler> GPUDevice::getOnuncapturederror(
    Napi::Env) {
  UNIMPLEMENTED();
}

void GPUDevice::setOnuncapturederror(
    Napi::Env, interop::Interface<interop::EventHandler> value) {
  UNIMPLEMENTED();
}

void GPUDevice::addEventListener(
    Napi::Env, std::string type,
    std::optional<interop::Interface<interop::EventListener>> callback,
    std::optional<std::variant<interop::AddEventListenerOptions, bool>>
        options) {
  UNIMPLEMENTED();
}

void GPUDevice::removeEventListener(
    Napi::Env, std::string type,
    std::optional<interop::Interface<interop::EventListener>> callback,
    std::optional<std::variant<interop::EventListenerOptions, bool>> options) {
  UNIMPLEMENTED();
}

bool GPUDevice::dispatchEvent(Napi::Env,
                              interop::Interface<interop::Event> event) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
