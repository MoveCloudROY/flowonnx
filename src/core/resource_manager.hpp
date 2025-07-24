#ifndef FLOWONNX_RESOURCE_MANAGER_HPP
#define FLOWONNX_RESOURCE_MANAGER_HPP

#include <condition_variable>
#include <memory>
#include <mutex>

#include "flowonnx/core/log.hpp"

namespace flowonnx {
class ResourceManager {
private:
    static constexpr int MAX_RESOURCE = 50;
    int available_resources;
    std::mutex resource_mutex;
    std::condition_variable resource_cv;

public:
    ResourceManager()
        : available_resources(MAX_RESOURCE) {}

    // 请求资源
    bool acquire(int amount, const std::string &node_name, const std::string &task_name) {
        std::unique_lock<std::mutex> lock(resource_mutex);

        FLOWONNX_INFO("task [{}] require resource [{}] on [{}]: (available: [{}])", task_name, amount, node_name, available_resources);

        // 等待直到有足够的资源
        resource_cv.wait(lock, [this, amount]() {
            return available_resources >= amount;
        });

        available_resources -= amount;

        FLOWONNX_INFO("task [{}] acquire resource [{}] on [{}]: (available: [{}])", task_name, amount, node_name, available_resources);

        return true;
    }

    // 释放资源
    void release(int amount, const std::string &node_name, const std::string &task_name) {
        std::unique_lock<std::mutex> lock(resource_mutex);
        available_resources += amount;

        FLOWONNX_INFO("task [{}] release resource [{}] on [{}]: (available: [{}])", task_name, amount, node_name, available_resources);

        resource_cv.notify_all();
    }

    // 获取当前可用资源
    int get_available() {
        std::unique_lock<std::mutex> lock(resource_mutex);
        return available_resources;
    }
};
} // namespace flowonnx

#endif