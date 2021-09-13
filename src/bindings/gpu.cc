#include "src/bindings/gpu.h"

#include <cassert>

#include "src/bindings/gpuadapter.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPU
////////////////////////////////////////////////////////////////////////////////
GPU::GPU() { instance_.DiscoverDefaultAdapters(); }

interop::Promise<std::optional<interop::Interface<interop::GPUAdapter>>>
GPU::requestAdapter(Napi::Env env,
                    std::optional<interop::GPURequestAdapterOptions> options) {
  auto promise =
      interop::Promise<std::optional<interop::Interface<interop::GPUAdapter>>>(
          env);

  if (options.has_value() && options->forceFallbackAdapter) {
    // Software adapters are not currently supported.
    promise.Resolve({});
    return promise;
  }

  auto adapters = instance_.GetAdapters();
  if (adapters.empty()) {
    promise.Resolve({});
    return promise;
  }

  auto adapter = GPUAdapter::Create<GPUAdapter>(env, adapters[0]);
  promise.Resolve(
      std::optional<interop::Interface<interop::GPUAdapter>>(adapter));
  return promise;
}

}  // namespace bindings
}  // namespace wgpu
