#ifndef FLOWONNX_ENVIRONMENT_H
#define FLOWONNX_ENVIRONMENT_H

#include <filesystem>
#include <memory>

#include <flowonnx/common.h>
#include <flowonnx/global.h>

namespace flowonnx {

class FLOWONNX_EXPORT Environment {
public:
    Environment();
    ~Environment();

    static Environment *instance();

public:
    bool load(const std::filesystem::path &path, ExecutionProvider ep, std::string *errorMessage);
    bool isLoaded() const;

    std::filesystem::path runtimePath() const;
    ExecutionProvider executionProvider() const;
    std::string versionString() const;

protected:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace flowonnx

#endif // FLOWONNX_ENVIRONMENT_H
