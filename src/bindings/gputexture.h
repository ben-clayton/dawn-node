#ifndef DAWN_NODE_SRC_BINDINGS_GPUTEXTURE_H_
#define DAWN_NODE_SRC_BINDINGS_GPUTEXTURE_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUTexture : public interop::GPUTexture {
 public:
  GPUTexture(wgpu::Texture texture);

  operator wgpu::Texture() const { return texture_; }

  interop::Interface<interop::GPUTextureView> createView(
      Napi::Env,
      std::optional<interop::GPUTextureViewDescriptor> descriptor) override;
  void destroy(Napi::Env) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::Texture texture_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUTEXTURE_H_
