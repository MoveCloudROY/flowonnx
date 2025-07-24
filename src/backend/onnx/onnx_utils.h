#ifndef FLOWONNX_ONNX_UTILS_H
#define FLOWONNX_ONNX_UTILS_H

#include "core/model.hpp"
#include <onnxruntime_cxx_api.h>
#include <string>

namespace flowonnx::impl::backend::onnx {

// // 解析单个ONNX模型文件
// static ModelInfo parseOnnxModel(const std::string &model_path);

// 将ONNXTensorElementDataType转换为字符串
std::string ONNXTypeToString(ONNXTensorElementDataType type);

} // namespace flowonnx::impl::backend::onnx

#endif // FLOWONNX_ONNX_UTILS_H