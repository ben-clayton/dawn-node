#ifndef DAWN_NODE_SRC_BINDINGS_GPURENDERPIPELINE_H_
#define DAWN_NODE_SRC_BINDINGS_GPURENDERPIPELINE_H_

#include "dawn/webgpu_cpp.h"
#include "dawn_native/DawnNative.h"
#include "napi.h"
#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class GPURenderPipeline : public interop::GPURenderPipeline {
 public:
  GPURenderPipeline(wgpu::RenderPipeline pipeline);

  operator wgpu::RenderPipeline &() { return pipeline_; }

 private:
  wgpu::RenderPipeline pipeline_;
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_BINDINGS_GPURENDERPIPELINE_H_
