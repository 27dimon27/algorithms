#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

// Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
// Найдите количество различных кратчайших путей между заданными вершинами.
// Требуемая сложность O(V+E). Граф должен быть реализован в виде класса.

class Graph {
public:
    explicit Graph(int vertices) : v(vertices), adj(vertices) {} // adj - вектор векторов для хранения списка смежности

    void addEdge(int from, int to) { // добавление неориентированного ребра между двумя вершинами
        adj[from].push_back(to); // to становится соседом from
        adj[to].push_back(from); // from становится соседом to
    }

    int countShortestPaths(int start, int target) { // подсчёт количества кратчайших путей от start до target
        const int INF = 1e9;
        std::vector<int> dist(v, INF);
        std::vector<int> pathCount(v, 0);
        std::queue<int> q;

        dist[start] = 0;
        pathCount[start] = 1;
        q.push(start);

        while (!q.empty()) { // цикл BFS
            int u = q.front();
            q.pop();

            for (int neighbor : adj[u]) {
                if (dist[neighbor] > dist[u] + 1) { // нашли более короткий путь
                    dist[neighbor] = dist[u] + 1;
                    pathCount[neighbor] = pathCount[u];
                    q.push(neighbor);
                } else if (dist[neighbor] == dist[u] + 1) { // нашли ещё один кратчайший путь такой же длины
                    pathCount[neighbor] += pathCount[u];
                }
            }
        }

        return pathCount[target]; // возвращаем количество кратчайших путей до целевой вершины
    }

private:
    int v; // кол-во вершин
    std::vector<std::vector<int>> adj; // список смежности
};

int main() {
    int vertices, edges;
    std::cin >> vertices >> edges; // считываем кол-во вершин и рёбер

    Graph g(vertices);

    for (int i = 0; i < edges; ++i) { // считываем и добавляем рёбра в граф
        int from, to;
        std::cin >> from >> to;
        g.addEdge(from, to);
    }

    int u, w;
    std::cin >> u >> w;

    std::cout << g.countShortestPaths(u, w) << std::endl;

    return 0;
}