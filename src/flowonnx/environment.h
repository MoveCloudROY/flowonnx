#ifndef FLOWONNX_ENVIRONMENT_H
#define FLOWONNX_ENVIRONMENT_H

#include <map>
#include <filesystem>

#include <flowonnx/flowonnxglobal.h>
#include <flowonnx/flowonnx_common.h>

#define ortEnv (flowonnx::Environment::instance())

namespace flowonnx {

    class FLOWONNX_EXPORT Environment {
    public:
        Environment();
        ~Environment();

        static Environment *instance();

    public:
        bool load(const std::filesystem::path &path, ExecutionProvider ep,
                  std::string *errorMessage);
        bool isLoaded() const;

        std::filesystem::path runtimePath() const;
        ExecutionProvider executionProvider() const;
        std::string versionString() const;

    protected:
        class Impl;
        std::unique_ptr<Impl> _impl;
    };

}

#endif // FLOWONNX_ENVIRONMENT_H
