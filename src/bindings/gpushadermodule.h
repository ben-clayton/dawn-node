#ifndef DAWN_NODE_SRC_BINDINGS_GPUSHADERMODULE_H_
#define DAWN_NODE_SRC_BINDINGS_GPUSHADERMODULE_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUDevice;

class GPUShaderModule : public interop::GPUShaderModule {
 public:
  GPUShaderModule(wgpu::ShaderModule shader, GPUDevice* device);

  operator wgpu::ShaderModule &() { return shader_; }

  interop::Promise<interop::Interface<interop::GPUCompilationInfo>>
      compilationInfo(Napi::Env) override;

 private:
  wgpu::ShaderModule shader_;
  GPUDevice* const device_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUSHADERMODULE_H_
