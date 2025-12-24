#include "ArcGraph.h"
#include <cassert>

ArcGraph::ArcGraph(int verticesCount) 
    : verticesCount_(verticesCount) {}

ArcGraph::ArcGraph(const IGraph& graph) 
    : verticesCount_(graph.VerticesCount()) {
    
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int next : nextVertices) {
            AddEdge(i, next);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < verticesCount_);
    assert(to >= 0 && to < verticesCount_);
    
    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return verticesCount_;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < verticesCount_);
    
    std::vector<int> result;
    for (const auto& edge : edges) {
        if (edge.first == vertex) {
            result.push_back(edge.second);
        }
    }
    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < verticesCount_);
    
    std::vector<int> result;
    for (const auto& edge : edges) {
        if (edge.second == vertex) {
            result.push_back(edge.first);
        }
    }
    return result;
}