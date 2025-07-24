#ifndef FLOWONNX_LOG_HPP
#define FLOWONNX_LOG_HPP

#include <string>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace flowonnx {

class Log {
public:
    static void init(std::string const &logFilePath);

    static std::shared_ptr<spdlog::logger> &getCoreLogger() { return s_core_logger; }
    static void setLogLevel(spdlog::level::level_enum level) { s_core_logger->set_level(level); }

private:
    static inline std::shared_ptr<spdlog::logger> s_core_logger;
};

} // namespace flowonnx

// log macros
#define FLOWONNX_TRACE(...)    ::flowonnx::Log::getCoreLogger()->trace(__VA_ARGS__)
#define FLOWONNX_INFO(...)     ::flowonnx::Log::getCoreLogger()->info(__VA_ARGS__)
#define FLOWONNX_WARN(...)     ::flowonnx::Log::getCoreLogger()->warn(__VA_ARGS__)
#define FLOWONNX_ERROR(...)    ::flowonnx::Log::getCoreLogger()->error(__VA_ARGS__)
#define FLOWONNX_CRITICAL(...) ::flowonnx::Log::getCoreLogger()->critical(__VA_ARGS__)

#endif