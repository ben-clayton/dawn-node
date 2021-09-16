#ifndef DAWN_NODE_SRC_BINDINGS_GPUSHADERMODULE_H_
#define DAWN_NODE_SRC_BINDINGS_GPUSHADERMODULE_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "src/bindings/async_runner.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUShaderModule : public interop::GPUShaderModule {
 public:
  GPUShaderModule(wgpu::ShaderModule shader, AsyncRunner async);

  operator wgpu::ShaderModule() const { return shader_; }

  interop::Promise<interop::Interface<interop::GPUCompilationInfo>>
      compilationInfo(Napi::Env) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::ShaderModule shader_;
  AsyncRunner async_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUSHADERMODULE_H_
