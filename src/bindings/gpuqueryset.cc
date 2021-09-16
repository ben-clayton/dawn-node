#include "src/bindings/gpuqueryset.h"

#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUQuerySet
////////////////////////////////////////////////////////////////////////////////
GPUQuerySet::GPUQuerySet(wgpu::QuerySet query_set) : query_set_(query_set) {}

void GPUQuerySet::destroy(Napi::Env) { query_set_.Release(); }

std::optional<std::string> GPUQuerySet::getLabel(Napi::Env) { UNIMPLEMENTED(); }

void GPUQuerySet::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
