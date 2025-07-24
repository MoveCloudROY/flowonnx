#include <iostream>
#include <onnxruntime_cxx_api.h>
#include <string>
#include <vector>

// 结构体用于存储端口信息
struct PortInfo {
    std::string name;
    std::string type;
    std::vector<int64_t> shape;
    std::vector<std::string> symbolic_dims;
};

// 结构体用于存储模型信息
struct ModelInfo {
    std::string model_path;
    std::string graph_name;
    std::string producer_name;
    std::vector<PortInfo> inputs;
    std::vector<PortInfo> outputs;
};

// 将ONNXTensorElementDataType转换为字符串
std::string getTensorElementTypeName(ONNXTensorElementDataType type) {
    switch (type) {
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT:
        return "float32";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8:
        return "uint8";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8:
        return "int8";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT16:
        return "uint16";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16:
        return "int16";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32:
        return "int32";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64:
        return "int64";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING:
        return "string";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
        return "bool";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
        return "float16";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_DOUBLE:
        return "float64";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT32:
        return "uint32";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT64:
        return "uint64";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX64:
        return "complex64";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX128:
        return "complex128";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_BFLOAT16:
        return "bfloat16";
    default:
        return "unknown";
    }
}

// 解析单个ONNX模型文件
ModelInfo parseOnnxModel(const std::string &model_path) {
    ModelInfo model_info;
    model_info.model_path = model_path;

    try {
        // 初始化ONNX Runtime
        Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "OnnxModelParser");
        Ort::SessionOptions session_options;

        // 创建会话
        Ort::Session session(env, model_path.c_str(), session_options);
        Ort::AllocatorWithDefaultOptions allocator;

        // 获取模型元数据
        auto model_metadata = session.GetModelMetadata();
        model_info.graph_name = model_metadata.GetGraphNameAllocated(allocator).get();
        model_info.producer_name = model_metadata.GetProducerNameAllocated(allocator).get();

        // 解析输入端口
        size_t input_count = session.GetInputCount();
        for (size_t i = 0; i < input_count; i++) {
            PortInfo port;

            // 获取端口名称
            auto name = session.GetInputNameAllocated(i, allocator);
            port.name = name.get();

            // 获取类型信息
            auto type_info = session.GetInputTypeInfo(i);
            auto tensor_info = type_info.GetTensorTypeAndShapeInfo();

            // 获取数据类型
            auto element_type = tensor_info.GetElementType();
            port.type = getTensorElementTypeName(element_type);

            // 获取形状
            port.shape = tensor_info.GetShape();

            // 获取符号维度
            size_t dim_count = tensor_info.GetDimensionsCount();
            if (dim_count > 0) {
                std::vector<const char *> symbols(dim_count);
                tensor_info.GetSymbolicDimensions(symbols.data(), dim_count);
                for (size_t j = 0; j < dim_count; j++) {
                    if (symbols[j] != nullptr) {
                        port.symbolic_dims.push_back(symbols[j]);
                    } else {
                        port.symbolic_dims.push_back("");
                    }
                }
            }

            model_info.inputs.push_back(port);
        }

        // 解析输出端口
        size_t output_count = session.GetOutputCount();
        for (size_t i = 0; i < output_count; i++) {
            PortInfo port;

            // 获取端口名称
            auto name = session.GetOutputNameAllocated(i, allocator);
            port.name = name.get();

            // 获取类型信息
            auto type_info = session.GetOutputTypeInfo(i);
            auto tensor_info = type_info.GetTensorTypeAndShapeInfo();

            // 获取数据类型
            auto element_type = tensor_info.GetElementType();
            port.type = getTensorElementTypeName(element_type);

            // 获取形状
            port.shape = tensor_info.GetShape();

            // 获取符号维度
            size_t dim_count = tensor_info.GetDimensionsCount();
            if (dim_count > 0) {
                std::vector<const char *> symbols(dim_count);
                tensor_info.GetSymbolicDimensions(symbols.data(), dim_count);
                for (size_t j = 0; j < dim_count; j++) {
                    if (symbols[j] != nullptr) {
                        port.symbolic_dims.push_back(symbols[j]);
                    } else {
                        port.symbolic_dims.push_back("");
                    }
                }
            }

            model_info.outputs.push_back(port);
        }

    } catch (const Ort::Exception &e) {
        std::cerr << "ONNX Runtime error parsing " << model_path << ": " << e.what() << std::endl;
        throw;
    }

    return model_info;
}

