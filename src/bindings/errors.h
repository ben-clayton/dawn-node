#ifndef DAWN_NODE_SRC_ERRORS_H_
#define DAWN_NODE_SRC_ERRORS_H_

#include "webgpu_interop.h"

namespace wgpu {
namespace bindings {

class Errors {
 public:
  static Napi::Error HierarchyRequestError(Napi::Env);
  static Napi::Error WrongDocumentError(Napi::Env);
  static Napi::Error InvalidCharacterError(Napi::Env);
  static Napi::Error NoModificationAllowedError(Napi::Env);
  static Napi::Error NotFoundError(Napi::Env);
  static Napi::Error NotSupportedError(Napi::Env);
  static Napi::Error InUseAttributeError(Napi::Env);
  static Napi::Error InvalidStateError(Napi::Env);
  static Napi::Error SyntaxError(Napi::Env);
  static Napi::Error InvalidModificationError(Napi::Env);
  static Napi::Error NamespaceError(Napi::Env);
  static Napi::Error SecurityError(Napi::Env);
  static Napi::Error NetworkError(Napi::Env);
  static Napi::Error AbortError(Napi::Env);
  static Napi::Error URLMismatchError(Napi::Env);
  static Napi::Error QuotaExceededError(Napi::Env);
  static Napi::Error TimeoutError(Napi::Env);
  static Napi::Error InvalidNodeTypeError(Napi::Env);
  static Napi::Error DataCloneError(Napi::Env);
  static Napi::Error EncodingError(Napi::Env);
  static Napi::Error NotReadableError(Napi::Env);
  static Napi::Error UnknownError(Napi::Env);
  static Napi::Error ConstraintError(Napi::Env);
  static Napi::Error DataError(Napi::Env);
  static Napi::Error TransactionInactiveError(Napi::Env);
  static Napi::Error ReadOnlyError(Napi::Env);
  static Napi::Error VersionError(Napi::Env);
  static Napi::Error OperationError(Napi::Env);
  static Napi::Error NotAllowedError(Napi::Env);
};

}  // namespace bindings
}  // namespace wgpu

#endif  // DAWN_NODE_SRC_ERRORS_H_
