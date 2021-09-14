#ifndef DAWN_NODE_SRC_BINDINGS_CONVERT_H_
#define DAWN_NODE_SRC_BINDINGS_CONVERT_H_

#include <type_traits>

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

template <typename T>
struct ImplOf {};

#define DECLARE_IMPL(NAME)       \
  class NAME;                    \
  template <>                    \
  struct ImplOf<interop::NAME> { \
    using type = NAME;           \
  }

DECLARE_IMPL(GPUBuffer);
DECLARE_IMPL(GPUPipelineLayout);
DECLARE_IMPL(GPUQuerySet);
DECLARE_IMPL(GPURenderPipeline);
DECLARE_IMPL(GPUShaderModule);
DECLARE_IMPL(GPUTexture);
DECLARE_IMPL(GPUTextureView);
#undef DECLARE_IMPL

class Converter {
 public:
  Converter(Napi::Env e) : env(e) {}

  template <typename OUT, typename IN>
  [[nodiscard]] inline bool operator()(OUT&& out, IN&& in) {
    return Convert(std::forward<OUT>(out), std::forward<IN>(in));
  }

  [[nodiscard]] bool Convert(wgpu::Extent3D& out,
                             const interop::GPUExtent3D& in);

  [[nodiscard]] bool Convert(wgpu::Origin3D& out,
                             const interop::GPUOrigin3DDict& in);

  [[nodiscard]] bool Convert(wgpu::Color& out, const interop::GPUColor& in);

  [[nodiscard]] bool Convert(
      wgpu::Origin3D& out,
      const std::vector<interop::GPUIntegerCoordinate>& in);

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

  [[nodiscard]] bool Convert(BufferSource& out,
                             const interop::BufferSource& in);

  [[nodiscard]] bool Convert(wgpu::TextureDataLayout& out,
                             const interop::GPUImageDataLayout& in);

  [[nodiscard]] bool Convert(wgpu::TextureFormat& out,
                             const interop::GPUTextureFormat& in);

  [[nodiscard]] bool Convert(wgpu::TextureUsage& out,
                             const interop::GPUTextureUsageFlags& in);

  [[nodiscard]] bool Convert(wgpu::ColorWriteMask& out,
                             const interop::GPUColorWriteFlags& in);

  [[nodiscard]] bool Convert(wgpu::BufferUsage& out,
                             const interop::GPUBufferUsageFlags& in);

  [[nodiscard]] bool Convert(wgpu::MapMode& out,
                             const interop::GPUMapModeFlags& in);

  [[nodiscard]] bool Convert(wgpu::TextureDimension& out,
                             const interop::GPUTextureDimension& in);

  [[nodiscard]] bool Convert(wgpu::TextureViewDimension& out,
                             const interop::GPUTextureViewDimension& in);

  [[nodiscard]] bool Convert(wgpu::ProgrammableStageDescriptor& out,
                             const interop::GPUProgrammableStage& in);

  [[nodiscard]] bool Convert(wgpu::BlendComponent& out,
                             const interop::GPUBlendComponent& in);

  [[nodiscard]] bool Convert(wgpu::BlendFactor& out,
                             const interop::GPUBlendFactor& in);

  [[nodiscard]] bool Convert(wgpu::BlendOperation& out,
                             const interop::GPUBlendOperation& in);

  [[nodiscard]] bool Convert(wgpu::BlendState& out,
                             const interop::GPUBlendState& in);

  [[nodiscard]] bool Convert(wgpu::PrimitiveState& out,
                             const interop::GPUPrimitiveState& in);

  [[nodiscard]] bool Convert(wgpu::ColorTargetState& out,
                             const interop::GPUColorTargetState& in);

  [[nodiscard]] bool Convert(wgpu::DepthStencilState& out,
                             const interop::GPUDepthStencilState& in);

  [[nodiscard]] bool Convert(wgpu::MultisampleState& out,
                             const interop::GPUMultisampleState& in);

