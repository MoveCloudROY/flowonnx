#include "environment.h"

#include <algorithm>
#include <stdexcept>

#include <loadso/library.h>
#include <loadso/system.h>
#include <onnxruntime_cxx_api.h>

#include "utils/format.h"

namespace fs = std::filesystem;

namespace flowonnx {

static Environment *g_env = nullptr;

class Environment::Impl {
public:
    bool load(const fs::path &path, ExecutionProvider ep, std::string *errorMessage) {
        LoadSO::Library tempLib;

        // 1. Load Ort shared library and create handle
#ifdef _WIN32
        auto orgLibPath = LoadSO::System::SetLibraryPath(path.parent_path());
#endif
        if (!tempLib.open(path, LoadSO::Library::ResolveAllSymbolsHint)) {
            *errorMessage =
                formatTextN("%1: Load library failed: %2", path, tempLib.lastError());
            return false;
        }
#ifdef _WIN32
        LoadSO::System::SetLibraryPath(orgLibPath);
#endif

        // 2. Get Ort API getter handle
        auto addr = tempLib.resolve("OrtGetApiBase");
        if (!addr) {
            *errorMessage =
                formatTextN("%1: Get api handle failed: %2", path, tempLib.lastError());
            return false;
        }

        // 3. Check Ort API
        auto handle = (OrtApiBase * (ORT_API_CALL *)()) addr;
        auto apiBase = handle();
        auto api = apiBase->GetApi(ORT_API_VERSION);
        if (!api) {
            *errorMessage = formatTextN("%1: Failed to get api instance");
            return false;
        }

        // Successfully get Ort API.
        Ort::InitApi(api);

        std::swap(lib, tempLib);

        loaded = true;
        ortPath = path;
        executionProvider = ep;

        ortApiBase = apiBase;
        ortApi = api;
        return true;
    }

    LoadSO::Library lib;

    // Metadata
    bool loaded = false;
    fs::path ortPath;
    ExecutionProvider executionProvider = EP_CPU;

    // Library data
    void *hLibrary = nullptr;
    const OrtApi *ortApi = nullptr;
    const OrtApiBase *ortApiBase = nullptr;
};

Environment::Environment()
    : _impl(std::make_unique<Impl>()) {
    g_env = this;
}

Environment::~Environment() {
    g_env = nullptr;
}

bool Environment::load(const fs::path &path, ExecutionProvider ep, std::string *errorMessage) {
    auto &impl = *_impl;
    if (impl.loaded) {
        *errorMessage = formatTextN("%1: Library \"%2\" has been loaded", path, impl.ortPath);
        return false;
    }
    return impl.load(path, ep, errorMessage);
}

bool Environment::isLoaded() const {
    auto &impl = *_impl;
    return impl.loaded;
}

Environment *Environment::instance() {
    return g_env;
}

fs::path Environment::runtimePath() const {
    auto &impl = *_impl;
    return impl.ortPath;
}

ExecutionProvider Environment::executionProvider() const {
    auto &impl = *_impl;
    return impl.executionProvider;
}

std::string Environment::versionString() const {
    auto &impl = *_impl;
    return impl.ortApiBase ? impl.ortApiBase->GetVersionString() : std::string();
}

} // namespace flowonnx