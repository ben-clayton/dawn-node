#include "src/bindings/gpurenderbundle.h"

#include <cassert>

#include "src/bindings/convert.h"
#include "src/bindings/gpubuffer.h"
#include "src/bindings/gpurenderbundle.h"
#include "src/bindings/gpurenderpipeline.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPURenderBundle
////////////////////////////////////////////////////////////////////////////////
GPURenderBundle::GPURenderBundle(wgpu::RenderBundle bundle) : bundle_(bundle) {}

std::optional<std::string> GPURenderBundle::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPURenderBundle::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
