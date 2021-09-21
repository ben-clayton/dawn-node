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

#ifndef DAWN_NODE_SRC_BINDINGS_ASYNC_RUNNER_H_
#define DAWN_NODE_SRC_BINDINGS_ASYNC_RUNNER_H_

#include <memory>

#include "dawn/webgpu_cpp.h"
#include "napi.h"

namespace wgpu {
namespace bindings {

class AsyncRunnerState {
 public:
  AsyncRunnerState(Napi::Env env, wgpu::Device device);
  void Begin();
  void End();

 private:
  void QueueTick();

  Napi::Env env_;
  wgpu::Device const device_;
  int count_ = 0;
};

using AsyncRunner = std::shared_ptr<AsyncRunnerState>;

class AsyncTask {
 public:
  inline AsyncTask(AsyncRunner runner) : runner_(runner) { runner->Begin(); };
  inline ~AsyncTask() { runner_->End(); }

 private:
  AsyncRunner runner_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_ASYNC_RUNNER_H_
