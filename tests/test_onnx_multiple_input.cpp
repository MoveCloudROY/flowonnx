#include <iostream>
#include <memory>
#include <onnxruntime_cxx_api.h>
#include <random>
#include <vector>

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

static const char *const kOrtSessionOptionsUseDeviceAllocatorForInitializers = "session.use_device_allocator_for_initializers";

int main(int argc, char **argv) {
    // 设置默认模型路径，如果没有提供命令行参数
    std::string model_path = "../../zhibin-5.2-dsinfer/inferences/duration/linguistic.onnx";

    if (argc > 1) {
        model_path = argv[1];
    }

    std::cout << "Testing ONNX model with multiple inputs: " << model_path << std::endl;

    try {
        // 1. 初始化ONNX Runtime
        Ort::InitApi();
        Ort::Env env(ORT_LOGGING_LEVEL_INFO, "MultipleInputTest");

        // 2. 创建会话选项
        Ort::SessionOptions session_options;

        // session_options.AddConfigEntry(kOrtSessionOptionsUseDeviceAllocatorForInitializers, "1");
        // session_options.AddConfigEntry("memory.enable_memory_arena_shrinkage", "cpu:0;gpu:0");
        // 3. 加载模型
        std::cout
            << "Loading model..." << std::endl;
        Ort::Session session(env, model_path.c_str(), session_options);
        std::cout << "Model loaded successfully!" << std::endl;

        // 4. 获取输入信息
        Ort::AllocatorWithDefaultOptions allocator;
        size_t num_inputs = session.GetInputCount();

        std::cout << "Number of inputs: " << num_inputs << std::endl;

        // 存储输入名称和形状信息
        std::vector<std::string> input_names;
        std::vector<std::vector<int64_t>> input_shapes;
        std::vector<ONNXTensorElementDataType> input_types;

        // 获取每个输入的详细信息
        for (size_t i = 0; i < num_inputs; i++) {
            // 获取输入名称
            auto input_name = session.GetInputNameAllocated(i, allocator);
            input_names.push_back(input_name.get());

            // 获取输入类型信息
            auto type_info = session.GetInputTypeInfo(i);
            auto tensor_info = type_info.GetTensorTypeAndShapeInfo();

            // 获取形状
            auto shape = tensor_info.GetShape();
            input_shapes.push_back(shape);

            // 获取数据类型
            auto element_type = tensor_info.GetElementType();
            input_types.push_back(element_type);

            std::cout << "Input " << i << ": " << input_names[i] << " [";
            for (size_t j = 0; j < shape.size(); j++) {
                if (j > 0)
                    std::cout << ", ";
                if (shape[j] == -1) {
                    std::cout << "dynamic";
                } else {
                    std::cout << shape[j];
                }
            }
            std::cout << "] " << getTensorElementTypeName(element_type) << std::endl;
        }

        // 5. 准备输入数据
        std::cout << "\nPreparing input data..." << std::endl;

        std::vector<Ort::Value> input_tensors;
        std::vector<const char *> input_names_cstr;

        // 随机数生成器
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

        // 为每个输入创建张量
        for (size_t i = 0; i < num_inputs; i++) {
            auto &shape = input_shapes[i];

            // 处理动态维度，设置为合理的值
            std::vector<int64_t> actual_shape = shape;
            for (size_t j = 0; j < actual_shape.size(); j++) {
                if (actual_shape[j] == -1) {
                    // 根据位置设置合理的动态维度值
                    if (j == 0) {
                        actual_shape[j] = 1; // batch size
                    } else if (j == 1) {
                        actual_shape[j] = 10; // sequence length (较小的值)
                    } else {
                        actual_shape[j] = 1; // 其他维度
                    }
                }
            }

            // 计算元素总数
            size_t total_elements = 1;
            for (auto dim : actual_shape) {
                total_elements *= dim;
            }

            std::cout << "Creating tensor for input " << i << " with " << total_elements << " elements" << std::endl;

            // 根据数据类型创建数据
            if (input_types[i] == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT) {
                // 创建float数据
                std::vector<float> input_data(total_elements);
                for (size_t j = 0; j < total_elements; j++) {
                    input_data[j] = dis(gen);
                }

                // 创建张量
                auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
                auto tensor = Ort::Value::CreateTensor<float>(
                    memory_info,
                    input_data.data(),
                    total_elements,
                    actual_shape.data(),
                    actual_shape.size()
                );
                if (!tensor.IsTensor()) {
                    std::cerr << "Failed to create tensor for input " << i << std::endl;
                    return 1;
                }

                input_tensors.push_back(std::move(tensor));
            } else if (input_types[i] == ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64) {
                // 创建int64数据
                std::vector<int64_t> input_data(total_elements);
                for (size_t j = 0; j < total_elements; j++) {
                    input_data[j] = static_cast<int64_t>(dis(gen) * 100) % 10;
                }

                // 创建张量
                auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
                auto tensor = Ort::Value::CreateTensor<int64_t>(
                    memory_info,
                    input_data.data(),
                    total_elements,
                    actual_shape.data(),
                    actual_shape.size()
                );

                if (!tensor.IsTensor()) {
                    std::cerr << "Failed to create tensor for input " << i << std::endl;
                    return 1;
                }

                input_tensors.push_back(std::move(tensor));
            } else {
                std::cerr << "Unsupported input data type for input " << i << std::endl;
                return 1;
            }

            // 准备输入名称指针
            input_names_cstr.push_back(input_names[i].c_str());
        }

        // 6. 获取输出信息
        size_t num_outputs = session.GetOutputCount();
        std::vector<std::string> output_names;
        std::vector<const char *> output_names_cstr;

        for (size_t i = 0; i < num_outputs; i++) {
            auto output_name = session.GetOutputNameAllocated(i, allocator);
            output_names.push_back(output_name.get());
            output_names_cstr.push_back(output_names[i].c_str());
            std::cout << "Output " << i << ": " << output_names_cstr[i] << std::endl;
        }
        std::vector<const char *> output_node_names = {"encoder_out", "x_masks"};

        std::cout << "Number of outputs: " << num_outputs << std::endl;

        // 7. 执行推理
        std::cout << "\nRunning inference..." << std::endl;

        auto output_tensors = session.Run(
            Ort::RunOptions{nullptr},
            input_names_cstr.data(),
            input_tensors.data(),
            num_inputs,
            output_node_names.data(),
            num_outputs
        );

        std::cout << "Inference completed successfully!" << std::endl;
        std::cout << "Generated " << output_tensors.size() << " output tensors." << std::endl;

        // 8. 简单输出结果信息
        for (size_t i = 0; i < output_tensors.size(); i++) {
            auto tensor_info = output_tensors[i].GetTensorTypeAndShapeInfo();
            auto shape = tensor_info.GetShape();

            std::cout << "Output " << i << " shape: [";
            for (size_t j = 0; j < shape.size(); j++) {
                if (j > 0)
                    std::cout << ", ";
                std::cout << shape[j];
            }
            std::cout << "]" << std::endl;
        }

        std::cout << "\nMultiple input ONNX runtime test completed successfully!" << std::endl;

    } catch (const Ort::Exception &e) {
        std::cerr << "ONNX Runtime error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
