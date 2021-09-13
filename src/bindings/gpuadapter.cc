#include "src/bindings/gpuadapter.h"

#include <cassert>

#include "src/bindings/gpudevice.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUAdapter
////////////////////////////////////////////////////////////////////////////////
GPUAdapter::GPUAdapter(dawn_native::Adapter a) : adapter_(a) {}

std::string GPUAdapter::getName(Napi::Env) { return "dawn-adapter"; }

interop::Interface<interop::GPUSupportedFeatures> GPUAdapter::getFeatures(
    Napi::Env) {
  UNIMPLEMENTED();
}

interop::Interface<interop::GPUSupportedLimits> GPUAdapter::getLimits(
    Napi::Env) {
  UNIMPLEMENTED();
}

bool GPUAdapter::getIsFallbackAdapter(Napi::Env) { UNIMPLEMENTED(); }

interop::Promise<interop::Interface<interop::GPUDevice>>
GPUAdapter::requestDevice(
    Napi::Env env, std::optional<interop::GPUDeviceDescriptor> descriptor) {
  dawn_native::DeviceDescriptor desc{};  // TODO
  interop::Promise<interop::Interface<interop::GPUDevice>> promise(env);

  auto wgpu_device = adapter_.CreateDevice(&desc);
  if (wgpu_device) {
    promise.Resolve(
        interop::GPUDevice::Create<GPUDevice>(env, env, wgpu_device));
  } else {
    Napi::Error::New(env, "failed to create device")
        .ThrowAsJavaScriptException();
  }
  return promise;
}

}  // namespace bindings
}  // namespace wgpu
