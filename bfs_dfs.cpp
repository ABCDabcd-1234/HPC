#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // ✅ Correct Parallel BFS (Level-wise)
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        vector<int> current, next;

        visited[start] = true;
        current.push_back(start);

        while (!current.empty()) {

            #pragma omp parallel for
            for (int i = 0; i < current.size(); i++) {
                int node = current[i];

                #pragma omp critical
                cout << node << " ";

                for (int neighbour : adj[node]) {
                    if (!visited[neighbour]) {

                        #pragma omp critical
                        {
                            if (!visited[neighbour]) {
                                visited[neighbour] = true;
                                next.push_back(neighbour);
                            }
                        }
                    }
                }
            }

            current = next;
            next.clear();
        }
    }

    // ✅ DFS (Sequential - safest and correct)
    void dfsUtil(int node, vector<bool> &visited) {
        visited[node] = true;
        cout << node << " ";

        for (int neighbour : adj[node]) {
            if (!visited[neighbour]) {
                dfsUtil(neighbour, visited);
            }
        }
    }

    void DFS(int start) {
        vector<bool> visited(V, false);
        dfsUtil(start, visited);
    }
};

int main() {
    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    // -------- BFS Timing --------
    auto start_bfs = high_resolution_clock::now();

    cout << "Parallel BFS: ";
    g.parallelBFS(0);

    auto end_bfs = high_resolution_clock::now();
    auto duration_bfs = duration_cast<microseconds>(end_bfs - start_bfs);

    cout << "\nTime BFS: " << duration_bfs.count() << " microseconds\n\n";

    // -------- DFS Timing --------
    auto start_dfs = high_resolution_clock::now();

    cout << "DFS: ";
    g.DFS(0);

    auto end_dfs = high_resolution_clock::now();
    auto duration_dfs = duration_cast<microseconds>(end_dfs - start_dfs);

    cout << "\nTime DFS: " << duration_dfs.count() << " microseconds\n";

    return 0;
}