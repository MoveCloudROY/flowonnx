#ifndef DS_EXCUTION_GRAPH_HPP
#define DS_EXCUTION_GRAPH_HPP

#include <memory>

#include <nlohmann/json.hpp>
#include <tbb/flow_graph.h>

#include "core/node.hpp"

namespace flowonnx::impl {

class Graph {

public:
    explicit Graph() = default;

    static std::shared_ptr<Graph> fromJson(const nlohmann::json &json);
    static void makeEdge(Graph &g, std::shared_ptr<Node> s, std::shared_ptr<Node> &r);
    static void removeEdge(Graph &g, std::shared_ptr<Node> s, std::shared_ptr<Node> &r);

    const int id() const { return m_id; }

    void addNode(std::shared_ptr<Node> &node);
    void generateRunningGraph();

protected:
    std::unordered_map<std::string, std::shared_ptr<Node>> m_nodes;

    int m_id;
    tbb::flow::graph m_graph;
};

} // namespace flowonnx::impl

#endif