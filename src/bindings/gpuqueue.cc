#include "src/bindings/gpuqueue.h"

#include <cassert>

#include "src/bindings/gpucommandbuffer.h"
#include "src/bindings/gpudevice.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUQueue
////////////////////////////////////////////////////////////////////////////////
GPUQueue::GPUQueue(wgpu::Queue queue, GPUDevice* device)
    : queue_(queue), device_(device) {}

void GPUQueue::submit(
    Napi::Env,
    std::vector<interop::Interface<interop::GPUCommandBuffer>> commandBuffers) {
  std::vector<wgpu::CommandBuffer> bufs(commandBuffers.size());
  for (size_t i = 0; i < commandBuffers.size(); i++) {
    bufs[i] = *commandBuffers[i].As<GPUCommandBuffer>();
  }
  queue_.Submit(bufs.size(), bufs.data());
}

interop::Promise<void> GPUQueue::onSubmittedWorkDone(Napi::Env env) {
  struct Context {
    Napi::Env env;
    interop::Promise<void> promise;
    GPUDevice* device;
  };
  auto ctx = new Context{env, interop::Promise<void>(env), device_};

  device_->BeginAsync();
  queue_.OnSubmittedWorkDone(
      1,  // TODO: What should this value be?
      [](WGPUQueueWorkDoneStatus status, void* userdata) {
        auto* c = static_cast<Context*>(userdata);
        if (status !=
            WGPUQueueWorkDoneStatus::WGPUQueueWorkDoneStatus_Success) {
          Napi::Error::New(c->env, "onSubmittedWorkDone() failed")
              .ThrowAsJavaScriptException();
        }
        c->promise.Resolve();
        c->device->EndAsync();
        delete c;
      },
      ctx);

  return ctx->promise;
}

void GPUQueue::writeBuffer(Napi::Env,
                           interop::Interface<interop::GPUBuffer> buffer,
                           interop::GPUSize64 bufferOffset,
                           interop::BufferSource data,
                           std::optional<interop::GPUSize64> dataOffset,
                           std::optional<interop::GPUSize64> size) {
  UNIMPLEMENTED();
}

void GPUQueue::writeTexture(Napi::Env, interop::GPUImageCopyTexture destination,
                            interop::BufferSource data,
                            interop::GPUImageDataLayout dataLayout,
                            interop::GPUExtent3D size) {
  UNIMPLEMENTED();
}

void GPUQueue::copyExternalImageToTexture(
    Napi::Env, interop::GPUImageCopyExternalImage source,
    interop::GPUImageCopyTextureTagged destination,
    interop::GPUExtent3D copySize) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
