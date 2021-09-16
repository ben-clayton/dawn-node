#include "src/bindings/gpusampler.h"

#include <cassert>

#include "src/bindings/convert.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUSampler
////////////////////////////////////////////////////////////////////////////////
GPUSampler::GPUSampler(wgpu::Sampler sampler) : sampler_(sampler) {}

std::optional<std::string> GPUSampler::getLabel(Napi::Env) { UNIMPLEMENTED(); }

void GPUSampler::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
