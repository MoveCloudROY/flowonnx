#ifndef DS_FLOWONNX_COMMON_TYPES_H
#define DS_FLOWONNX_COMMON_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    flow_success = 0,
    flow_invalid,
    flow_spec_load_failed,
    flow_spec_already_loaded,
    flow_env_initialize_failed,
    flow_env_already_initialized,
    flow_runtime_initialize_failed,
} flow_status_t;

typedef enum {
    flow_cpu = 0,
    flow_directml,
    flow_cuda,
    flow_coreml,
    flow_native,

} flow_runtime_t;

#ifdef __cplusplus
}
#endif

#endif // DS_FLOWONNX_COMMON_TYPES_H