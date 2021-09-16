
#include "src/bindings/async_runner.h"

#include <cassert>

namespace wgpu {
namespace bindings {

AsyncRunnerState::AsyncRunnerState(Napi::Env env, wgpu::Device device)
    : env_(env), device_(device) {}

void AsyncRunnerState::Begin() {
  if (count_++ == 0) {
    QueueTick();
  }
}

void AsyncRunnerState::End() {
  assert(count_ > 0);
  count_--;
}

void AsyncRunnerState::QueueTick() {
  env_.Global()
      .Get("setImmediate")
      .As<Napi::Function>()
      .Call({
          // TODO: Create once, reuse.
          Napi::Function::New(env_,
                              [this](const Napi::CallbackInfo&) {
                                if (count_ > 0) {
                                  device_.Tick();
                                  QueueTick();
                                }
                              }),
      });
}

}  // namespace bindings
}  // namespace wgpu
