#ifndef FLOWONNX_COMMON_HPP
#define FLOWONNX_COMMON_HPP
#include "flowonnx/flowonnx_common_types.h"

namespace flowonnx::impl {

using status_t = flow_status_t;

namespace status {
constexpr status_t success = flow_success;
constexpr status_t invalid = flow_invalid;
constexpr status_t spec_load_failed = flow_spec_load_failed;
constexpr status_t spec_already_loaded = flow_spec_already_loaded;
constexpr status_t env_initialize_failed = flow_env_initialize_failed;
constexpr status_t env_already_initialized = flow_env_already_initialized;
constexpr status_t runtime_initialize_failed = flow_runtime_initialize_failed;
} // namespace status

using runtime_t = flow_runtime_t;
namespace runtime {
constexpr runtime_t cpu = flow_cpu;
constexpr runtime_t directml = flow_directml;
constexpr runtime_t cuda = flow_cuda;
constexpr runtime_t coreml = flow_coreml;
} // namespace runtime

} // namespace flowonnx::impl

#endif // FLOWONNX_COMMON_HPP