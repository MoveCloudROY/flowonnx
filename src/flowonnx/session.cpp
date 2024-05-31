#include "session.h"
#include "session_p.h"

namespace fs = std::filesystem;

namespace flowonnx {

    SessionSystem *SessionSystem::instance() {
        static SessionSystem _instance;
        return &_instance;
    }

    Session::Session() : _impl(std::make_unique<Impl>()) {
    }

    Session::~Session() = default;

    Session::Session(Session &&other) noexcept {
        std::swap(_impl, other._impl);
    }

    Session &Session::operator=(Session &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        std::swap(_impl, other._impl);
        return *this;
    }

    bool Session::open(const fs::path &path, std::string *errorMessage) {
        auto &impl = *_impl;

        fs::path canonicalPath;
        try {
            canonicalPath = fs::canonical(path);
        } catch (const std::exception &e) {
            *errorMessage = e.what();
            return false;
        }

        if (!fs::is_regular_file(canonicalPath)) {
            *errorMessage = "Not a regular file";
            return false;
        }

        auto mgr = SessionSystem::instance();
        auto it = mgr->sessionImageMap.find(canonicalPath);
        if (it == mgr->sessionImageMap.end()) {
            impl.image = new SessionImage(path);
        } else {
            impl.image = it->second;
            impl.image->ref();
        }

        return true;
    }

    bool Session::close() {
        auto &impl = *_impl;
        if (!impl.image)
            return false;

        if (impl.image->deref() == 0) {
            impl.image = nullptr;
        }
        return true;
    }

    fs::path Session::path() const {
        auto &impl = *_impl;
        return impl.image ? impl.image->path : fs::path();
    }

}