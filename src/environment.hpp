#ifndef FLOWONNX_ENVIRONMENT_H
#define FLOWONNX_ENVIRONMENT_H

#include <filesystem>
#include <memory>

#include <loadso/library.h>
#include <loadso/system.h>
#include <onnxruntime_cxx_api.h>

#include <flowonnx/flowonnx_common.h>
#include <flowonnx/flowonnx_config.h>

#include "core/common.hpp"

namespace flowonnx::impl {

namespace fs = std::filesystem;
class FLOWONNX_API Environment {

public:
    ~Environment() = default;

    static Environment *instance();

public:
    bool load(const fs::path &path, ExecutionProvider ep, std::string *errorMessage);
    bool isLoaded() const;

    fs::path runtimePath() const;
    ExecutionProvider executionProvider() const;
    std::string versionString() const;

protected:
    LoadSO::Library m_lib;

    // Metadata
    bool m_loaded = false;
    fs::path m_ort_path;
    ExecutionProvider m_ep = EP_CPU;

    // Library data
    void *m_hLibrary = nullptr;
    const OrtApi *m_ort_api = nullptr;
    const OrtApiBase *m_ort_api_base = nullptr;

private:
    Environment() = default;
};

} // namespace flowonnx::impl

#endif // FLOWONNX_ENVIRONMENT_H