  [[nodiscard]] bool Convert(wgpu::FragmentState& out,
                             const interop::GPUFragmentState& in);

  [[nodiscard]] bool Convert(wgpu::PrimitiveTopology& out,
                             const interop::GPUPrimitiveTopology& in);

  [[nodiscard]] bool Convert(wgpu::FrontFace& out,
                             const interop::GPUFrontFace& in);

  [[nodiscard]] bool Convert(wgpu::CullMode& out,
                             const interop::GPUCullMode& in);

  [[nodiscard]] bool Convert(wgpu::CompareFunction& out,
                             const interop::GPUCompareFunction& in);

  [[nodiscard]] bool Convert(wgpu::IndexFormat& out,
                             const interop::GPUIndexFormat& in);

  [[nodiscard]] bool Convert(wgpu::StencilOperation& out,
                             const interop::GPUStencilOperation& in);

  [[nodiscard]] bool Convert(wgpu::StencilFaceState& out,
                             const interop::GPUStencilFaceState& in);

  [[nodiscard]] bool Convert(wgpu::VertexState& out,
                             const interop::GPUVertexState& in);

  [[nodiscard]] bool Convert(wgpu::VertexBufferLayout& out,
                             const interop::GPUVertexBufferLayout& in);

  [[nodiscard]] bool Convert(wgpu::VertexStepMode& out,
                             const interop::GPUVertexStepMode& in);

  [[nodiscard]] bool Convert(wgpu::VertexAttribute& out,
                             const interop::GPUVertexAttribute& in);

  [[nodiscard]] bool Convert(wgpu::VertexFormat& out,
                             const interop::GPUVertexFormat& in);

  [[nodiscard]] bool Convert(wgpu::RenderPassColorAttachment& out,
                             const interop::GPURenderPassColorAttachment& in);

  [[nodiscard]] bool Convert(
      wgpu::RenderPassDepthStencilAttachment& out,
      const interop::GPURenderPassDepthStencilAttachment& in);

  [[nodiscard]] bool Convert(wgpu::LoadOp& out, const interop::GPULoadOp& in);

  [[nodiscard]] bool Convert(wgpu::StoreOp& out, const interop::GPUStoreOp& in);

  inline bool Convert(const char*& out, const std::string& in) {
    out = in.c_str();
    return true;
  }

  template <typename T>
  inline bool Convert(T& out, const T& in) {
    out = in;
    return true;
  }

  template <typename OUT, typename IN,
            typename = std::enable_if_t<std::is_integral_v<IN> &&
                                        std::is_integral_v<OUT>>>
  inline bool Convert(OUT& out, const IN& in) {
    out = static_cast<OUT>(in);
    return true;
  }

  template <typename OUT, typename... IN_TYPES>
  inline bool Convert(OUT& out, const std::variant<IN_TYPES...>& in) {
    return std::visit([&](auto&& i) { return Convert(out, i); }, in);
  }

  template <typename OUT, typename IN>
  inline bool Convert(OUT& out, const std::optional<IN>& in) {
    if (in.has_value()) {
      return Convert(out, in.value());
    }
    return true;
  }

  template <typename OUT, typename IN>
  inline bool Convert(OUT& out, const interop::Interface<IN>& in) {
    using Impl = typename ImplOf<IN>::type;
    out = *in.template As<Impl>();
    return true;
  }

 private:
  Napi::Env env;
  using VertexBufferLayouts = std::unique_ptr<wgpu::VertexBufferLayout[]>;
  std::vector<VertexBufferLayouts> vb_layouts;
  using VertexAttributes = std::unique_ptr<wgpu::VertexAttribute[]>;
  std::vector<VertexAttributes> vertex_attributes;
  using ColorTargets = std::unique_ptr<wgpu::ColorTargetState[]>;
  std::vector<ColorTargets> color_targets;
  using BlendStates = std::unique_ptr<wgpu::BlendState>;
  std::vector<BlendStates> blend_states;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_CONVERT_H_
