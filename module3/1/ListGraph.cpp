#include "ListGraph.h"
#include <cassert>

ListGraph::ListGraph(int verticesCount) 
    : adjacencyLists(verticesCount)
    , reverseAdjacencyLists(verticesCount) {}

ListGraph::ListGraph(const IGraph& graph) 
    : adjacencyLists(graph.VerticesCount())
    , reverseAdjacencyLists(graph.VerticesCount()) {
    
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int next : nextVertices) {
            AddEdge(i, next);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjacencyLists.size());
    assert(to >= 0 && to < adjacencyLists.size());
    
    adjacencyLists[from].push_back(to);
    reverseAdjacencyLists[to].push_back(from);
}

int ListGraph::VerticesCount() const {
    return static_cast<int>(adjacencyLists.size());
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacencyLists.size());
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < reverseAdjacencyLists.size());
    return reverseAdjacencyLists[vertex];
}