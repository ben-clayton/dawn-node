#ifndef DAWN_NODE_SRC_BINDINGS_CONVERT_H_
#define DAWN_NODE_SRC_BINDINGS_CONVERT_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

[[nodiscard]] inline bool Convert(wgpu::Extent3D& out,
                                  const interop::GPUExtent3D& in) {
  out = {};
  if (auto* dict = std::get_if<interop::GPUExtent3DDict>(&in)) {
    out.depthOrArrayLayers = dict->depthOrArrayLayers;
    out.width = dict->width;
    out.height = dict->height;
    return true;
  }
  if (auto* vec =
          std::get_if<std::vector<interop::GPUIntegerCoordinate>>(&in)) {
    switch (vec->size()) {
      default:
      case 3:
        out.depthOrArrayLayers = (*vec)[3];
      case 2:  // fallthrough
        out.height = (*vec)[1];
      case 1:  // fallthrough
        out.width = (*vec)[0];
        return true;
    }
  }
  return false;
}

[[nodiscard]] inline bool Convert(wgpu::Origin3D& out,
                                  const interop::GPUOrigin3D& in) {
  out = {};
  if (auto* dict = std::get_if<interop::GPUOrigin3DDict>(&in)) {
    out.x = dict->x;
    out.y = dict->y;
    out.z = dict->z;
    return true;
  }
  if (auto* vec =
          std::get_if<std::vector<interop::GPUIntegerCoordinate>>(&in)) {
    switch (vec->size()) {
      default:
      case 3:
        out.z = (*vec)[3];
      case 2:  // fallthrough
        out.y = (*vec)[1];
      case 1:  // fallthrough
        out.x = (*vec)[0];
        return true;
    }
  }
  return false;
}

[[nodiscard]] inline bool Convert(wgpu::TextureAspect& out,
                                  const interop::GPUTextureAspect& in) {
  out = wgpu::TextureAspect::All;
  switch (in) {
    case interop::GPUTextureAspect::kAll:
      out = wgpu::TextureAspect::All;
      return true;
    case interop::GPUTextureAspect::kStencilOnly:
      out = wgpu::TextureAspect::StencilOnly;
      return true;
    case interop::GPUTextureAspect::kDepthOnly:
      out = wgpu::TextureAspect::DepthOnly;
      return true;
  }
  return false;
}

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_CONVERT_H_
