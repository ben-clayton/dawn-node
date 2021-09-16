#ifndef DAWN_NODE_SRC_BINDINGS_CONVERT_H_
#define DAWN_NODE_SRC_BINDINGS_CONVERT_H_

#include <functional>
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

DECLARE_IMPL(GPUBindGroup);
DECLARE_IMPL(GPUBindGroupLayout);
DECLARE_IMPL(GPUBuffer);
DECLARE_IMPL(GPUPipelineLayout);
DECLARE_IMPL(GPUQuerySet);
DECLARE_IMPL(GPURenderBundle);
DECLARE_IMPL(GPURenderPipeline);
DECLARE_IMPL(GPUSampler);
DECLARE_IMPL(GPUShaderModule);
DECLARE_IMPL(GPUTexture);
DECLARE_IMPL(GPUTextureView);
#undef DECLARE_IMPL

class Converter {
 public:
  Converter(Napi::Env e) : env(e) {}
  ~Converter();

  template <typename OUT, typename IN>
  [[nodiscard]] inline bool operator()(OUT&& out, IN&& in) {
    return Convert(std::forward<OUT>(out), std::forward<IN>(in));
  }

  template <typename OUT_A, typename OUT_B, typename IN>
  [[nodiscard]] inline bool operator()(OUT_A&& out_a, OUT_B&& out_b, IN&& in) {
    return Convert(std::forward<OUT_A>(out_a), std::forward<OUT_B>(out_b),
                   std::forward<IN>(in));
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

  [[nodiscard]] bool Convert(BufferSource& out, interop::BufferSource in);

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

  [[nodiscard]] bool Convert(wgpu::ShaderStage& out,
                             const interop::GPUShaderStageFlags& in);

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

  [[nodiscard]] bool Convert(wgpu::BindGroupEntry& out,
                             const interop::GPUBindGroupEntry& in);

  [[nodiscard]] bool Convert(wgpu::BindGroupLayoutEntry& out,
                             const interop::GPUBindGroupLayoutEntry& in);

  [[nodiscard]] bool Convert(wgpu::BufferBindingLayout& out,
                             const interop::GPUBufferBindingLayout& in);

  [[nodiscard]] bool Convert(wgpu::SamplerBindingLayout& out,
                             const interop::GPUSamplerBindingLayout& in);

  [[nodiscard]] bool Convert(wgpu::TextureBindingLayout& out,
                             const interop::GPUTextureBindingLayout& in);

  [[nodiscard]] bool Convert(wgpu::StorageTextureBindingLayout& out,
                             const interop::GPUStorageTextureBindingLayout& in);

  [[nodiscard]] bool Convert(wgpu::BufferBindingType& out,
                             const interop::GPUBufferBindingType& in);

  [[nodiscard]] bool Convert(wgpu::SamplerBindingType& out,
                             const interop::GPUSamplerBindingType& in);

  [[nodiscard]] bool Convert(wgpu::TextureSampleType& out,
                             const interop::GPUTextureSampleType& in);

  [[nodiscard]] bool Convert(wgpu::StorageTextureAccess& out,
                             const interop::GPUStorageTextureAccess& in);

  [[nodiscard]] bool Convert(wgpu::QueryType& out,
                             const interop::GPUQueryType& in);

  [[nodiscard]] bool Convert(wgpu::PipelineStatisticName& out,
                             const interop::GPUPipelineStatisticName& in);

  [[nodiscard]] bool Convert(wgpu::AddressMode& out,
                             const interop::GPUAddressMode& in);

  [[nodiscard]] bool Convert(wgpu::FilterMode& out,
                             const interop::GPUFilterMode& in);

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

  template <typename OUT, typename IN,
            typename _ = std::enable_if_t<!std::is_same_v<IN, std::string>>>
  inline bool Convert(OUT*& out, const std::optional<IN>& in) {
    if (in.has_value()) {
      auto* el = Allocate<std::remove_const_t<OUT>>();
      if (!Convert(*el, in.value())) {
        return false;
      }
      out = el;
    }
    return true;
  }

  template <typename OUT, typename IN>
  inline bool Convert(OUT& out, const interop::Interface<IN>& in) {
    using Impl = typename ImplOf<IN>::type;
    out = *in.template As<Impl>();
    return true;
  }

  template <typename OUT, typename IN>
  inline bool Convert(OUT*& out_els, uint32_t& out_count,
                      const std::vector<IN>& in) {
    auto* els = Allocate<std::remove_const_t<OUT>>(in.size());
    for (size_t i = 0; i < in.size(); i++) {
      if (!Convert(els[i], in[i])) {
        return false;
      }
    }
    out_els = els;
    out_count = in.size();
    return true;
  }

 private:
  Napi::Env env;

  template <typename T>
  T* Allocate(size_t n = 1) {
    auto* ptr = new T[n]{};
    free_.emplace_back([ptr] { delete[] ptr; });
    return ptr;
  }

  std::vector<std::function<void()>> free_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_CONVERT_H_
