#include <iostream>
#include <vector>
using namespace std;

bool isSafe(int v, const vector<vector<int>>& graph, const vector<int>& color, int c) { // проверка на условие задачи, что соседние клетки не должны иметь один цвет
    for (int i = 0; i < graph.size(); ++i) {
        if (graph[v][i] && color[i] == c) {
            return false;
        }
    }
    return true;
}

bool graph_backtracking(const vector<vector<int>>& graph, int k, vector<int>& color, int v) { // рекурсивно вызывается bactracking(ну или возврат)
    if (v == graph.size()) {
        return true;
    }

    for (int c = 1; c <= k; ++c) {
        if (isSafe(v, graph, color, c)) {
            color[v] = c;
            if (graph_backtracking(graph, k, color, v + 1)) {
                return true;
            }
            color[v] = 0;
        }
    }

    return false;
}

bool graphColoring(const vector<vector<int>>& graph, int k, vector<int>& color) { // собственно расскраска
    return graph_backtracking(graph, k, color, 0);
}

int main() {
    int n, k;
    cin >> n >> k;

    vector<vector<int>> graph(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            char c;
            cin >> c;
            graph[i][j] = c - '0';
        }
    }

    vector<int> color(n, 0);

    if (graphColoring(graph, k, color)) {
        cout << "YES" << endl;
        for (int c : color) {
            cout << c << " ";
        }
        cout << endl;
    }
    else {
        cout << "NO" << endl;
    }

    return 0;
}