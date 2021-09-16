
#include "src/bindings/errors.h"

namespace wgpu {
namespace bindings {

namespace {
const char* kHierarchyRequestError = "HierarchyRequestError";
const char* kWrongDocumentError = "WrongDocumentError";
const char* kInvalidCharacterError = "InvalidCharacterError";
const char* kNoModificationAllowedError = "NoModificationAllowedError";
const char* kNotFoundError = "NotFoundError";
const char* kNotSupportedError = "NotSupportedError";
const char* kInUseAttributeError = "InUseAttributeError";
const char* kInvalidStateError = "InvalidStateError";
const char* kSyntaxError = "SyntaxError";
const char* kInvalidModificationError = "InvalidModificationError";
const char* kNamespaceError = "NamespaceError";
const char* kSecurityError = "SecurityError";
const char* kNetworkError = "NetworkError";
const char* kAbortError = "AbortError";
const char* kURLMismatchError = "URLMismatchError";
const char* kQuotaExceededError = "QuotaExceededError";
const char* kTimeoutError = "TimeoutError";
const char* kInvalidNodeTypeError = "InvalidNodeTypeError";
const char* kDataCloneError = "DataCloneError";
const char* kEncodingError = "EncodingError";
const char* kNotReadableError = "NotReadableError";
const char* kUnknownError = "UnknownError";
const char* kConstraintError = "ConstraintError";
const char* kDataError = "DataError";
const char* kTransactionInactiveError = "TransactionInactiveError";
const char* kReadOnlyError = "ReadOnlyError";
const char* kVersionError = "VersionError";
const char* kOperationError = "OperationError";
const char* kNotAllowedError = "NotAllowedError";

class DOMException : public interop::DOMException {
 public:
  std::string name_;
  std::string message_;
  unsigned short code_;

  DOMException(std::string name, std::string message, unsigned short code)
      : name_(name), message_(message), code_(code) {}

  std::string getName(Napi::Env) override { return name_; }

  std::string getMessage(Napi::Env) override { return message_; }

  unsigned short getCode(Napi::Env) override { return code_; }
};

}  // namespace

Errors::Error Errors::HierarchyRequestError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(
      env, kHierarchyRequestError, kHierarchyRequestError, 0);
}

Errors::Error Errors::WrongDocumentError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kWrongDocumentError,
                                                     kWrongDocumentError, 0);
}

Errors::Error Errors::InvalidCharacterError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(
      env, kInvalidCharacterError, kInvalidCharacterError, 0);
}

Errors::Error Errors::NoModificationAllowedError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(
      env, kNoModificationAllowedError, kNoModificationAllowedError, 0);
}

Errors::Error Errors::NotFoundError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kNotFoundError,
                                                     kNotFoundError, 0);
}

Errors::Error Errors::NotSupportedError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kNotSupportedError,
                                                     kNotSupportedError, 0);
}

Errors::Error Errors::InUseAttributeError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kInUseAttributeError,
                                                     kInUseAttributeError, 0);
}

Errors::Error Errors::InvalidStateError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kInvalidStateError,
                                                     kInvalidStateError, 0);
}

Errors::Error Errors::SyntaxError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kSyntaxError,
                                                     kSyntaxError, 0);
}

Errors::Error Errors::InvalidModificationError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(
      env, kInvalidModificationError, kInvalidModificationError, 0);
}

Errors::Error Errors::NamespaceError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kNamespaceError,
                                                     kNamespaceError, 0);
}

Errors::Error Errors::SecurityError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kSecurityError,
                                                     kSecurityError, 0);
}

Errors::Error Errors::NetworkError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kNetworkError,
                                                     kNetworkError, 0);
}

Errors::Error Errors::AbortError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kAbortError,
                                                     kAbortError, 0);
}

Errors::Error Errors::URLMismatchError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kURLMismatchError,
                                                     kURLMismatchError, 0);
}

Errors::Error Errors::QuotaExceededError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kQuotaExceededError,
                                                     kQuotaExceededError, 0);
}

Errors::Error Errors::TimeoutError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kTimeoutError,
                                                     kTimeoutError, 0);
}

Errors::Error Errors::InvalidNodeTypeError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kInvalidNodeTypeError,
                                                     kInvalidNodeTypeError, 0);
}

Errors::Error Errors::DataCloneError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kDataCloneError,
                                                     kDataCloneError, 0);
}

Errors::Error Errors::EncodingError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kEncodingError,
                                                     kEncodingError, 0);
}

Errors::Error Errors::NotReadableError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kNotReadableError,
                                                     kNotReadableError, 0);
}

Errors::Error Errors::UnknownError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kUnknownError,
                                                     kUnknownError, 0);
}

Errors::Error Errors::ConstraintError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kConstraintError,
                                                     kConstraintError, 0);
}

Errors::Error Errors::DataError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kDataError,
                                                     kDataError, 0);
}

Errors::Error Errors::TransactionInactiveError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(
      env, kTransactionInactiveError, kTransactionInactiveError, 0);
}

Errors::Error Errors::ReadOnlyError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kReadOnlyError,
                                                     kReadOnlyError, 0);
}

Errors::Error Errors::VersionError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kVersionError,
                                                     kVersionError, 0);
}

Errors::Error Errors::OperationError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kOperationError,
                                                     kOperationError, 0);
}

Errors::Error Errors::NotAllowedError(Napi::Env env) {
  return interop::DOMException::Create<DOMException>(env, kNotAllowedError,
                                                     kNotAllowedError, 0);
}

}  // namespace bindings
}  // namespace wgpu
