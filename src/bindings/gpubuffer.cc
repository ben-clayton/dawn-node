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
    : buffer_(buffer), desc_(desc), device_(device), async_(async) {
  if (desc.mappedAtCreation) {
    state_ = State::MappedAtCreation;
  }
  if (desc.size % 4) {
    device_.InjectError(wgpu::ErrorType::Validation,
                        "Buffer size must be a multiple of 4 bytes");
  }
}

interop::Promise<void> GPUBuffer::mapAsync(
    Napi::Env env, interop::GPUMapModeFlags mode,
    std::optional<interop::GPUSize64> offset,
    std::optional<interop::GPUSize64> size) {
  wgpu::MapMode md{};
  Converter conv(env);
  if (!conv(md, mode)) {
    return {env};
  }

  if (state_ != State::Unmapped) {
    interop::Promise<void> promise(env);
    promise.Reject(Errors::OperationError(env));
    device_.InjectError(
        wgpu::ErrorType::Validation,
        "mapAsync called on buffer that is not in the unmapped state");
    return promise;
  }

  struct Context {
    Napi::Env env;
    interop::Promise<void> promise;
    AsyncTask task;
    State& state;
  };
  auto ctx = new Context{env, interop::Promise<void>(env), async_, state_};
  auto promise = ctx->promise;

  uint64_t o = offset.has_value() ? offset.value() : 0;
  uint64_t s = size.has_value() ? size.value() : (desc_.size - o);

  state_ = State::MappingPending;

  buffer_.MapAsync(
      md, o, s,
      [](WGPUBufferMapAsyncStatus status, void* userdata) {
        auto* c = static_cast<Context*>(userdata);

        c->state = State::Unmapped;
        switch (status) {
          case WGPUBufferMapAsyncStatus::WGPUBufferMapAsyncStatus_Success:
            c->promise.Resolve();
            c->state = State::Mapped;
            break;
          case WGPUBufferMapAsyncStatus::WGPUBufferMapAsyncStatus_Error:
            c->promise.Reject(Errors::OperationError(c->env));
            break;
          case WGPUBufferMapAsyncStatus::
              WGPUBufferMapAsyncStatus_UnmappedBeforeCallback:
            c->promise.Reject(Errors::AbortError(c->env));
            break;
          case WGPUBufferMapAsyncStatus::WGPUBufferMapAsyncStatus_Unknown:
          case WGPUBufferMapAsyncStatus::WGPUBufferMapAsyncStatus_DeviceLost:
          case WGPUBufferMapAsyncStatus::
              WGPUBufferMapAsyncStatus_DestroyedBeforeCallback:
            // TODO: The spec is a bit vague around what the promise should do
            // here.
            c->promise.Reject(Errors::UnknownError(c->env));
            break;
        }

        delete c;
      },
      ctx);

  return promise;
}

interop::ArrayBuffer GPUBuffer::getMappedRange(
    Napi::Env env, std::optional<interop::GPUSize64> offset,
    std::optional<interop::GPUSize64> size) {
  if (state_ != State::Mapped && state_ != State::MappedAtCreation) {
    Errors::OperationError(env).ThrowAsJavaScriptException();
    return {};
  }

  uint64_t o = offset.has_value() ? offset.value() : 0;
  uint64_t s = size.has_value() ? size.value() : (desc_.size - o);

  uint64_t start = o;
  uint64_t end = o + s;
  for (auto& mapping : mapped_) {
    if (mapping.Intersects(start, end)) {
      Errors::OperationError(env).ThrowAsJavaScriptException();
      return {};
    }
  }

  auto* ptr = (desc_.usage & wgpu::BufferUsage::MapWrite)
                  ? buffer_.GetMappedRange(o, s)
                  : const_cast<void*>(buffer_.GetConstMappedRange(o, s));
  if (!ptr) {
    Errors::OperationError(env).ThrowAsJavaScriptException();
    return {};
  }
  auto array_buffer = Napi::ArrayBuffer::New(env, ptr, s);
  mapped_.emplace_back(Mapping{start, end, Napi::Persistent(array_buffer)});
  return array_buffer;
}

void GPUBuffer::unmap(Napi::Env env) {
  if (state_ == State::Destroyed) {
    device_.InjectError(wgpu::ErrorType::Validation,
                        "unmap() called on a destroyed buffer");
    return;
  }

  for (auto& mapping : mapped_) {
    mapping.buffer.Value().Detach();
  }
  mapped_.clear();
  buffer_.Unmap();
  state_ = State::Unmapped;
}

void GPUBuffer::destroy(Napi::Env) {
  buffer_.Release();
  state_ = State::Destroyed;
}

std::optional<std::string> GPUBuffer::getLabel(Napi::Env) { UNIMPLEMENTED(); }

void GPUBuffer::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
