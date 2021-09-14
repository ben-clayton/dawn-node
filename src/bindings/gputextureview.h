#ifndef DAWN_NODE_SRC_BINDINGS_GPUTEXTUREVIEW_H_
#define DAWN_NODE_SRC_BINDINGS_GPUTEXTUREVIEW_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUTextureView : public interop::GPUTextureView {
 public:
  GPUTextureView(wgpu::TextureView view);

  operator wgpu::TextureView &() { return view_; }

 private:
  wgpu::TextureView view_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUTEXTUREVIEW_H_
