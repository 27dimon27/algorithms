#include <iostream>
#include <vector>
#include <queue>

class Graph {
private:
    int vertices;
    std::vector<std::vector<int>> adjList;
    
public:
    Graph(int n) : vertices(n), adjList(n) {}
    
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
    
    bool isTree() {
        int edgeCount = 0;
        for (int i = 0; i < vertices; i++) {
            edgeCount += adjList[i].size();
        }
        edgeCount /= 2;
        
        if (edgeCount != vertices - 1) {
            return false;
        }
        
        std::vector<bool> visited(vertices, false);
        std::vector<int> parent(vertices, -1);
        
        int components = 0;
        
        for (int i = 0; i < vertices; i++) {
            if (!visited[i]) {
                components++;
                if (components > 1) {
                    return false;
                }
                
                if (!bfs(i, visited, parent)) {
                    return false;
                }
            }
        }
        
        return true;
    }
    
private:
    bool bfs(int start, std::vector<bool>& visited, std::vector<int>& parent) {
        std::queue<int> q;
        q.push(start);
        visited[start] = true;
        
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            
            for (int neighbor : adjList[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    q.push(neighbor);
                } else if (parent[current] != neighbor) {
                    return false;
                }
            }
        }
        
        return true;
    }
};

int main() {
    int n, m;
    std::cin >> n >> m;
    
    Graph graph(n);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        graph.addEdge(u, v);
    }
    
    if (graph.isTree()) {
        std::cout << 1 << std::endl;
    } else {
        std::cout << 0 << std::endl;
    }
    
    return 0;
}