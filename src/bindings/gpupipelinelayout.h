#ifndef DAWN_NODE_SRC_BINDINGS_GPUPIPELINELAYOUT_H_
#define DAWN_NODE_SRC_BINDINGS_GPUPIPELINELAYOUT_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUDevice;

class GPUPipelineLayout : public interop::GPUPipelineLayout {
 public:
  GPUPipelineLayout(wgpu::PipelineLayout layout);

  operator wgpu::PipelineLayout &() { return layout_; }

  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::PipelineLayout layout_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUPIPELINELAYOUT_H_
