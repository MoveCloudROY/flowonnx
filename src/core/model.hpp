#ifndef FLOWONNX_MODEL_HPP
#define FLOWONNX_MODEL_HPP

#define TBB_PREVIEW_CONCURRENT_LRU_CACHE 1
#include <tbb/concurrent_lru_cache.h>

namespace flowonnx::impl {
// 结构体用于存储端口信息
struct PortInfo {
    std::string name;
    std::string type;
    std::vector<int64_t> shape;
    std::vector<std::string> symbolic_dims;
};

// 结构体用于存储模型信息
struct ModelInfo {
    int64_t hash_code;
    std::string model_path;
    std::string graph_name;
    std::string producer_name;
    std::vector<PortInfo> inputs;
    std::vector<PortInfo> outputs;
};

class Model {

public:
    Model() = default;

    // 打印端口信息
    static void printPortInfo(const PortInfo &port, const std::string &prefix = "  ");

    // 打印模型信息
    static void printModelInfo(const ModelInfo &model_info);

    // 解析多个ONNX模型文件
    static std::vector<ModelInfo> parseMultipleOnnxModels(const std::vector<std::string> &model_paths);
};

} // namespace flowonnx::impl
#endif