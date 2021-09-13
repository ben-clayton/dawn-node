#ifndef DAWN_NODE_SRC_BINDINGS_GPUADAPTER_H_
#define DAWN_NODE_SRC_BINDINGS_GPUADAPTER_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUAdapter : public interop::GPUAdapter {
 public:
  GPUAdapter(dawn_native::Adapter a);

  std::string getName(Napi::Env) override;
  interop::Interface<interop::GPUSupportedFeatures> getFeatures(
      Napi::Env) override;
  interop::Interface<interop::GPUSupportedLimits> getLimits(Napi::Env) override;
  bool getIsFallbackAdapter(Napi::Env) override;
  interop::Promise<interop::Interface<interop::GPUDevice>> requestDevice(
      Napi::Env env,
      std::optional<interop::GPUDeviceDescriptor> descriptor) override;

 private:
  dawn_native::Adapter adapter_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUADAPTER_H_
