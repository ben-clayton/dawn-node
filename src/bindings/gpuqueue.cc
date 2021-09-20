#include "src/bindings/gpuqueue.h"

#include <cassert>

#include "src/bindings/convert.h"
#include "src/bindings/gpubuffer.h"
#include "src/bindings/gpucommandbuffer.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUQueue
////////////////////////////////////////////////////////////////////////////////
GPUQueue::GPUQueue(wgpu::Queue queue, AsyncRunner async)
    : queue_(queue), async_(async) {}

void GPUQueue::submit(
    Napi::Env env,
    std::vector<interop::Interface<interop::GPUCommandBuffer>> commandBuffers) {
  std::vector<wgpu::CommandBuffer> bufs(commandBuffers.size());
  for (size_t i = 0; i < commandBuffers.size(); i++) {
    bufs[i] = *commandBuffers[i].As<GPUCommandBuffer>();
  }
  Converter conv(env);
  uint32_t bufs_size;
  if (!conv(bufs_size, bufs.size())) {
    return;
  }
  queue_.Submit(bufs_size, bufs.data());
}

interop::Promise<void> GPUQueue::onSubmittedWorkDone(Napi::Env env) {
  struct Context {
    Napi::Env env;
    interop::Promise<void> promise;
    AsyncTask task;
  };
  auto ctx = new Context{env, interop::Promise<void>(env), async_};

  queue_.OnSubmittedWorkDone(
      0,
      [](WGPUQueueWorkDoneStatus status, void* userdata) {
        auto* c = static_cast<Context*>(userdata);
        if (status !=
            WGPUQueueWorkDoneStatus::WGPUQueueWorkDoneStatus_Success) {
          Napi::Error::New(c->env, "onSubmittedWorkDone() failed")
              .ThrowAsJavaScriptException();
        }
        c->promise.Resolve();
        delete c;
      },
      ctx);

  return ctx->promise;
}

void GPUQueue::writeBuffer(Napi::Env env,
                           interop::Interface<interop::GPUBuffer> buffer,
                           interop::GPUSize64 bufferOffset,
                           interop::BufferSource data,
                           std::optional<interop::GPUSize64> dataOffset,
                           std::optional<interop::GPUSize64> size) {
  wgpu::Buffer buf = *buffer.As<GPUBuffer>();
  Converter::BufferSource src{};
  Converter conv(env);
  if (!conv(src, data)) {
    return;
  }

  // TODO: Bounds check
  if (dataOffset.has_value()) {
    if (src.data) {
      src.data = reinterpret_cast<uint8_t*>(src.data) + dataOffset.value();
    }
    src.size -= dataOffset.value();
  }
  if (size.has_value()) {
    src.size = size.value();
  }

  queue_.WriteBuffer(buf, bufferOffset, src.data, src.size);
}

void GPUQueue::writeTexture(Napi::Env env,
                            interop::GPUImageCopyTexture destination,
                            interop::BufferSource data,
                            interop::GPUImageDataLayout dataLayout,
                            interop::GPUExtent3D size) {
  wgpu::ImageCopyTexture dst{};
  Converter::BufferSource src{};
  wgpu::TextureDataLayout layout{};
  wgpu::Extent3D sz{};
  Converter conv(env);
  if (!conv(dst, destination) ||    //
      !conv(src, data) ||           //
      !conv(layout, dataLayout) ||  //
      !conv(sz, size)) {
    return;
  }

  queue_.WriteTexture(&dst, src.data, src.size, &layout, &sz);
}

void GPUQueue::copyExternalImageToTexture(
    Napi::Env, interop::GPUImageCopyExternalImage source,
    interop::GPUImageCopyTextureTagged destination,
    interop::GPUExtent3D copySize) {
  UNIMPLEMENTED();
}

std::optional<std::string> GPUQueue::getLabel(Napi::Env) { UNIMPLEMENTED(); }

void GPUQueue::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
