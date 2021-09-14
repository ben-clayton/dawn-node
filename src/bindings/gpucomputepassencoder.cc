#include "src/bindings/gpucomputepassencoder.h"

#include <cassert>

#include "src/bindings/gpubuffer.h"
#include "src/bindings/gpucomputepipeline.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUComputePassEncoder
////////////////////////////////////////////////////////////////////////////////
GPUComputePassEncoder::GPUComputePassEncoder(wgpu::ComputePassEncoder enc)
    : enc_(enc) {}

void GPUComputePassEncoder::setPipeline(
    Napi::Env, interop::Interface<interop::GPUComputePipeline> pipeline) {
  enc_.SetPipeline(*pipeline.As<GPUComputePipeline>());
}

void GPUComputePassEncoder::dispatch(Napi::Env, interop::GPUSize32 x,
                                     std::optional<interop::GPUSize32> y,
                                     std::optional<interop::GPUSize32> z) {
  enc_.Dispatch(x, y.value_or(1), z.value_or(1));
}

void GPUComputePassEncoder::dispatchIndirect(
    Napi::Env, interop::Interface<interop::GPUBuffer> indirectBuffer,
    interop::GPUSize64 indirectOffset) {
  enc_.DispatchIndirect(*indirectBuffer.As<GPUBuffer>(), indirectOffset);
}

void GPUComputePassEncoder::beginPipelineStatisticsQuery(
    Napi::Env, interop::Interface<interop::GPUQuerySet> querySet,
    interop::GPUSize32 queryIndex) {
  UNIMPLEMENTED();
}

void GPUComputePassEncoder::endPipelineStatisticsQuery(Napi::Env) {
  UNIMPLEMENTED();
}

void GPUComputePassEncoder::writeTimestamp(
    Napi::Env, interop::Interface<interop::GPUQuerySet> querySet,
    interop::GPUSize32 queryIndex) {
  UNIMPLEMENTED();
}

void GPUComputePassEncoder::endPass(Napi::Env) { enc_.EndPass(); }

}  // namespace bindings
}  // namespace wgpu
