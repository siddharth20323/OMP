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
