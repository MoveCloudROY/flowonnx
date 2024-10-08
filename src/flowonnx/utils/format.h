#ifndef FLOWONNX_FORMAT_H
#define FLOWONNX_FORMAT_H

#include <filesystem>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include <flowonnx/global.h>

namespace flowonnx {

template <class T>
std::string anyToString(T &&t) {
    using T2 = std::remove_cv_t<std::remove_reference_t<T>>;
    if constexpr (std::is_same_v<T2, bool>) {
        return t ? "true" : "false";
    } else if constexpr (std::is_integral_v<T2>) {
        return std::to_string(t);
    } else if constexpr (std::is_floating_point_v<T2>) {
        std::ostringstream oss;
        oss << std::noshowpoint << t;
        return oss.str();
    } else if constexpr (std::is_same_v<T2, std::filesystem::path>) {
#ifdef _WIN32
        return wide2utf8(t.wstring());
#else
        return t.string();
#endif
    }
#ifdef _WIN32
    else if constexpr (std::is_same_v<T2, std::wstring>) {
        return wide2utf8(t);
    }
#endif
    else {
        return std::string(t);
    }
}

#ifdef _WIN32
FLOWONNX_EXPORT std::string wide2utf8(const std::wstring &s);
#endif

FLOWONNX_EXPORT std::string formatText(const std::string &format, const std::vector<std::string> &args);

template <typename... Args>
auto formatTextN(const std::string &format, Args &&...args) {
    return formatText(format, {anyToString(std::forward<decltype(args)>(args))...});
}
} // namespace flowonnx

#endif // FLOWONNX_FORMAT_H
