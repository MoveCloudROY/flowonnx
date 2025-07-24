
#include <memory>
#include <string>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "core/log.hpp"

namespace flowonnx {

void Log::init(std::string const &logFilePath) {
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.c_str(), true));

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");

    s_core_logger = std::make_shared<spdlog::logger>("FlowONNX", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_core_logger);
    s_core_logger->set_level(spdlog::level::trace);
    s_core_logger->flush_on(spdlog::level::trace);
}

} // namespace flowonnx