#include <stdio.h>
#include "PQueue.h"

#define VERY_HIGH_VALUE 999999

void outputLink(int source, int current, int pred[]) {
    if (source==current) {
        printf("%d", current);
    } else {
        outputLink(source, pred[current], pred);
        printf("-%d", current);
    }
}

int visitedCheck(int *vSet, int numOfV) {
    for (int i = 0; i< numOfV;i++) {
        if (vSet[i]) {      // still have unvisited vertex
            return 1;       // continue while loop
        }
    }
    return 0;
}

void dijkstraSSSP(Graph g, Vertex source) {
    int dist[MAX_NODES];
    int pred[MAX_NODES];
    int vSet[MAX_NODES];
    // Vertex v has not been visited if vSet[v] = 1

    PQueueInit();
    int numOfV = numOfVertices(g);
    for (int i = 0; i < numOfV; ++i) {
        joinPQueue(i);
        dist[i] = VERY_HIGH_VALUE;
        pred[i] = -1;
        vSet[i] = 1;
    }
    dist[source] = 0;

    while (visitedCheck(vSet, numOfV)) {
        int i = leavePQueue(dist);
        for (int j = 0; j < numOfV; ++j) {
            int weight = adjacent(g, i, j);
            // an edge exist if weight > 0
            if (weight > 0 && vSet[j]  && dist[i] + weight < dist[j]) {
                dist[j] = dist[i] + weight;
                pred[j] = i;
            }
        }
        vSet[i] = 0;
    }

    for (int i = 0; i < numOfV; i++) {
        if (dist[i] == VERY_HIGH_VALUE) {
            printf("\n%d: no path", i);
        } else {
            printf("\n%d: distance = %d, shortest path: ", i, dist[i]);
            // output the path recursively
            outputLink(source, i, pred);
        }
    }
}


int main(void) {

    int n, source;
    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    printf("Enter the source node: ");
    scanf("%d", &source);
    Graph g = newGraph(n);
    Edge e;

    while (1) {
        printf("Enter an edge (from): ");
        if (!scanf("%d", &e.v)){
            break;
        }
        printf("Enter an edge (to): ");
        if (!scanf("%d", &e.w)){
            break;
        }
        printf("Enter the weight: ");
        if (!scanf("%d", &e.weight)){
            break;
        }
        insertEdge(g, e);
        Vertex temp = e.v;
        e.v = e.w;
        e.w = temp;
        insertEdge(g, e);
    }
    printf("Done.");
    dijkstraSSSP(g, source);

    freeGraph(g);
    return 0;
}
