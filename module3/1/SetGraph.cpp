#include "SetGraph.h"
#include <cassert>

SetGraph::SetGraph(int verticesCount) 
    : adjacencySets(verticesCount)
    , reverseAdjacencySets(verticesCount) {}

SetGraph::SetGraph(const IGraph& graph) 
    : adjacencySets(graph.VerticesCount())
    , reverseAdjacencySets(graph.VerticesCount()) {
    
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int next : nextVertices) {
            AddEdge(i, next);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjacencySets.size());
    assert(to >= 0 && to < adjacencySets.size());
    
    adjacencySets[from].insert(to);
    reverseAdjacencySets[to].insert(from);
}

int SetGraph::VerticesCount() const {
    return static_cast<int>(adjacencySets.size());
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacencySets.size());
    
    std::vector<int> result;
    for (int next : adjacencySets[vertex]) {
        result.push_back(next);
    }
    return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < reverseAdjacencySets.size());
    
    std::vector<int> result;
    for (int prev : reverseAdjacencySets[vertex]) {
        result.push_back(prev);
    }
    return result;
}