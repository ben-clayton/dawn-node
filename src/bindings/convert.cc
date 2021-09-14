#include "src/bindings/convert.h"

#include "src/bindings/gpubuffer.h"
#include "src/bindings/gputexture.h"

namespace wgpu {
namespace bindings {

bool Convert(Napi::Env env, wgpu::Extent3D& out,
             const interop::GPUExtent3D& in) {
  out = {};
  if (auto* dict = std::get_if<interop::GPUExtent3DDict>(&in)) {
    out.depthOrArrayLayers = dict->depthOrArrayLayers.value_or(1);
    out.width = dict->width;
    out.height = dict->height.value_or(1);
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
  Napi::Error::New(env, "invalid value for GPUExtent3D")
      .ThrowAsJavaScriptException();
  return false;
}

bool Convert(Napi::Env env, wgpu::Origin3D& out,
             const interop::GPUOrigin3D& in) {
  out = {};
  if (auto* dict = std::get_if<interop::GPUOrigin3DDict>(&in)) {
    out.x = dict->x.value_or(0);
    out.y = dict->y.value_or(0);
    out.z = dict->z.value_or(0);
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
  Napi::Error::New(env, "invalid value for GPUOrigin3D")
      .ThrowAsJavaScriptException();
  return false;
}

bool Convert(Napi::Env env, wgpu::TextureAspect& out,
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
  Napi::Error::New(env, "invalid value for GPUTextureAspect")
      .ThrowAsJavaScriptException();
  return false;
}

bool Convert(Napi::Env env, wgpu::ImageCopyTexture& out,
             const interop::GPUImageCopyTexture& in) {
  out = {};
  out.texture = *in.texture.As<GPUTexture>();
  out.mipLevel = in.mipLevel.value_or(0);
  wgpu::Extent3D size{};
  if (in.origin.has_value()) {
    if (!Convert(env, out.origin, in.origin.value())) {
      return false;
    }
  }
  if (in.aspect.has_value()) {
    if (!Convert(env, out.aspect, in.aspect.value())) {
      return false;
    }
  }
  return true;
}

bool Convert(Napi::Env env, wgpu::ImageCopyBuffer& out,
             const interop::GPUImageCopyBuffer& in) {
  out = {};
  out.buffer = *in.buffer.As<GPUBuffer>();
  out.layout.offset = in.offset.value_or(0);
  out.layout.bytesPerRow = in.bytesPerRow.value_or(0);
  out.layout.rowsPerImage = in.rowsPerImage.value_or(0);
  return true;
}

bool Convert(Napi::Env env, BufferSource& out,
             const interop::BufferSource& in) {
  out = {};
  if (auto* view = std::get_if<interop::ArrayBufferView>(&in)) {
    std::visit(
        [&](auto&& v) {
          auto arr = v.ArrayBuffer();
          out.data = arr.Data();
          out.size = arr.ByteLength();
        },
        *view);
    return true;
  }
  if (auto* buf = std::get_if<interop::ArrayBuffer>(&in)) {
    return true;
  }
  Napi::Error::New(env, "invalid value for BufferSource")
      .ThrowAsJavaScriptException();
  return false;
}

bool Convert(Napi::Env env, wgpu::TextureDataLayout& out,
             const interop::GPUImageDataLayout& in) {
  out = {};
  out.bytesPerRow = in.bytesPerRow.value_or(0);
  out.offset = in.offset.value_or(0);
  out.rowsPerImage = in.rowsPerImage.value_or(0);
  return true;
}

bool Convert(Napi::Env env, wgpu::TextureFormat& out,
             const interop::GPUTextureFormat& in) {
  out = wgpu::TextureFormat::Undefined;
  switch (in) {
    case interop::GPUTextureFormat::kR8Unorm:
      out = wgpu::TextureFormat::R8Unorm;
      return true;
    case interop::GPUTextureFormat::kR8Snorm:
      out = wgpu::TextureFormat::R8Snorm;
      return true;
    case interop::GPUTextureFormat::kR8Uint:
      out = wgpu::TextureFormat::R8Uint;
      return true;
    case interop::GPUTextureFormat::kR8Sint:
      out = wgpu::TextureFormat::R8Sint;
      return true;
    case interop::GPUTextureFormat::kR16Uint:
      out = wgpu::TextureFormat::R16Uint;
      return true;
    case interop::GPUTextureFormat::kR16Sint:
      out = wgpu::TextureFormat::R16Sint;
      return true;
    case interop::GPUTextureFormat::kR16Float:
      out = wgpu::TextureFormat::R16Float;
      return true;
    case interop::GPUTextureFormat::kRg8Unorm:
      out = wgpu::TextureFormat::RG8Unorm;
      return true;
    case interop::GPUTextureFormat::kRg8Snorm:
      out = wgpu::TextureFormat::RG8Snorm;
      return true;
    case interop::GPUTextureFormat::kRg8Uint:
      out = wgpu::TextureFormat::RG8Uint;
      return true;
    case interop::GPUTextureFormat::kRg8Sint:
      out = wgpu::TextureFormat::RG8Sint;
      return true;
    case interop::GPUTextureFormat::kR32Uint:
      out = wgpu::TextureFormat::R32Uint;
      return true;
    case interop::GPUTextureFormat::kR32Sint:
      out = wgpu::TextureFormat::R32Sint;
      return true;
    case interop::GPUTextureFormat::kR32Float:
      out = wgpu::TextureFormat::R32Float;
      return true;
    case interop::GPUTextureFormat::kRg16Uint:
      out = wgpu::TextureFormat::RG16Uint;
      return true;
    case interop::GPUTextureFormat::kRg16Sint:
      out = wgpu::TextureFormat::RG16Sint;
      return true;
    case interop::GPUTextureFormat::kRg16Float:
      out = wgpu::TextureFormat::RG16Float;
      return true;
    case interop::GPUTextureFormat::kRgba8Unorm:
      out = wgpu::TextureFormat::RGBA8Unorm;
      return true;
    case interop::GPUTextureFormat::kRgba8UnormSrgb:
      out = wgpu::TextureFormat::RGBA8UnormSrgb;
      return true;
    case interop::GPUTextureFormat::kRgba8Snorm:
      out = wgpu::TextureFormat::RGBA8Snorm;
      return true;
    case interop::GPUTextureFormat::kRgba8Uint:
      out = wgpu::TextureFormat::RGBA8Uint;
      return true;
    case interop::GPUTextureFormat::kRgba8Sint:
      out = wgpu::TextureFormat::RGBA8Sint;
      return true;
    case interop::GPUTextureFormat::kBgra8Unorm:
      out = wgpu::TextureFormat::BGRA8Unorm;
      return true;
    case interop::GPUTextureFormat::kBgra8UnormSrgb:
      out = wgpu::TextureFormat::BGRA8UnormSrgb;
      return true;
    case interop::GPUTextureFormat::kRgb9E5Ufloat:
      out = wgpu::TextureFormat::RGB9E5Ufloat;
      return true;
    case interop::GPUTextureFormat::kRgb10A2Unorm:
      out = wgpu::TextureFormat::RGB10A2Unorm;
      return true;
    case interop::GPUTextureFormat::kRg11B10Ufloat:
      out = wgpu::TextureFormat::RG11B10Ufloat;
      return true;
    case interop::GPUTextureFormat::kRg32Uint:
      out = wgpu::TextureFormat::RG32Uint;
      return true;
    case interop::GPUTextureFormat::kRg32Sint:
      out = wgpu::TextureFormat::RG32Sint;
      return true;
    case interop::GPUTextureFormat::kRg32Float:
      out = wgpu::TextureFormat::RG32Float;
      return true;
    case interop::GPUTextureFormat::kRgba16Uint:
      out = wgpu::TextureFormat::RGBA16Uint;
      return true;
    case interop::GPUTextureFormat::kRgba16Sint:
      out = wgpu::TextureFormat::RGBA16Sint;
      return true;
    case interop::GPUTextureFormat::kRgba16Float:
      out = wgpu::TextureFormat::RGBA16Float;
      return true;
    case interop::GPUTextureFormat::kRgba32Uint:
      out = wgpu::TextureFormat::RGBA32Uint;
      return true;
    case interop::GPUTextureFormat::kRgba32Sint:
      out = wgpu::TextureFormat::RGBA32Sint;
      return true;
    case interop::GPUTextureFormat::kRgba32Float:
      out = wgpu::TextureFormat::RGBA32Float;
      return true;
    case interop::GPUTextureFormat::kStencil8:
      out = wgpu::TextureFormat::Stencil8;
      return true;
    case interop::GPUTextureFormat::kDepth16Unorm:
      break;  // TODO: ???
    case interop::GPUTextureFormat::kDepth24Plus:
      out = wgpu::TextureFormat::Depth24Plus;
      return true;
    case interop::GPUTextureFormat::kDepth24PlusStencil8:
      out = wgpu::TextureFormat::Depth24PlusStencil8;
      return true;
    case interop::GPUTextureFormat::kDepth32Float:
      out = wgpu::TextureFormat::Depth32Float;
      return true;
    case interop::GPUTextureFormat::kBc1RgbaUnorm:
      out = wgpu::TextureFormat::BC1RGBAUnorm;
      return true;
    case interop::GPUTextureFormat::kBc1RgbaUnormSrgb:
      out = wgpu::TextureFormat::BC1RGBAUnormSrgb;
      return true;
    case interop::GPUTextureFormat::kBc2RgbaUnorm:
      out = wgpu::TextureFormat::BC2RGBAUnorm;
      return true;
    case interop::GPUTextureFormat::kBc2RgbaUnormSrgb:
      out = wgpu::TextureFormat::BC2RGBAUnormSrgb;
      return true;
    case interop::GPUTextureFormat::kBc3RgbaUnorm:
      out = wgpu::TextureFormat::BC3RGBAUnorm;
      return true;
    case interop::GPUTextureFormat::kBc3RgbaUnormSrgb:
      out = wgpu::TextureFormat::BC3RGBAUnormSrgb;
      return true;
    case interop::GPUTextureFormat::kBc4RUnorm:
      out = wgpu::TextureFormat::BC4RUnorm;
      return true;
    case interop::GPUTextureFormat::kBc4RSnorm:
      out = wgpu::TextureFormat::BC4RSnorm;
      return true;
    case interop::GPUTextureFormat::kBc5RgUnorm:
      out = wgpu::TextureFormat::BC5RGUnorm;
      return true;
    case interop::GPUTextureFormat::kBc5RgSnorm:
      out = wgpu::TextureFormat::BC5RGSnorm;
      return true;
    case interop::GPUTextureFormat::kBc6HRgbUfloat:
      out = wgpu::TextureFormat::BC6HRGBUfloat;
      return true;
    case interop::GPUTextureFormat::kBc6HRgbFloat:
      out = wgpu::TextureFormat::BC6HRGBFloat;
      return true;
    case interop::GPUTextureFormat::kBc7RgbaUnorm:
      out = wgpu::TextureFormat::BC7RGBAUnorm;
      return true;
    case interop::GPUTextureFormat::kBc7RgbaUnormSrgb:
      out = wgpu::TextureFormat::BC7RGBAUnormSrgb;
      return true;
    case interop::GPUTextureFormat::kDepth24UnormStencil8:
      out = wgpu::TextureFormat::Depth24PlusStencil8;
      return true;
    case interop::GPUTextureFormat::kDepth32FloatStencil8:
      break;  // TODO: ???
  }
  Napi::Error::New(env, "invalid value for GPUTextureFormat")
      .ThrowAsJavaScriptException();
  return false;
}

bool Convert(Napi::Env env, wgpu::TextureUsage& out,
             const interop::GPUTextureUsageFlags& in) {
  out = static_cast<wgpu::TextureUsage>(in);
  return true;
}

bool Convert(Napi::Env env, wgpu::TextureDimension& out,
             const interop::GPUTextureDimension& in) {
  out = wgpu::TextureDimension::e1D;
  switch (in) {
    case interop::GPUTextureDimension::k1D:
      out = wgpu::TextureDimension::e1D;
      return true;
    case interop::GPUTextureDimension::k2D:
      out = wgpu::TextureDimension::e2D;
      return true;
    case interop::GPUTextureDimension::k3D:
      out = wgpu::TextureDimension::e3D;
      return true;
  }
  Napi::Error::New(env, "invalid value for GPUTextureDimension")
      .ThrowAsJavaScriptException();
  return false;
}

bool Convert(Napi::Env env, wgpu::TextureViewDimension& out,
             const interop::GPUTextureViewDimension& in) {
  out = wgpu::TextureViewDimension::Undefined;
  switch (in) {
    case interop::GPUTextureViewDimension::k1D:
      out = wgpu::TextureViewDimension::e1D;
      return true;
    case interop::GPUTextureViewDimension::k2D:
      out = wgpu::TextureViewDimension::e2D;
      return true;
    case interop::GPUTextureViewDimension::k2DArray:
      out = wgpu::TextureViewDimension::e2DArray;
      return true;
    case interop::GPUTextureViewDimension::kCube:
      out = wgpu::TextureViewDimension::Cube;
      return true;
    case interop::GPUTextureViewDimension::kCubeArray:
      out = wgpu::TextureViewDimension::CubeArray;
      return true;
    case interop::GPUTextureViewDimension::k3D:
      out = wgpu::TextureViewDimension::e3D;
      return true;
  }
  Napi::Error::New(env, "invalid value for GPUTextureViewDimension")
      .ThrowAsJavaScriptException();
  return false;
}

bool Convert(Napi::Env env, wgpu::BufferUsage& out,
             const interop::GPUBufferUsageFlags& in) {
  out = static_cast<wgpu::BufferUsage>(in);
  return true;
}

bool Convert(Napi::Env env, wgpu::MapMode& out,
             const interop::GPUMapModeFlags& in) {
  out = static_cast<wgpu::MapMode>(in);
  return true;
}

}  // namespace bindings
}  // namespace wgpu
