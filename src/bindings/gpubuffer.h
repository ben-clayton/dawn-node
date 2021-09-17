#ifndef DAWN_NODE_SRC_BINDINGS_GPUBUFFER_H_
#define DAWN_NODE_SRC_BINDINGS_GPUBUFFER_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "src/bindings/async_runner.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPUBuffer : public interop::GPUBuffer {
 public:
  GPUBuffer(wgpu::Buffer buffer, wgpu::BufferDescriptor desc,
            wgpu::Device device, AsyncRunner async);

  operator wgpu::Buffer() const { return buffer_; }
  const wgpu::BufferDescriptor& Desc() const { return desc_; }

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
  struct Mapping {
    uint64_t start;
    uint64_t end;
    inline bool Intersects(uint64_t s, uint64_t e) const {
      return s < end && e > start;
    }
    Napi::Reference<interop::ArrayBuffer> buffer;
  };

  // https://www.w3.org/TR/webgpu/#buffer-interface
  enum class State {
    Unmapped,
    Mapped,
    MappedAtCreation,
    MappingPending,
    Destroyed,
  };

  wgpu::Buffer buffer_;
  wgpu::BufferDescriptor const desc_;
  wgpu::Device const device_;
  AsyncRunner async_;
  State state_ = State::Unmapped;
  std::vector<Mapping> mapped_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPUBUFFER_H_
