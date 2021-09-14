#include "src/bindings/gpuqueryset.h"

#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUQuerySet
////////////////////////////////////////////////////////////////////////////////
GPUQuerySet::GPUQuerySet(wgpu::QuerySet query_set) : query_set_(query_set) {}

}  // namespace bindings
}  // namespace wgpu
