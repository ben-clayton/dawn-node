#ifndef DAWN_NODE_SRC_BINDINGS_GPUBINDGROUP_H_
#define DAWN_NODE_SRC_BINDINGS_GPUBINDGROUP_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUBindGroup : public interop::GPUBindGroup {
 public:
  GPUBindGroup(wgpu::BindGroup group);

  operator wgpu::BindGroup() const { return group_; }

  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::BindGroup group_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUBINDGROUP_H_
