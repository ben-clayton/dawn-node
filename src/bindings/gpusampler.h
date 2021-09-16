#ifndef DAWN_NODE_SRC_BINDINGS_GPUSAMPLER_H_
#define DAWN_NODE_SRC_BINDINGS_GPUSAMPLER_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUSampler : public interop::GPUSampler {
 public:
  GPUSampler(wgpu::Sampler sampler);

  operator wgpu::Sampler &() { return sampler_; }

  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::Sampler sampler_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUSAMPLER_H_
