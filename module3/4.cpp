#include <iostream>

static const int N = 4;
static const int MAXD = 80;

int goal[16] = {
    1, 2, 3, 4,
    5, 6, 7, 8,
    9,10,11,12,
    13,14,15,0
};

int dx[4] = {0, 0, -1, 1};
int dy[4] = {-1, 1, 0, 0};
char mv[4] = {'R', 'L', 'D', 'U'};
int opp[4] = {1, 0, 3, 2};

int board[16];
int pos0;
std::string path;

int manhattan() {
    int dist = 0;
    for (int i = 0; i < 16; i++) {
        if (board[i] == 0) continue;
        int v = board[i] - 1;
        dist += abs(i / 4 - v / 4) + abs(i % 4 - v % 4);
    }
    return dist;
}

bool dfs(int g, int bound, int prev) {
    int h = manhattan();
    if (g + h > bound) return false;
    if (h == 0) return true;

    int x = pos0 / 4;
    int y = pos0 % 4;

    for (int d = 0; d < 4; d++) {
        if (d == prev) continue;
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx < 0 || ny < 0 || nx >= 4 || ny >= 4) continue;

        int np = nx * 4 + ny;
        std::swap(board[pos0], board[np]);
        int old = pos0;
        pos0 = np;

        path.push_back(mv[d]);
        if (dfs(g + 1, bound, opp[d])) return true;
        path.pop_back();

        pos0 = old;
        std::swap(board[pos0], board[np]);
    }
    return false;
}

bool solvable() {
    int inv = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = i + 1; j < 16; j++) {
            if (board[i] && board[j] && board[i] > board[j]) {
                inv++;
            }
        }
    }

    int rowFromTop = pos0 / 4;
    int rowFromBottom = 4 - rowFromTop;

    if (rowFromBottom % 2 == 0) {
        return inv % 2 == 1;
    } else {
        return inv % 2 == 0;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    for (int i = 0; i < 16; i++) {
        std::cin >> board[i];
        if (board[i] == 0) pos0 = i;
    }

    if (!solvable()) {
        std::cout << -1 << "\n";
        return 0;
    }

    int bound = manhattan();
    for (; bound <= MAXD; bound++) {
        path.clear();
        if (dfs(0, bound, -1)) {
            std::cout << path.size() << "\n";
            std::cout << path << "\n";
            return 0;
        }
    }

    std::cout << -1 << "\n";
    return 0;
}
