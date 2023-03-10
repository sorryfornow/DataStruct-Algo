#include <stdio.h>
#include "Graph.h"

#define MAX_NODES 1000

int visited[MAX_NODES];  // array to store visiting order
// indexed by vertex 0..nV-1

bool dfsCycleCheck(Graph graph, Vertex v, Vertex u) {
    // mark v as visited
    visited[v] = 1;
    for (Vertex w = 0; w < numOfVertices(graph); w++) {
        // for each (v,w)∈edges(G) do
        if (adjacent(graph, v, w)) {
            // if w has been visited then
            if (visited[w] && w != u) {
                return true;
            }
            // else if dfsCycleCheck(G,w) then
            if (!visited[w] && dfsCycleCheck(graph, w, v)) {
                return true;
            }
        }
    }
    return false;
}

bool hasCycle(Graph graph) {
    for (int i = 0; i < numOfVertices(graph); ++i) {
        visited[i] = 0; // initialization
    }
    // begin with the unvisited vertex
    for (Vertex v = 0; v < numOfVertices(graph); ++v) {
        if (!visited[v] && dfsCycleCheck(graph, v, v)) {
            return true;
        }
    }
    return false;
}


int main() {
    int vertexNumber = 0;
    printf("Enter the number of vertices: ");
    if (scanf("%d", &vertexNumber)){
        Graph g = newGraph(vertexNumber);
        Edge e;
        int v = -1, w = -1;
        while (1) {
            printf("Enter an edge (from): ");
            if (!scanf("%d", &v)) {
                break;
            }
            printf("Enter an edge (to): ");
            if (!scanf("%d", &w)) {
                break;
            }
            e.v = v;
            e.w = w;
            insertEdge(g, e);
        }
        printf("Done.\n");
        if (hasCycle(g)) {
            printf("The graph has a cycle.\n");
        } else {
            printf("The graph is acyclic.\n");
        }
        freeGraph(g);
    }
    return 0;
}