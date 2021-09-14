#ifndef DAWN_NODE_SRC_BINDINGS_GPUBINDGROUPLAYOUT_H_
#define DAWN_NODE_SRC_BINDINGS_GPUBINDGROUPLAYOUT_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUDevice;

class GPUBindGroupLayout : public interop::GPUBindGroupLayout {
 public:
  GPUBindGroupLayout(wgpu::BindGroupLayout layout);

  operator wgpu::BindGroupLayout &() { return layout_; }

 private:
  wgpu::BindGroupLayout layout_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUBINDGROUPLAYOUT_H_
