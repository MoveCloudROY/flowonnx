#ifndef DS_TYPE_HPP
#define DS_TYPE_HPP

#include <onnxruntime_cxx_api.h>

namespace flowonnx {

template <typename T>
struct DataObject {
};

struct OrtValue : public DataObject<Ort::Value> {
};

} // namespace flowonnx

#endif