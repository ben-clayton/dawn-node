#ifndef DAWN_NODE_SRC_BINDINGS_GPUQUERYSET_H_
#define DAWN_NODE_SRC_BINDINGS_GPUQUERYSET_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUQuerySet : public interop::GPUQuerySet {
 public:
  GPUQuerySet(wgpu::QuerySet query_set);

  operator wgpu::QuerySet() const { return query_set_; }

  void destroy(Napi::Env) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::QuerySet query_set_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUQUERYSET_H_
