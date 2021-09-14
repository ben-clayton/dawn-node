#ifndef DAWN_NODE_SRC_BINDINGS_GPUBUFFER_H_
#define DAWN_NODE_SRC_BINDINGS_GPUBUFFER_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUDevice;

class GPUBuffer : public interop::GPUBuffer {
 public:
  GPUBuffer(wgpu::Buffer buffer, wgpu::BufferDescriptor desc,
            GPUDevice* device);

  operator wgpu::Buffer &() { return buffer_; }

  interop::Promise<void> mapAsync(
      Napi::Env env, interop::GPUMapModeFlags mode,
      std::optional<interop::GPUSize64> offset,
      std::optional<interop::GPUSize64> size) override;
  interop::ArrayBuffer getMappedRange(
      Napi::Env env, std::optional<interop::GPUSize64> offset,
      std::optional<interop::GPUSize64> size) override;
  void unmap(Napi::Env) override;
  void destroy(Napi::Env) override;
  std::optional<std::string> getLabel(Napi::Env) override;
  void setLabel(Napi::Env, std::optional<std::string> value) override;

 private:
  wgpu::Buffer buffer_;
  wgpu::BufferDescriptor const desc_;
  GPUDevice* device_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUBUFFER_H_
