#include <iostream>
#include <vector>
#include <queue>
#include <climits>

// Требуется отыскать самый короткий маршрут между городами.
// Из города может выходить дорога, которая возвращается в этот же город.
// Требуемое время работы O((N+M)logN), где N – количество городов, M – известных дорог между ними.
// Граф должен быть реализован в виде класса.

class Graph {
public:
    explicit Graph(int vertices) : n(vertices), adj(vertices) {}

    void addEdge(int u, int v, int w) { // добавление ориентированного ребра от u к v с весом w
        adj[u].emplace_back(v, w);
    }

    int shortestPath(int start, int target) { // алгоритм Дейкстры для поиска кратчайшего пути от start до target
        std::vector<int> dist(n, INT_MAX);
        dist[start] = 0;

        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq; // храним пары (расстояние, вершина) и используем greater для порядка возрастания
        pq.emplace(0, start);

        while (!pq.empty()) {
            int currentDist = pq.top().first; // извлекаем вершину с наименьшим расстоянием
            int u = pq.top().second;
            pq.pop();

            if (currentDist > dist[u]) continue; // если извлечённое расстояние больше актуального, пропускаем эту вершину

            if (u == target) return currentDist; // если достигли целевой вершины, возвращаем расстояние до неё

            for (const auto& edge : adj[u]) { // обход всех соседей вершины u
                int v = edge.first;
                int weight = edge.second;

                if (dist[u] + weight < dist[v]) { // если найден более короткий путь до вершины v
                    dist[v] = dist[u] + weight;
                    pq.emplace(dist[v], v);
                }
            }
        }

        return dist[target];
    }

private:
    int n; // количество вершин
    std::vector<std::vector<std::pair<int, int>>> adj; // список смежности
};

int main() {
    int N, M;
    std::cin >> N >> M; // считываем кол-во вершин и рёбер

    Graph g(N);

    for (int i = 0; i < M; ++i) {
        int u, v, w;
        std::cin >> u >> v >> w;
        g.addEdge(u, v, w); // добавляем ребро в одну сторону
        g.addEdge(v, u, w); // добавляем ребро в другую сторону
    }

    int s, t;
    std::cin >> s >> t;

    std::cout << g.shortestPath(s, t) << std::endl;

    return 0;
}