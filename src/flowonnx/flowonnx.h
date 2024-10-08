#ifndef FLOWONNX_H
#define FLOWONNX_H

#include <cstdint>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "flowonnx/global.h"

// FLOWONNX_EXPORT bool initialize(const nlohmann::json::object_t &args, Error *error);

// FLOWONNX_EXPORT int64_t sessionCreate(std::filesystem::path &path, args, Error *error);
// FLOWONNX_EXPORT bool sessionDestroy(int64_t handle, Error *error);
// FLOWONNX_EXPORT bool sessionRunning(int64_t handle);

// FLOWONNX_EXPORT int64_t taskCreate();
// FLOWONNX_EXPORT void taskDestroy(int64_t handle);
// FLOWONNX_EXPORT bool taskStart(int64_t handle, nlohmann::json::value_t input, Error *error);
// FLOWONNX_EXPORT bool taskStop(int64_t handle, Error *error);
// FLOWONNX_EXPORT int taskState(int64_t handle);
// FLOWONNX_EXPORT bool taskResult(int64_t handle, JsonValue *result);

#endif // FLOWONNX_H