// 打印端口信息
void printPortInfo(const PortInfo &port, const std::string &prefix = "  ") {
    std::cout << prefix << "Name: " << port.name << std::endl;
    std::cout << prefix << "Type: " << port.type << std::endl;
    std::cout << prefix << "Shape: [";
    for (size_t i = 0; i < port.shape.size(); i++) {
        if (i > 0)
            std::cout << ", ";
        if (port.shape[i] == -1) {
            std::cout << "dynamic";
        } else {
            std::cout << port.shape[i];
        }
    }
    std::cout << "]" << std::endl;

    if (!port.symbolic_dims.empty()) {
        std::cout << prefix << "Symbolic Dims: [";
        for (size_t i = 0; i < port.symbolic_dims.size(); i++) {
            if (i > 0)
                std::cout << ", ";
            std::cout << "\"" << port.symbolic_dims[i] << "\"";
        }
        std::cout << "]" << std::endl;
    }
}

// 打印模型信息
void printModelInfo(const ModelInfo &model_info) {
    std::cout << "=== ONNX Model Information ===" << std::endl;
    std::cout << "Model Path: " << model_info.model_path << std::endl;
    std::cout << "Graph Name: " << model_info.graph_name << std::endl;
    std::cout << "Producer: " << model_info.producer_name << std::endl;
    std::cout << std::endl;

    std::cout << "Input Ports (" << model_info.inputs.size() << "):" << std::endl;
    for (size_t i = 0; i < model_info.inputs.size(); i++) {
        std::cout << "  Port #" << i << ":" << std::endl;
        printPortInfo(model_info.inputs[i], "    ");
        std::cout << std::endl;
    }

    std::cout << "Output Ports (" << model_info.outputs.size() << "):" << std::endl;
    for (size_t i = 0; i < model_info.outputs.size(); i++) {
        std::cout << "  Port #" << i << ":" << std::endl;
        printPortInfo(model_info.outputs[i], "    ");
        std::cout << std::endl;
    }
}

// 解析多个ONNX模型文件
std::vector<ModelInfo> parseMultipleOnnxModels(const std::vector<std::string> &model_paths) {
    std::vector<ModelInfo> models_info;

    // 初始化ONNX Runtime API
    Ort::InitApi();

    for (const auto &path : model_paths) {
        std::cout << "Parsing model: " << path << std::endl;
        try {
            ModelInfo model_info = parseOnnxModel(path);
            models_info.push_back(model_info);
            std::cout << "✓ Successfully parsed: " << path << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "✗ Failed to parse: " << path << " - " << e.what() << std::endl;
        }
        std::cout << std::endl;
    }

    return models_info;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <onnx_model_path1> [onnx_model_path2] ..." << std::endl;
        std::cerr << "Example: " << argv[0] << " model1.onnx model2.onnx" << std::endl;
        return 1;
    }

    // 收集所有模型路径
    std::vector<std::string> model_paths;
    for (int i = 1; i < argc; i++) {
        model_paths.push_back(argv[i]);
    }

    try {
        // 解析所有模型
        auto models_info = parseMultipleOnnxModels(model_paths);

        // 输出结果
        std::cout << "===========================================" << std::endl;
        std::cout << "ONNX Models Analysis Summary" << std::endl;
        std::cout << "Total models processed: " << models_info.size() << std::endl;
        std::cout << "===========================================" << std::endl;
        std::cout << std::endl;

        // 打印每个模型的详细信息
        for (size_t i = 0; i < models_info.size(); i++) {
            std::cout << "Model #" << (i + 1) << std::endl;
            printModelInfo(models_info[i]);
            std::cout << "===========================================" << std::endl;
            std::cout << std::endl;
        }

        // 简要统计信息
        std::cout << "Summary Statistics:" << std::endl;
        for (const auto &model : models_info) {
            std::cout << "  " << model.model_path << ":" << std::endl;
            std::cout << "    Inputs: " << model.inputs.size() << ", Outputs: " << model.outputs.size() << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}