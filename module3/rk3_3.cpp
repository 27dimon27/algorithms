#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

struct Edge {
    int from, to, cost;
};

class Graph {
private:
    int N;
    std::vector<Edge> edges;

public:
    Graph(int n) : N(n) {}

    void addEdge(int from, int to, int cost) {
        edges.push_back({from, to, cost});
    }

    long long shortestPathWithKEdges(int start, int end, int K) {
        const long long INF = LLONG_MAX / 2;
        
        std::vector<long long> dist(N + 1, INF);
        dist[start] = 0;
        
        for (int i = 0; i < K; ++i) {
            std::vector<long long> newDist = dist;
            
            for (const Edge& e : edges) {
                if (dist[e.from] < INF) {
                    if (newDist[e.to] > dist[e.from] + e.cost) {
                        newDist[e.to] = dist[e.from] + e.cost;
                    }
                }
            }
            
            dist = move(newDist);
        }
        
        return (dist[end] == INF) ? -1 : dist[end];
    }
};

int main() {
    int N, M, K, S, F;
    std::cin >> N >> M >> K >> S >> F;
    
    Graph g(N);
    
    for (int i = 0; i < M; ++i) {
        int si, fi, pi;
        std::cin >> si >> fi >> pi;
        g.addEdge(si, fi, pi);
    }
    
    long long result = g.shortestPathWithKEdges(S, F, K);
    std::cout << result << std::endl;
    
    return 0;
}