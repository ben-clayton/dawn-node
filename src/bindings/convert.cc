#include "src/bindings/convert.h"

#include "src/bindings/gpubuffer.h"
#include "src/bindings/gpushadermodule.h"
#include "src/bindings/gputexture.h"
#include "src/bindings/gputextureview.h"

namespace wgpu {
namespace bindings {

bool Converter::Convert(wgpu::Extent3D& out, const interop::GPUExtent3D& in) {
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
        out.depthOrArrayLayers = (*vec)[2];
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

bool Converter::Convert(wgpu::Origin3D& out,
                        const interop::GPUOrigin3DDict& in) {
  out = {};
  out.x = in.x;
  out.y = in.y;
  out.z = in.z;
  return true;
}

bool Converter::Convert(wgpu::Color& out, const interop::GPUColor& in) {
  out = {};
  if (auto* dict = std::get_if<interop::GPUColorDict>(&in)) {
    out.r = dict->r;
    out.g = dict->g;
    out.b = dict->b;
    out.a = dict->a;
    return true;
  }
  if (auto* vec = std::get_if<std::vector<double>>(&in)) {
    switch (vec->size()) {
      default:
      case 4:
        out.a = (*vec)[3];
      case 3:  // fallthrough
        out.b = (*vec)[2];
      case 2:  // fallthrough
        out.g = (*vec)[1];
      case 1:  // fallthrough
        out.r = (*vec)[0];
        return true;
    }
  }
  Napi::Error::New(env, "invalid value for GPUColor")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::Origin3D& out,
                        const std::vector<interop::GPUIntegerCoordinate>& in) {
  out = {};
  switch (in.size()) {
    default:
    case 3:
      out.z = in[2];
    case 2:  // fallthrough
      out.y = in[1];
    case 1:  // fallthrough
      out.x = in[0];
  }
  return true;
}

bool Converter::Convert(wgpu::TextureAspect& out,
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

bool Converter::Convert(wgpu::ImageCopyTexture& out,
                        const interop::GPUImageCopyTexture& in) {
  out = {};
  return Convert(out.texture, in.texture) &&
         Convert(out.mipLevel, in.mipLevel) && Convert(out.origin, in.origin) &&
         Convert(out.aspect, in.aspect);
}

bool Converter::Convert(wgpu::ImageCopyBuffer& out,
                        const interop::GPUImageCopyBuffer& in) {
  out = {};
  out.buffer = *in.buffer.As<GPUBuffer>();
  return Convert(out.layout.offset, in.offset) &&
         Convert(out.layout.bytesPerRow, in.bytesPerRow) &&
         Convert(out.layout.rowsPerImage, in.rowsPerImage);
}

bool Converter::Convert(BufferSource& out, const interop::BufferSource& in) {
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

bool Converter::Convert(wgpu::TextureDataLayout& out,
                        const interop::GPUImageDataLayout& in) {
  out = {};
  return Convert(out.bytesPerRow, in.bytesPerRow) &&
         Convert(out.offset, in.offset) &&
         Convert(out.rowsPerImage, in.rowsPerImage);
}

bool Converter::Convert(wgpu::TextureFormat& out,
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

bool Converter::Convert(wgpu::TextureUsage& out,
                        const interop::GPUTextureUsageFlags& in) {
  out = static_cast<wgpu::TextureUsage>(in);
  return true;
}

bool Converter::Convert(wgpu::ColorWriteMask& out,
                        const interop::GPUColorWriteFlags& in) {
  out = static_cast<wgpu::ColorWriteMask>(in);
  return true;
}

bool Converter::Convert(wgpu::BufferUsage& out,
                        const interop::GPUBufferUsageFlags& in) {
  out = static_cast<wgpu::BufferUsage>(in);
  return true;
}

bool Converter::Convert(wgpu::MapMode& out,
                        const interop::GPUMapModeFlags& in) {
  out = static_cast<wgpu::MapMode>(in);
  return true;
}

bool Converter::Convert(wgpu::TextureDimension& out,
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

bool Converter::Convert(wgpu::TextureViewDimension& out,
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
    default:
      break;
  }
  Napi::Error::New(env, "invalid value for GPUTextureViewDimension")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::ProgrammableStageDescriptor& out,
                        const interop::GPUProgrammableStage& in) {
  out = {};
  out.entryPoint = in.entryPoint.c_str();
  out.module = *in.module.As<GPUShaderModule>();
  return true;
}

bool Converter::Convert(wgpu::BlendComponent& out,
                        const interop::GPUBlendComponent& in) {
  out = {};
  return Convert(out.operation, in.operation) &&
         Convert(out.dstFactor, in.dstFactor) &&
         Convert(out.srcFactor, in.srcFactor);
}

bool Converter::Convert(wgpu::BlendFactor& out,
                        const interop::GPUBlendFactor& in) {
  out = wgpu::BlendFactor::Zero;
  switch (in) {
    case interop::GPUBlendFactor::kZero:
      out = wgpu::BlendFactor::Zero;
      return true;
    case interop::GPUBlendFactor::kOne:
      out = wgpu::BlendFactor::One;
      return true;
    case interop::GPUBlendFactor::kSrc:
      out = wgpu::BlendFactor::Src;
      return true;
    case interop::GPUBlendFactor::kOneMinusSrc:
      out = wgpu::BlendFactor::OneMinusSrc;
      return true;
    case interop::GPUBlendFactor::kSrcAlpha:
      out = wgpu::BlendFactor::SrcAlpha;
      return true;
    case interop::GPUBlendFactor::kOneMinusSrcAlpha:
      out = wgpu::BlendFactor::OneMinusSrcAlpha;
      return true;
    case interop::GPUBlendFactor::kDst:
      out = wgpu::BlendFactor::Dst;
      return true;
    case interop::GPUBlendFactor::kOneMinusDst:
      out = wgpu::BlendFactor::OneMinusDst;
      return true;
    case interop::GPUBlendFactor::kDstAlpha:
      out = wgpu::BlendFactor::DstAlpha;
      return true;
    case interop::GPUBlendFactor::kOneMinusDstAlpha:
      out = wgpu::BlendFactor::OneMinusDstAlpha;
      return true;
    case interop::GPUBlendFactor::kSrcAlphaSaturated:
      out = wgpu::BlendFactor::SrcAlphaSaturated;
      return true;
    case interop::GPUBlendFactor::kConstant:
      out = wgpu::BlendFactor::Constant;
      return true;
    case interop::GPUBlendFactor::kOneMinusConstant:
      out = wgpu::BlendFactor::OneMinusConstant;
      return true;
    default:
      break;
  }
  Napi::Error::New(env, "invalid value for GPUBlendFactor")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::BlendOperation& out,
                        const interop::GPUBlendOperation& in) {
  out = wgpu::BlendOperation::Add;
  switch (in) {
    case interop::GPUBlendOperation::kAdd:
      out = wgpu::BlendOperation::Add;
      return true;
    case interop::GPUBlendOperation::kSubtract:
      out = wgpu::BlendOperation::Subtract;
      return true;
    case interop::GPUBlendOperation::kReverseSubtract:
      out = wgpu::BlendOperation::ReverseSubtract;
      return true;
    case interop::GPUBlendOperation::kMin:
      out = wgpu::BlendOperation::Min;
      return true;
    case interop::GPUBlendOperation::kMax:
      out = wgpu::BlendOperation::Max;
      return true;
    default:
      break;
  }
  Napi::Error::New(env, "invalid value for GPUBlendOperation")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::BlendState& out,
                        const interop::GPUBlendState& in) {
  out = {};
  return Convert(out.alpha, in.alpha) && Convert(out.color, in.alpha);
}

bool Converter::Convert(wgpu::PrimitiveState& out,
                        const interop::GPUPrimitiveState& in) {
  out = {};
  return Convert(out.topology, in.topology) &&
         Convert(out.stripIndexFormat, in.stripIndexFormat) &&
         Convert(out.frontFace, in.frontFace) &&
         Convert(out.cullMode, in.cullMode);
}

bool Converter::Convert(wgpu::ColorTargetState& out,
                        const interop::GPUColorTargetState& in) {
  out = {};
  if (in.blend.has_value()) {
    auto blend = std::make_unique<wgpu::BlendState>();
    if (!Convert(*blend, in.blend)) {
      return false;
    }
    out.blend = blend.get();
    blend_states.emplace_back(std::move(blend));
  }
  return Convert(out.format, in.format) && Convert(out.writeMask, in.writeMask);
}

bool Converter::Convert(wgpu::DepthStencilState& out,
                        const interop::GPUDepthStencilState& in) {
  out = {};
  return Convert(out.format, in.format) &&
         Convert(out.depthWriteEnabled, in.depthWriteEnabled) &&
         Convert(out.depthCompare, in.depthCompare) &&
         Convert(out.stencilFront, in.stencilFront) &&
         Convert(out.stencilBack, in.stencilBack) &&
         Convert(out.stencilReadMask, in.stencilReadMask) &&
         Convert(out.stencilWriteMask, in.stencilWriteMask) &&
         Convert(out.depthBias, in.depthBias) &&
         Convert(out.depthBiasSlopeScale, in.depthBiasSlopeScale) &&
         Convert(out.depthBiasClamp, in.depthBiasClamp);
}

bool Converter::Convert(wgpu::MultisampleState& out,
                        const interop::GPUMultisampleState& in) {
  out = {};
  return true;
}

bool Converter::Convert(wgpu::FragmentState& out,
                        const interop::GPUFragmentState& in) {
  out = {};
  auto targets = std::make_unique<wgpu::ColorTargetState[]>(in.targets.size());
  for (size_t i = 0; i < in.targets.size(); i++) {
    if (!Convert(targets[i], in.targets[i])) {
      return false;
    }
  }
  out.targets = targets.get();
  out.targetCount = in.targets.size();
  color_targets.emplace_back(std::move(targets));
  return Convert(out.module, in.module) &&
         Convert(out.entryPoint, in.entryPoint);
}

bool Converter::Convert(wgpu::PrimitiveTopology& out,
                        const interop::GPUPrimitiveTopology& in) {
  out = wgpu::PrimitiveTopology::LineList;
  switch (in) {
    case interop::GPUPrimitiveTopology::kPointList:
      out = wgpu::PrimitiveTopology::PointList;
      return true;
    case interop::GPUPrimitiveTopology::kLineList:
      out = wgpu::PrimitiveTopology::LineList;
      return true;
    case interop::GPUPrimitiveTopology::kLineStrip:
      out = wgpu::PrimitiveTopology::LineStrip;
      return true;
    case interop::GPUPrimitiveTopology::kTriangleList:
      out = wgpu::PrimitiveTopology::TriangleList;
      return true;
    case interop::GPUPrimitiveTopology::kTriangleStrip:
      out = wgpu::PrimitiveTopology::TriangleStrip;
      return true;
  }
  Napi::Error::New(env, "invalid value for GPUPrimitiveTopology")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::FrontFace& out, const interop::GPUFrontFace& in) {
  out = wgpu::FrontFace::CW;
  switch (in) {
    case interop::GPUFrontFace::kCw:
      out = wgpu::FrontFace::CW;
      return true;
    case interop::GPUFrontFace::kCcw:
      out = wgpu::FrontFace::CCW;
      return true;
  }
  Napi::Error::New(env, "invalid value for GPUFrontFace")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::CullMode& out, const interop::GPUCullMode& in) {
  out = wgpu::CullMode::None;
  switch (in) {
    case interop::GPUCullMode::kNone:
      out = wgpu::CullMode::None;
      return true;
    case interop::GPUCullMode::kFront:
      out = wgpu::CullMode::Front;
      return true;
    case interop::GPUCullMode::kBack:
      out = wgpu::CullMode::Back;
      return true;
  }
  Napi::Error::New(env, "invalid value for GPUCullMode")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::CompareFunction& out,
                        const interop::GPUCompareFunction& in) {
  out = wgpu::CompareFunction::Undefined;
  switch (in) {
    case interop::GPUCompareFunction::kNever:
      out = wgpu::CompareFunction::Never;
      return true;
    case interop::GPUCompareFunction::kLess:
      out = wgpu::CompareFunction::Less;
      return true;
    case interop::GPUCompareFunction::kLessEqual:
      out = wgpu::CompareFunction::LessEqual;
      return true;
    case interop::GPUCompareFunction::kGreater:
      out = wgpu::CompareFunction::Greater;
      return true;
    case interop::GPUCompareFunction::kGreaterEqual:
      out = wgpu::CompareFunction::GreaterEqual;
      return true;
    case interop::GPUCompareFunction::kEqual:
      out = wgpu::CompareFunction::Equal;
      return true;
    case interop::GPUCompareFunction::kNotEqual:
      out = wgpu::CompareFunction::NotEqual;
      return true;
    case interop::GPUCompareFunction::kAlways:
      out = wgpu::CompareFunction::Always;
      return true;
  }
  Napi::Error::New(env, "invalid value for GPUCompareFunction")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::IndexFormat& out,
                        const interop::GPUIndexFormat& in) {
  out = wgpu::IndexFormat::Undefined;
  switch (in) {
    case interop::GPUIndexFormat::kUint16:
      out = wgpu::IndexFormat::Uint16;
      return true;
    case interop::GPUIndexFormat::kUint32:
      out = wgpu::IndexFormat::Uint32;
      return true;
  }
  Napi::Error::New(env, "invalid value for GPUIndexFormat")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::StencilOperation& out,
                        const interop::GPUStencilOperation& in) {
  out = wgpu::StencilOperation::Zero;
  switch (in) {
    case interop::GPUStencilOperation::kKeep:
      out = wgpu::StencilOperation::Keep;
      return true;
    case interop::GPUStencilOperation::kZero:
      out = wgpu::StencilOperation::Zero;
      return true;
    case interop::GPUStencilOperation::kReplace:
      out = wgpu::StencilOperation::Replace;
      return true;
    case interop::GPUStencilOperation::kInvert:
      out = wgpu::StencilOperation::Invert;
      return true;
    case interop::GPUStencilOperation::kIncrementClamp:
      out = wgpu::StencilOperation::IncrementClamp;
      return true;
    case interop::GPUStencilOperation::kDecrementClamp:
      out = wgpu::StencilOperation::DecrementClamp;
      return true;
    case interop::GPUStencilOperation::kIncrementWrap:
      out = wgpu::StencilOperation::IncrementWrap;
      return true;
    case interop::GPUStencilOperation::kDecrementWrap:
      out = wgpu::StencilOperation::DecrementWrap;
      return true;
  }
  Napi::Error::New(env, "invalid value for GPUStencilOperation")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::StencilFaceState& out,
                        const interop::GPUStencilFaceState& in) {
  return Convert(out.compare, in.compare) && Convert(out.failOp, in.failOp) &&
         Convert(out.depthFailOp, in.depthFailOp) &&
         Convert(out.passOp, in.passOp);
}

bool Converter::Convert(wgpu::VertexBufferLayout& out,
                        const interop::GPUVertexBufferLayout& in) {
  out = {};
  auto attributes =
      std::make_unique<wgpu::VertexAttribute[]>(in.attributes.size());
  for (size_t i = 0; i < in.attributes.size(); i++) {
    if (!Convert(attributes[i], in.attributes[i])) {
      return false;
    }
  }
  out.attributes = attributes.get();
  out.attributeCount = in.attributes.size();
  vertex_attributes.emplace_back(std::move(attributes));
  return Convert(out.arrayStride, in.arrayStride) &&
         Convert(out.stepMode, in.stepMode);
}

bool Converter::Convert(wgpu::VertexState& out,
                        const interop::GPUVertexState& in) {
  out = {};
  out.module = *in.module.As<GPUShaderModule>();
  auto buffers =
      std::make_unique<wgpu::VertexBufferLayout[]>(in.buffers.size());
  for (size_t i = 0; i < in.buffers.size(); i++) {
    if (!Convert(buffers[i], in.buffers[i])) {
      return false;
    }
  }
  out.buffers = buffers.get();
  out.bufferCount = in.buffers.size();
  vb_layouts.emplace_back(std::move(buffers));
  return Convert(out.entryPoint, in.entryPoint);
}

bool Converter::Convert(wgpu::VertexStepMode& out,
                        const interop::GPUVertexStepMode& in) {
  out = wgpu::VertexStepMode::Instance;
  switch (in) {
    case interop::GPUVertexStepMode::kInstance:
      out = wgpu::VertexStepMode::Instance;
      return true;
    case interop::GPUVertexStepMode::kVertex:
      out = wgpu::VertexStepMode::Vertex;
      return true;
    default:
      break;
  }
  Napi::Error::New(env, "invalid value for GPUVertexStepMode")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::VertexAttribute& out,
                        const interop::GPUVertexAttribute& in) {
  return Convert(out.format, in.format) && Convert(out.offset, in.offset) &&
         Convert(out.shaderLocation, in.shaderLocation);
}

bool Converter::Convert(wgpu::VertexFormat& out,
                        const interop::GPUVertexFormat& in) {
  out = wgpu::VertexFormat::Undefined;
  switch (in) {
    case interop::GPUVertexFormat::kUint8X2:
      out = wgpu::VertexFormat::Uint8x2;
      return true;
    case interop::GPUVertexFormat::kUint8X4:
      out = wgpu::VertexFormat::Uint8x4;
      return true;
    case interop::GPUVertexFormat::kSint8X2:
      out = wgpu::VertexFormat::Sint8x2;
      return true;
    case interop::GPUVertexFormat::kSint8X4:
      out = wgpu::VertexFormat::Sint8x4;
      return true;
    case interop::GPUVertexFormat::kUnorm8X2:
      out = wgpu::VertexFormat::Unorm8x2;
      return true;
    case interop::GPUVertexFormat::kUnorm8X4:
      out = wgpu::VertexFormat::Unorm8x4;
      return true;
    case interop::GPUVertexFormat::kSnorm8X2:
      out = wgpu::VertexFormat::Snorm8x2;
      return true;
    case interop::GPUVertexFormat::kSnorm8X4:
      out = wgpu::VertexFormat::Snorm8x4;
      return true;
    case interop::GPUVertexFormat::kUint16X2:
      out = wgpu::VertexFormat::Uint16x2;
      return true;
    case interop::GPUVertexFormat::kUint16X4:
      out = wgpu::VertexFormat::Uint16x4;
      return true;
    case interop::GPUVertexFormat::kSint16X2:
      out = wgpu::VertexFormat::Sint16x2;
      return true;
    case interop::GPUVertexFormat::kSint16X4:
      out = wgpu::VertexFormat::Sint16x4;
      return true;
    case interop::GPUVertexFormat::kUnorm16X2:
      out = wgpu::VertexFormat::Unorm16x2;
      return true;
    case interop::GPUVertexFormat::kUnorm16X4:
      out = wgpu::VertexFormat::Unorm16x4;
      return true;
    case interop::GPUVertexFormat::kSnorm16X2:
      out = wgpu::VertexFormat::Snorm16x2;
      return true;
    case interop::GPUVertexFormat::kSnorm16X4:
      out = wgpu::VertexFormat::Snorm16x4;
      return true;
    case interop::GPUVertexFormat::kFloat16X2:
      out = wgpu::VertexFormat::Float16x2;
      return true;
    case interop::GPUVertexFormat::kFloat16X4:
      out = wgpu::VertexFormat::Float16x4;
      return true;
    case interop::GPUVertexFormat::kFloat32:
      out = wgpu::VertexFormat::Float32;
      return true;
    case interop::GPUVertexFormat::kFloat32X2:
      out = wgpu::VertexFormat::Float32x2;
      return true;
    case interop::GPUVertexFormat::kFloat32X3:
      out = wgpu::VertexFormat::Float32x3;
      return true;
    case interop::GPUVertexFormat::kFloat32X4:
      out = wgpu::VertexFormat::Float32x4;
      return true;
    case interop::GPUVertexFormat::kUint32:
      out = wgpu::VertexFormat::Uint32;
      return true;
    case interop::GPUVertexFormat::kUint32X2:
      out = wgpu::VertexFormat::Uint32x2;
      return true;
    case interop::GPUVertexFormat::kUint32X3:
      out = wgpu::VertexFormat::Uint32x3;
      return true;
    case interop::GPUVertexFormat::kUint32X4:
      out = wgpu::VertexFormat::Uint32x4;
      return true;
    case interop::GPUVertexFormat::kSint32:
      out = wgpu::VertexFormat::Sint32;
      return true;
    case interop::GPUVertexFormat::kSint32X2:
      out = wgpu::VertexFormat::Sint32x2;
      return true;
    case interop::GPUVertexFormat::kSint32X3:
      out = wgpu::VertexFormat::Sint32x3;
      return true;
    case interop::GPUVertexFormat::kSint32X4:
      out = wgpu::VertexFormat::Sint32x4;
      return true;
    default:
      break;
  }
  Napi::Error::New(env, "invalid value for GPUVertexFormat")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::RenderPassColorAttachment& out,
                        const interop::GPURenderPassColorAttachment& in) {
  out = {};
  if (auto* op = std::get_if<interop::GPULoadOp>(&in.loadValue)) {
    if (!Convert(out.loadOp, *op)) {
      return false;
    }
  } else if (auto* color = std::get_if<interop::GPUColor>(&in.loadValue)) {
    if (!Convert(out.clearColor, *color)) {
      return false;
    }
  } else {
    Napi::Error::New(env,
                     "invalid value for GPURenderPassColorAttachment.loadValue")
        .ThrowAsJavaScriptException();
    return false;
  }

  return Convert(out.view, in.view) &&
         Convert(out.resolveTarget, in.resolveTarget) &&
         Convert(out.storeOp, in.storeOp) &&
         Convert(out.attachment, in.resolveTarget);
}

bool Converter::Convert(
    wgpu::RenderPassDepthStencilAttachment& out,
    const interop::GPURenderPassDepthStencilAttachment& in) {
  out = {};
  if (auto* op = std::get_if<interop::GPULoadOp>(&in.depthLoadValue)) {
    if (!Convert(out.depthLoadOp, *op)) {
      return false;
    }
  } else if (auto* value = std::get_if<float>(&in.depthLoadValue)) {
    if (!Convert(out.clearDepth, *value)) {
      return false;
    }
  } else {
    Napi::Error::New(
        env,
        "invalid value for GPURenderPassDepthStencilAttachment.depthLoadValue")
        .ThrowAsJavaScriptException();
    return false;
  }

  if (auto* op = std::get_if<interop::GPULoadOp>(&in.stencilLoadValue)) {
    if (!Convert(out.stencilLoadOp, *op)) {
      return false;
    }
  } else if (auto* value =
                 std::get_if<interop::GPUStencilValue>(&in.stencilLoadValue)) {
    if (!Convert(out.clearStencil, *value)) {
      return false;
    }
  } else {
    Napi::Error::New(env,
                     "invalid value for "
                     "GPURenderPassDepthStencilAttachment.stencilLoadValue")
        .ThrowAsJavaScriptException();
    return false;
  }

  return Convert(out.view, in.view) &&
         Convert(out.depthStoreOp, in.depthStoreOp) &&
         Convert(out.depthReadOnly, in.depthReadOnly) &&
         Convert(out.stencilStoreOp, in.stencilStoreOp) &&
         Convert(out.stencilReadOnly, in.stencilReadOnly);
}

bool Converter::Convert(wgpu::LoadOp& out, const interop::GPULoadOp& in) {
  out = wgpu::LoadOp::Clear;
  switch (in) {
    case interop::GPULoadOp::kLoad:
      out = wgpu::LoadOp::Load;
      return true;
  }
  Napi::Error::New(env, "invalid value for GPULoadOp")
      .ThrowAsJavaScriptException();
  return false;
}

bool Converter::Convert(wgpu::StoreOp& out, const interop::GPUStoreOp& in) {
  out = wgpu::StoreOp::Clear;
  switch (in) {
    case interop::GPUStoreOp::kStore:
      out = wgpu::StoreOp::Store;
      return true;
    case interop::GPUStoreOp::kDiscard:
      out = wgpu::StoreOp::Discard;
      return true;
  }
  Napi::Error::New(env, "invalid value for GPUStoreOp")
      .ThrowAsJavaScriptException();
  return false;
}

}  // namespace bindings
}  // namespace wgpu
