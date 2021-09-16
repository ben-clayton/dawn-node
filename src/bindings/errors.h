#ifndef DAWN_NODE_SRC_ERRORS_H_
#define DAWN_NODE_SRC_ERRORS_H_

#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class Errors {
 public:
  using Error = interop::Interface<interop::DOMException>;
  static Error HierarchyRequestError(Napi::Env);
  static Error WrongDocumentError(Napi::Env);
  static Error InvalidCharacterError(Napi::Env);
  static Error NoModificationAllowedError(Napi::Env);
  static Error NotFoundError(Napi::Env);
  static Error NotSupportedError(Napi::Env);
  static Error InUseAttributeError(Napi::Env);
  static Error InvalidStateError(Napi::Env);
  static Error SyntaxError(Napi::Env);
  static Error InvalidModificationError(Napi::Env);
  static Error NamespaceError(Napi::Env);
  static Error SecurityError(Napi::Env);
  static Error NetworkError(Napi::Env);
  static Error AbortError(Napi::Env);
  static Error URLMismatchError(Napi::Env);
  static Error QuotaExceededError(Napi::Env);
  static Error TimeoutError(Napi::Env);
  static Error InvalidNodeTypeError(Napi::Env);
  static Error DataCloneError(Napi::Env);
  static Error EncodingError(Napi::Env);
  static Error NotReadableError(Napi::Env);
  static Error UnknownError(Napi::Env);
  static Error ConstraintError(Napi::Env);
  static Error DataError(Napi::Env);
  static Error TransactionInactiveError(Napi::Env);
  static Error ReadOnlyError(Napi::Env);
  static Error VersionError(Napi::Env);
  static Error OperationError(Napi::Env);
  static Error NotAllowedError(Napi::Env);
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_ERRORS_H_
