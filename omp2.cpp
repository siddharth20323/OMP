#include <iostream>
#include <climits>
#include <queue>
#include <omp.h>

using namespace std;

#define node 100  // Maximum number of nodes

int find_min_distance(int dist[], bool visited[], int n) {
    int min = INT_MAX, min_index = -1;

    for (int i = 0; i < n; ++i) {
        if (!visited[i] && dist[i] < min) {
            min = dist[i];
            min_index = i;
        }
    }

    return min_index; // Ensure this isn't -1 in the main function
}

void dijkstra(int graph[node][node], int src, int n) {
    int dist[node];   
    bool visited[node] = {false};  

    for (int i = 0; i < n; ++i) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;

    for (int c = 0; c < n - 1; c++) {
        int u = find_min_distance(dist, visited, n);
        if (u == -1) break; // Stop if no reachable node remains

        visited[u] = true;

        #pragma omp parallel for
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && graph[u][v] != INT_MAX && dist[u] != INT_MAX) {
                #pragma omp critical // Prevent race conditions in updating dist[v]
                {
                    if (dist[u] + graph[u][v] < dist[v]) {
                        dist[v] = dist[u] + graph[u][v];
                    }
                }
            }
        }
    }

    cout << "Shortest distances from node " << src << ":" << endl;
    for (int i = 0; i < n; ++i) {
        if (dist[i] == INT_MAX)
            cout << "Node " << i << " is unreachable." << endl;
        else
            cout << "Node " << i << ": " << dist[i] << endl;
    }
}

int main() {
    int n, m;
    cout << "Enter number of nodes and edges: ";
    cin >> n >> m;

    int graph[node][node];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            graph[i][j] = INT_MAX;
        }
        graph[i][i] = 0; // Ensure self-loops have 0 weight
    }

    cout << "Enter edges (u v w) for each edge (u, v) with weight w:" << endl;
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u][v] = w;
        graph[v][u] = w;  
    }

    int source;
    cout << "Enter source node: ";
    cin >> source;

    dijkstra(graph, source, n);

    return 0;
}


// Enter number of nodes and edges: 5 6
// Enter edges (u v w) for each edge (u, v) with weight w:
// 0 1 2
// 0 2 4
// 1 2 1
// 1 3 7
// 2 4 3
// 3 4 1
// Enter source node: 0
// Shortest distances from node 0:
// Node 0: 0
// Node 1: 2
// Node 2: 3
// Node 3: 7
// Node 4: 6
