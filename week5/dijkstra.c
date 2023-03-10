#include <stdio.h>
#include <stdlib.h>
#include "PQueue.h"

#define VERY_HIGH_VALUE 999999

void output(int src, int cur, int pred[]) {
    if (src!=cur) {
        output(src, pred[cur], pred);
        printf("-%d", cur);
    } else {
        printf("%d", cur);
    }
}

bool allVisited(int *vSet, int nV) {
    for (int i = 0; i< nV;i++) {
        if (vSet[i]) {
            return false;
        }
    }
    return true;
}

void dijkstraSSSP(Graph g, Vertex source) {
    int dist[MAX_NODES];
    int pred[MAX_NODES];
    int vSet[MAX_NODES];


    PQueueInit();
    int nV = numOfVertices(g);
    for (int i = 0; i < nV; ++i) {
        joinPQueue(i);
        dist[i] = VERY_HIGH_VALUE;
        pred[i] = -1;
        vSet[i] = true;
    }
    dist[source] = 0;

    while (!allVisited(vSet, nV)) {
        int u;
        u = leavePQueue(dist);
        for (int v = 0; v < nV; ++v) {
            int w = adjacent(g, u, v);
            if (w > 0 && vSet[v]  && dist[v] > dist[u] + w  ) {
                pred[v] = u;
                dist[v] = dist[u] + w;
            }
        }
        vSet[u] = false;
    }

    for (int i = 0; i < nV; i++) {
        if (dist[i] == VERY_HIGH_VALUE) {
            printf("\n%d: no path", i);
        } else {
            printf("\n%d: distance = %d, shortest path: ", i, dist[i]);
            output(source, i, pred);
        }
    }
}


int main(void) {

    int nV, src;
    printf("Enter the number of vertices: ");
    scanf("%d", &nV);
    printf("Enter the source node: ");
    scanf("%d", &src);
    Graph g = newGraph(nV);
    Edge e1;
    Edge e2;

    while (true) {
        printf("Enter an edge (from): ");
        if (scanf("%d", &e1.v)){
            e2.w = e1.v;
        } else{
            break;
        }
        printf("Enter an edge (to): ");
        if (scanf("%d", &e1.w)){
            e2.v = e1.w;
        } else{
            break;
        }
        printf("Enter the weight: ");
        if (scanf("%d", &e1.weight)){
            e2.weight = e1.weight;
        } else{
            break;
        }
        insertEdge(g, e1);
        insertEdge(g, e2);
    }
    printf("Done.");
    dijkstraSSSP(g, src);
    freeGraph(g);
    return 0;
}