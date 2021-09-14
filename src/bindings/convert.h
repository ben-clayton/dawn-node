#ifndef DAWN_NODE_SRC_BINDINGS_CONVERT_H_
#define DAWN_NODE_SRC_BINDINGS_CONVERT_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

[[nodiscard]] bool Convert(wgpu::Extent3D& out, const interop::GPUExtent3D& in);

[[nodiscard]] bool Convert(wgpu::Origin3D& out, const interop::GPUOrigin3D& in);

[[nodiscard]] bool Convert(wgpu::TextureAspect& out,
                           const interop::GPUTextureAspect& in);

[[nodiscard]] bool Convert(wgpu::ImageCopyTexture& out,
                           const interop::GPUImageCopyTexture& in);

[[nodiscard]] bool Convert(wgpu::ImageCopyBuffer& out,
                           const interop::GPUImageCopyBuffer& in);

struct BufferSource {
  void* data;
  size_t size;
};

[[nodiscard]] bool Convert(BufferSource& out, const interop::BufferSource& in);

[[nodiscard]] bool Convert(wgpu::TextureDataLayout& out,
                           const interop::GPUImageDataLayout& in);

[[nodiscard]] bool Convert(wgpu::TextureFormat& out,
                           const interop::GPUTextureFormat& in);

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_CONVERT_H_
