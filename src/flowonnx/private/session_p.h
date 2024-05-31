#ifndef DSINFER_SESSION_P_H
#define DSINFER_SESSION_P_H

#include <map>
#include <utility>

#include <flowonnx/session.h>

namespace flowonnx {

    class SessionImage {
    public:
        inline explicit SessionImage(std::filesystem::path path);
        inline int ref();
        inline int deref();

        std::filesystem::path path;
        int count;
    };

    class SessionSystem {
    public:
        std::map<std::filesystem::path, SessionImage *> sessionImageMap;

        static SessionSystem *instance();
    };

    class Session::Impl {
    public:
        SessionImage *image = nullptr;
    };

    inline SessionImage::SessionImage(std::filesystem::path path)
        : path(std::move(path)), count(1) {
        SessionSystem::instance()->sessionImageMap[path] = this;
    }

    inline int SessionImage::ref() {
        count++;
        return count;
    }

    inline int SessionImage::deref() {
        count--;
        if (count == 0) {
            SessionSystem::instance()->sessionImageMap.erase(path);
            delete this;
            return 0;
        }
        return count;
    }

}

#endif // DSINFER_SESSION_P_H
