#include "src/bindings/gpubuffer.h"

#include "src/bindings/convert.h"
#include "src/bindings/gpudevice.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUBuffer
////////////////////////////////////////////////////////////////////////////////
GPUBuffer::GPUBuffer(wgpu::Buffer buffer, wgpu::BufferDescriptor desc,
                     GPUDevice* device)
    : buffer_(buffer), desc_(desc), device_(device) {}

interop::Promise<void> GPUBuffer::mapAsync(
    Napi::Env env, interop::GPUMapModeFlags mode,
    std::optional<interop::GPUSize64> offset,
    std::optional<interop::GPUSize64> size) {
  wgpu::MapMode md{};
  Converter conv(env);
  if (!conv(md, mode)) {
    return {env};
  }

  // LOG("mode: ", md, ", offset: ", offset, ", size: ", size);

  struct Context {
    interop::Promise<void> promise;
    GPUDevice* device;
  };
  auto ctx = new Context{interop::Promise<void>(env), device_};

  uint64_t o = offset.has_value() ? offset.value() : 0;
  uint64_t s = size.has_value() ? size.value() : (desc_.size - o);

  device_->BeginAsync();
  buffer_.MapAsync(
      md, o, s,
      [](WGPUBufferMapAsyncStatus status, void* userdata) {
        auto* c = static_cast<Context*>(userdata);
        c->promise.Resolve();
        c->device->EndAsync();
        delete c;
      },
      ctx);

  return ctx->promise;
}

interop::ArrayBuffer GPUBuffer::getMappedRange(
    Napi::Env env, std::optional<interop::GPUSize64> offset,
    std::optional<interop::GPUSize64> size) {
  // LOG("offset: ", offset, ", size: ", size);
  uint64_t o = offset.has_value() ? offset.value() : 0;
  uint64_t s = size.has_value() ? size.value() : (desc_.size - o);
  auto* ptr = (desc_.usage & wgpu::BufferUsage::MapWrite)
                  ? buffer_.GetMappedRange(o, s)
                  : const_cast<void*>(buffer_.GetConstMappedRange(o, s));
  if (!ptr) {
    Napi::Error::New(env, "failed to map buffer").ThrowAsJavaScriptException();
    return {};
  }
  return Napi::ArrayBuffer::New(env, ptr, s);
}

void GPUBuffer::unmap(Napi::Env) { buffer_.Unmap(); }

void GPUBuffer::destroy(Napi::Env) { buffer_.Release(); }

std::optional<std::string> GPUBuffer::getLabel(Napi::Env) { UNIMPLEMENTED(); }

void GPUBuffer::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
