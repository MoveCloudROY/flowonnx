#ifndef SESSION_H
#define SESSION_H

#include <filesystem>
#include <memory>

#include <flowonnx/global.h>

namespace flowonnx {

class FLOWONNX_EXPORT Session {
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

#endif // SESSION_H
