#include <iostream>
#include <vector>
#include <algorithm>

class Graph {
private:
    int V;
    std::vector<std::vector<int>> adj;
    std::vector<int> degree;

    void dfs(int v, std::vector<bool>& visited) {
        visited[v] = true;
        for (int neighbor : adj[v]) {
            if (!visited[neighbor]) {
                dfs(neighbor, visited);
            }
        }
    }

    bool isConnected() {
        std::vector<bool> visited(V, false);
        
        int start = 0;
        for (int i = 0; i < V; i++) {
            if (!adj[i].empty()) {
                start = i;
                break;
            }
        }
        
        if (start == 0 && adj[0].empty()) {
            return true;
        }
        
        dfs(start, visited);
        
        for (int i = 0; i < V; i++) {
            if (!adj[i].empty() && !visited[i]) {
                return false;
            }
        }
        
        return true;
    }

public:
    Graph(int vertices) : V(vertices) {
        adj.resize(V);
        degree.resize(V, 0);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }

    bool isEulerian() {
        if (!isConnected()) {
            return false;
        }
        
        int oddCount = 0;
        for (int i = 0; i < V; i++) {
            if (degree[i] % 2 != 0) {
                oddCount++;
            }
        }
        
        return oddCount == 0;
    }
};

int main() {
    int N, M;
    std::cin >> N >> M;
    
    Graph graph(N);
    
    for (int i = 0; i < M; i++) {
        int u, v;
        std::cin >> u >> v;
        graph.addEdge(u, v);
    }
    
    std::cout << (graph.isEulerian() ? 1 : 0) << std::endl;
    
    return 0;
}