#include <iostream>
#include <queue>
#include <omp.h>

using namespace std;

const int MAX_NODES = 7;  // Maximum nodes
const int NUM_THREADS = 4; // Number of threads

void parallel_BFS(int graph[MAX_NODES][MAX_NODES], int n, int start) {
    bool visited[MAX_NODES] = {false};  // Visited array
    queue<int> frontier;  // BFS queue

    visited[start] = true;
    frontier.push(start);

    cout << "BFS Order: ";

    while (!frontier.empty()) {
        int frontier_size = frontier.size();
        int next_frontier[MAX_NODES] = {0}; // Stores next level nodes
        int next_frontier_count = 0;

        #pragma omp parallel num_threads(NUM_THREADS)
        {
            int local_frontier[MAX_NODES];
            int local_count = 0;

            #pragma omp for
            for (int i = 0; i < frontier_size; i++) {
                int node;

                #pragma omp critical
                {
                    node = frontier.front();
                    frontier.pop();
                }

                cout << node << " ";

                for (int j = 0; j < n; j++) {
                    if (graph[node][j] && !visited[j]) {
                        visited[j] = true;
                        local_frontier[local_count++] = j;
                    }
                }
            }

            #pragma omp critical
            {
                for (int k = 0; k < local_count; k++) {
                    next_frontier[next_frontier_count++] = local_frontier[k];
                }
            }
        }

        for (int i = 0; i < next_frontier_count; i++) {
            frontier.push(next_frontier[i]);
        }
    }

    cout << endl;
}

int main() {
    int graph[MAX_NODES][MAX_NODES] = {
        {0, 1, 1, 0, 0, 0, 0},
        {1, 0, 0, 1, 1, 0, 0},
        {1, 0, 0, 0, 0, 1, 1},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0}
    };

    int start_node = 0;
    parallel_BFS(graph, MAX_NODES, start_node);

    return 0;
}
BFS Order: 0 1 2 3 4 5 6 





#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

void BFS(int start, vector<vector<int>>& adj, vector<bool>& visited) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " "; 

        // Parallelize the exploration of neighbors
        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {
            int neighbor = adj[node][i];
            if (!visited[neighbor]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
}

int main() {
    int vertices, edges;
    cout << "Enter number of vertices: ";
    cin >> vertices;
    cout << "Enter number of edges: ";
    cin >> edges;

    vector<vector<int>> adj(vertices);
    vector<bool> visited(vertices, false); 

    cout << "Enter the edges (u v) where u and v are connected:\n";
    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u); 
    }

    int startNode;
    cout << "Enter the starting node: ";
    cin >> startNode;

    cout << "BFS Traversal: ";
    BFS(startNode, adj, visited);
    cout << endl;

    return 0;
}

Enter number of vertices: 5
Enter number of edges: 4
Enter the edges (u v) where u and v are connected:
0 1
0 2
1 3
3 4
Enter the starting node: 0
BFS Traversal: 0 1 2 3 4 

    
