#ifndef INFERENCE_H
#define INFERENCE_H

#include <flowonnx/session.h>

namespace flowonnx {

class FLOWONNX_EXPORT Inference {
public:
    Inference();
    ~Inference();

    Inference(Inference &&other) noexcept;
    Inference &operator=(Inference &&other) noexcept;

public:
protected:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace flowonnx

#endif // INFERENCE_H
