#include <iomanip>
#include <iostream>
#include <onnxruntime_cxx_api.h>
#include <string>
using namespace Ort;

// Utility function to display tensor element type
std::string getTensorElementTypeName(ONNXTensorElementDataType type) {
    switch (type) {
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT:
        return "float";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8:
        return "uint8_t";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8:
        return "int8_t";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT16:
        return "uint16_t";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16:
        return "int16_t";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32:
        return "int32_t";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64:
        return "int64_t";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING:
        return "string";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
        return "bool";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
        return "float16";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_DOUBLE:
        return "double";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT32:
        return "uint32_t";
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT64:
        return "uint64_t";
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

void printTensorInfo(const Ort::Session &session, bool isInput) {
    Ort::AllocatorWithDefaultOptions allocator;

    size_t count = isInput ? session.GetInputCount() : session.GetOutputCount();
    std::cout << (isInput ? "===== INPUT TENSORS =====" : "===== OUTPUT TENSORS =====") << std::endl;

    for (size_t i = 0; i < count; i++) {
        // Get name
        auto name = isInput ? session.GetInputNameAllocated(i, allocator) : session.GetOutputNameAllocated(i, allocator);

        // Get type information
        auto type_info = isInput ? session.GetInputTypeInfo(i) : session.GetOutputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();

        // Get tensor element type
        auto element_type = tensor_info.GetElementType();
        std::string type_name = getTensorElementTypeName(element_type);

        // Get tensor shape
        auto shape = tensor_info.GetShape();

        // Print information
        std::cout << "Tensor #" << i << std::endl;
        std::cout << "  Name: " << name.get() << std::endl;
        std::cout << "  Type: " << type_name << std::endl;
        std::cout << "  Shape: [";
        for (size_t j = 0; j < shape.size(); j++) {
            if (j > 0)
                std::cout << ", ";
            if (shape[j] == -1) {
                std::cout << "dynamic";
            } else {
                std::cout << shape[j];
            }
        }
        std::cout << "]" << std::endl;

        // Get symbolic dimensions if available
        size_t dim_count = tensor_info.GetDimensionsCount();
        std::vector<const char *> symbols(dim_count);
        tensor_info.GetSymbolicDimensions(symbols.data(), dim_count);
        if (dim_count > 0) {
            std::cout << "  Symbolic Dimensions: [";
            for (size_t j = 0; j < dim_count; j++) {
                if (j > 0)
                    std::cout << ", ";
                std::cout << symbols[j];
            }
            std::cout << "]" << std::endl;
        }

        std::cout << std::endl;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_model>" << std::endl;
        return 1;
    }

    const std::string model_path = argv[1];

    try {
        Ort::InitApi();
        // Create environment
        Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "ModelInfoExtraction");

        // Session options
        Ort::SessionOptions session_options;

        // Create session
        std::cout << "Loading model: " << model_path << std::endl;
        Ort::Session session(env, model_path.c_str(), session_options);

        // Print model metadata
        Ort::AllocatorWithDefaultOptions allocator;
        auto model_metadata = session.GetModelMetadata();

        // const char *model_name = model_metadata.GetGraphNameAllocated(allocator).get();
        std::cout << "Graph Name: " << model_metadata.GetGraphNameAllocated(allocator).get() << std::endl;
        std::cout << "Provider: " << model_metadata.GetProducerNameAllocated(allocator).get() << std::endl;

        // Print input tensors info
        printTensorInfo(session, true);

        // Print output tensors info
        printTensorInfo(session, false);

    } catch (const Ort::Exception &e) {
        std::cerr << "ONNX Runtime error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
