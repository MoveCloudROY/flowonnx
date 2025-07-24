#include "core/graph.hpp"

namespace flowonnx::impl {

std::shared_ptr<Graph> Graph::fromJson(const nlohmann::json &json) {
    // Graph g;
    // for (const auto &node : json["nodes"]) {
    //     std::string nodeName = node["name"];
    //     std::shared_ptr<Node> newNode = std::make_shared<Node>();
    //     g.nodes[nodeName] = newNode;
    // }
    return std::make_shared<Graph>();
}

void Graph::makeEdge(Graph &g, std::shared_ptr<Node> s, std::shared_ptr<Node> &r) {
    // Implement the logic to create an edge between nodes s and r
}

} // namespace flowonnx::impl