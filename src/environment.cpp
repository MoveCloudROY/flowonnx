#include "environment.hpp"

#include <algorithm>

#include "utils/format.h"

namespace flowonnx::impl {

bool Environment::load(const fs::path &path, ExecutionProvider ep, std::string *error_msg) {
    LoadSO::Library tmp_lib;

    // 1. Load Ort shared library and create handle
#ifdef _WIN32
    auto orgLibPath = LoadSO::System::SetLibraryPath(path.parent_path());
#endif
    if (!tmp_lib.open(path, LoadSO::Library::ResolveAllSymbolsHint)) {
        *error_msg =
            utils::formatTextN("%1: Load library failed: %2", path, tmp_lib.lastError());
        return false;
    }
#ifdef _WIN32
    LoadSO::System::SetLibraryPath(orgLibPath);
#endif

    // 2. Get Ort API getter handle
    auto addr = tmp_lib.resolve("OrtGetApiBase");
    if (!addr) {
        *error_msg =
            utils::formatTextN("%1: Get api handle failed: %2", path, tmp_lib.lastError());
        return false;
    }

    // 3. Check Ort API
    auto handle = (OrtApiBase * (ORT_API_CALL *)()) addr;
    auto api_base = handle();
    auto api = api_base->GetApi(ORT_API_VERSION);
    if (!api) {
        *error_msg = utils::formatTextN("%1: Failed to get api instance");
        return false;
    }

    // Successfully get Ort API.
    Ort::InitApi(api);

    std::swap(m_lib, tmp_lib);

    m_loaded = true;
    m_ort_path = path;
    m_ep = ep;

    m_ort_api_base = api_base;
    m_ort_api = api;
    return true;
}

Environment *Environment::instance() {
    static Environment instance;
    return &instance;
}

bool Environment::isLoaded() const {
    return m_loaded;
}

fs::path Environment::runtimePath() const {
    return m_ort_path;
}

ExecutionProvider Environment::executionProvider() const {
    return m_ep;
}

std::string Environment::versionString() const {
    return m_ort_api_base ? m_ort_api_base->GetVersionString() : std::string();
}

} // namespace flowonnx::impl