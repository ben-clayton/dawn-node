#include "src/bindings/gpushadermodule.h"

#include "src/bindings/gpudevice.h"
#include "src/utils/debug.h"

namespace wgpu {
namespace bindings {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUShaderModule
////////////////////////////////////////////////////////////////////////////////
GPUShaderModule::GPUShaderModule(wgpu::ShaderModule shader, GPUDevice* device)
    : shader_(shader), device_(device) {}

interop::Promise<interop::Interface<interop::GPUCompilationInfo>>
GPUShaderModule::compilationInfo(Napi::Env env) {
  struct GPUCompilationMessage : public interop::GPUCompilationMessage {
    WGPUCompilationMessage message;

    GPUCompilationMessage(const WGPUCompilationMessage& m) : message(m) {}
    std::string getMessage(Napi::Env) override { return message.message; }
    interop::GPUCompilationMessageType getType(Napi::Env) override {
      switch (message.type) {
        case WGPUCompilationMessageType_Error:
          return interop::GPUCompilationMessageType::kError;
        case WGPUCompilationMessageType_Warning:
          return interop::GPUCompilationMessageType::kWarning;
        case WGPUCompilationMessageType_Info:
          return interop::GPUCompilationMessageType::kInfo;
        default:
          return interop::GPUCompilationMessageType::kInvalidEnum;
      }
    }
    unsigned long long getLineNum(Napi::Env) override {
      return message.lineNum;
    }
    unsigned long long getLinePos(Napi::Env) override {
      return message.linePos;
    }
    unsigned long long getOffset(Napi::Env) override { return message.offset; }
    unsigned long long getLength(Napi::Env) override { return message.length; }
  };

  using Messages =
      interop::FrozenArray<interop::Interface<interop::GPUCompilationMessage>>;

  struct GPUCompilationInfo : public interop::GPUCompilationInfo {
    Messages messages;

    GPUCompilationInfo(Messages&& m) : messages(std::move(m)) {}
    Messages getMessages(Napi::Env) override { return messages; }
  };

  using Promise =
      interop::Promise<interop::Interface<interop::GPUCompilationInfo>>;

  struct Context {
    Napi::Env env;
    Promise promise;
    GPUDevice* device;
  };
  auto ctx = new Context{env, env, device_};

  device_->BeginAsync();
  shader_.GetCompilationInfo(
      [](WGPUCompilationInfoRequestStatus status,
         WGPUCompilationInfo const* compilationInfo, void* userdata) {
        auto* c = static_cast<Context*>(userdata);

        Messages messages(compilationInfo->messageCount);
        for (uint32_t i = 0; i < compilationInfo->messageCount; i++) {
          auto& msg = compilationInfo->messages[i];
          messages[i] =
              interop::GPUCompilationMessage::Create<GPUCompilationMessage>(
                  c->env, msg);
        }

        c->promise.Resolve(
            interop::GPUCompilationInfo::Create<GPUCompilationInfo>(
                c->env, std::move(messages)));
        c->device->EndAsync();
        delete c;
      },
      ctx);

  return ctx->promise;
}

std::optional<std::string> GPUShaderModule::getLabel(Napi::Env) {
  UNIMPLEMENTED();
}

void GPUShaderModule::setLabel(Napi::Env, std::optional<std::string> value) {
  UNIMPLEMENTED();
}

}  // namespace bindings
}  // namespace wgpu
