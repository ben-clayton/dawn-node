#ifndef DAWN_NODE_SRC_BINDINGS_GPU_H_
#define DAWN_NODE_SRC_BINDINGS_GPU_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPU : public interop::GPU {
 public:
  GPU();

  interop::Promise<std::optional<interop::Interface<interop::GPUAdapter>>>
  requestAdapter(
      Napi::Env env,
      std::optional<interop::GPURequestAdapterOptions> options) override;

 private:
  dawn_native::Instance instance_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPU_H_
