// Copyright 2021 The Dawn Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
