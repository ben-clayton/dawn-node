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

  operator wgpu::QuerySet &() { return query_set_; }

 private:
  wgpu::QuerySet query_set_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUQUERYSET_H_
