#include "src/bindings/gpubuffer.h"

#include "src/bindings/convert.h"
#include "src/bindings/errors.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUBuffer
////////////////////////////////////////////////////////////////////////////////
GPUBuffer::GPUBuffer(wgpu::Buffer buffer, wgpu::BufferDescriptor desc,
                     wgpu::Device device, AsyncRunner async)
    : buffer_(buffer), desc_(desc), device_(device), async_(async) {}

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

  if (!buffer_) {
    interop::Promise<void> promise(env);
    promise.Reject(Errors::OperationError(env));
    device_.InjectError(wgpu::ErrorType::Validation,
                        "mapAsync called on destroyed buffer");
    return promise;
  }

  struct Context {
    Napi::Env env;
    interop::Promise<void> promise;
    AsyncTask task;
  };
  auto ctx = new Context{env, interop::Promise<void>(env), async_};

  uint64_t o = offset.has_value() ? offset.value() : 0;
  uint64_t s = size.has_value() ? size.value() : (desc_.size - o);

  buffer_.MapAsync(
      md, o, s,
      [](WGPUBufferMapAsyncStatus status, void* userdata) {
        auto* c = static_cast<Context*>(userdata);
        switch (status) {
          case WGPUBufferMapAsyncStatus::WGPUBufferMapAsyncStatus_Success:
            c->promise.Resolve();
            break;
          case WGPUBufferMapAsyncStatus::WGPUBufferMapAsyncStatus_Error:
            c->promise.Reject(Errors::OperationError(c->env));
            break;
          case WGPUBufferMapAsyncStatus::WGPUBufferMapAsyncStatus_Unknown:
          case WGPUBufferMapAsyncStatus::WGPUBufferMapAsyncStatus_DeviceLost:
          case WGPUBufferMapAsyncStatus::
              WGPUBufferMapAsyncStatus_DestroyedBeforeCallback:
          case WGPUBufferMapAsyncStatus::
              WGPUBufferMapAsyncStatus_UnmappedBeforeCallback:
            c->promise.Reject(Errors::UnknownError(c->env));
            break;
        }

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
  auto array_buffer = Napi::ArrayBuffer::New(env, ptr, s);
  mapped_.emplace_back(Napi::Persistent(array_buffer));
  return array_buffer;
}

void GPUBuffer::unmap(Napi::Env) {
  for (auto& mapped : mapped_) {
    mapped.Value().Detach();
  }
  mapped_.clear();
  buffer_.Unmap();
}

void GPUBuffer::destroy(Napi::Env) { buffer_.Release(); }

std::optional<std::string> GPUBuffer::getLabel(Napi::Env) { UNIMPLEMENTED(); }

void GPUBuffer::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
