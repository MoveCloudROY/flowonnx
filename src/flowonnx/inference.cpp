#include "inference.h"
#include "private/inference_p.h"

namespace flowonnx {

Inference::Inference()
    : _impl(std::make_unique<Impl>()) {
}

Inference::~Inference() = default;

Inference::Inference(Inference &&other) noexcept {
    std::swap(_impl, other._impl);
}

Inference &Inference::operator=(Inference &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    std::swap(_impl, other._impl);
    return *this;
}

} // namespace flowonnx