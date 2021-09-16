#ifndef DAWN_NODE_SRC_BINDINGS_GPURENDERBUNDLE_H_
#define DAWN_NODE_SRC_BINDINGS_GPURENDERBUNDLE_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPURenderBundle : public interop::GPURenderBundle {
 public:
  GPURenderBundle(wgpu::RenderBundle bundle);

  operator wgpu::RenderBundle() const { return bundle_; }
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::RenderBundle bundle_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPURENDERBUNDLE_H_
