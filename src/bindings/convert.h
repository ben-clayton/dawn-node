#ifndef DAWN_NODE_SRC_BINDINGS_CONVERT_H_
#define DAWN_NODE_SRC_BINDINGS_CONVERT_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

[[nodiscard]] bool Convert(Napi::Env env, wgpu::Extent3D& out,
                           const interop::GPUExtent3D& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::Origin3D& out,
                           const interop::GPUOrigin3D& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::TextureAspect& out,
                           const interop::GPUTextureAspect& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::ImageCopyTexture& out,
                           const interop::GPUImageCopyTexture& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::ImageCopyBuffer& out,
                           const interop::GPUImageCopyBuffer& in);

struct BufferSource {
  void* data;
  size_t size;
};

[[nodiscard]] bool Convert(Napi::Env env, BufferSource& out,
                           const interop::BufferSource& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::TextureDataLayout& out,
                           const interop::GPUImageDataLayout& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::TextureFormat& out,
                           const interop::GPUTextureFormat& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::TextureUsage& out,
                           const interop::GPUTextureUsageFlags& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::TextureDimension& out,
                           const interop::GPUTextureDimension& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::TextureViewDimension& out,
                           const interop::GPUTextureViewDimension& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::BufferUsage& out,
                           const interop::GPUBufferUsageFlags& in);

[[nodiscard]] bool Convert(Napi::Env env, wgpu::MapMode& out,
                           const interop::GPUMapModeFlags& in);

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_CONVERT_H_
