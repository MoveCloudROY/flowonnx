#ifndef FLOWONNX_SESSION_HPP
#define FLOWONNX_SESSION_HPP

#include <filesystem>
#include <memory>

#include <flowonnx/flowonnx_config.h>

namespace flowonnx {

// Session for Once Inference
class FLOWONNX_API Session {
public:
    Session();
    ~Session();

    Session(Session &&other) noexcept;
    Session &operator=(Session &&other) noexcept;

    bool open(const std::filesystem::path &path, std::string *errorMessage);
    bool close();

    std::filesystem::path path() const;

protected:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace flowonnx

#endif // FLOWONNX_SESSION_HPP
