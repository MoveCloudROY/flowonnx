#ifndef DS_FLOWONNX_H
#define DS_FLOWONNX_H

#include <nlohmann/json.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "flowonnx/flowonnx_common.h"
#include "flowonnx/flowonnx_config.h"

FLOWONNX_API flow_status_t initialize(nlohmann::json::object_t const *args);

FLOWONNX_API flow_status_t sessionCreate(const char *path, nlohmann::json::value_t const *args, int64_t *session);
FLOWONNX_API flow_status_t sessionDestroy(int64_t handle);
FLOWONNX_API flow_status_t sessionRunning(int64_t handle);

FLOWONNX_API flow_status_t taskCreate(int64_t *task);
FLOWONNX_API flow_status_t taskDestroy(int64_t handle);
FLOWONNX_API flow_status_t taskStart(int64_t handle, nlohmann::json::value_t const *input);
FLOWONNX_API flow_status_t taskStop(int64_t handle);
FLOWONNX_API flow_status_t taskState(int64_t handle);
FLOWONNX_API flow_status_t taskResult(int64_t handle, nlohmann::json::value_t const *result);

#ifdef __cplusplus
}
#endif

#endif // DS_FLOWONNX_H
