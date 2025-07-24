#ifndef FLOWONNX_PORT_INFO_HPP
#define FLOWONNX_PORT_INFO_HPP

#include <string>

namespace flowonnx {
struct PortInfo {
    std::string m_node_name;
    std::string m_port_name;
};

} // namespace flowonnx

#endif