#include "MatrixGraph.h"
#include <cassert>

MatrixGraph::MatrixGraph(int verticesCount) 
    : adjacencyMatrix(verticesCount, std::vector<bool>(verticesCount, false)) {}

MatrixGraph::MatrixGraph(const IGraph& graph) 
    : adjacencyMatrix(graph.VerticesCount(), std::vector<bool>(graph.VerticesCount(), false)) {
    
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int next : nextVertices) {
            AddEdge(i, next);
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjacencyMatrix.size());
    assert(to >= 0 && to < adjacencyMatrix.size());
    
    adjacencyMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return static_cast<int>(adjacencyMatrix.size());
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacencyMatrix.size());
    
    std::vector<int> result;
    for (int i = 0; i < adjacencyMatrix[vertex].size(); ++i) {
        if (adjacencyMatrix[vertex][i]) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacencyMatrix.size());
    
    std::vector<int> result;
    for (int i = 0; i < adjacencyMatrix.size(); ++i) {
        if (adjacencyMatrix[i][vertex]) {
            result.push_back(i);
        }
    }
    return result;
}