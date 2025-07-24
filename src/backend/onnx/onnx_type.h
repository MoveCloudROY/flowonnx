#ifndef FLOWONNX_ONNX_TYPE_H
#define FLOWONNX_ONNX_TYPE_H

#include <onnxruntime_cxx_api.h>
#include <string>
#include <vector>

namespace flowonnx::impl::backend::onnx {

enum class ErrorCode {
    invalid = -1,
    success = 0,
    onnx_runtime_load_failed,
    onnx_runtime_already_loaded,
    env_initialize_failed,
    env_already_initialized,
    runtime_initialize_failed,
};

} // namespace flowonnx::impl::backend::onnx

#endif // FLOWONNX_ONNX_TYPE_H