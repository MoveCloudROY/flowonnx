#ifndef DS_NODE_HPP
#define DS_NODE_HPP

#include <onnxruntime_cxx_api.h>

#include <flowonnx/flowonnx_config.h>

#include "core/model.hpp"

namespace flowonnx::impl {

class FLOWONNX_API Node {

public:
    virtual void PreProcess() {}
    virtual void PostProcess() {}
    virtual void Reset() {}
    virtual void Run() = 0;

protected:
    std::unordered_map<std::string, PortInfo> inputs;
    std::unordered_map<std::string, PortInfo> outputs;
    std::unordered_map<std::string, PortInfo> inputs_from;
    std::unordered_map<std::string, PortInfo> outputs_to;
    // tbb::flow::function_node<Ort::Value, Ort::Value> node;
};

} // namespace flowonnx::impl

#endif