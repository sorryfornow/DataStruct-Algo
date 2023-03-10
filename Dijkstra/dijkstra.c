// Starting code for Dijkstra's algorithm ... COMP9024 21T3

#include <stdio.h>
#include <stdbool.h>
#include "PQueue.h"

#define VERY_HIGH_VALUE 999999

void printPath(int src, int cur, int pred[]) {
    if (cur == src) {
        printf("%d", cur);
    } else {
        printPath(src, pred[cur], pred);
        printf("-%d", cur);
    }
}

bool checkVSet(bool *vSet, int nV) {
    for (int i = 0; i< nV;i++) {
        if (vSet[i] == true) {
            return false;
        }
    }
    return true;
}

void dijkstraSSSP(Graph g, Vertex source) {
    int dist[MAX_NODES];
    int pred[MAX_NODES];
    bool vSet[MAX_NODES];  // vSet[v] = true <=> v has not been processed
    int s, t;

    PQueueInit();
    int nV = numOfVertices(g);
    for (s = 0; s < nV; s++) {
        joinPQueue(s);
        dist[s] = VERY_HIGH_VALUE;
        pred[s] = -1;
        vSet[s] = true;
    }
    dist[source] = 0;

    /* NEEDS TO BE COMPLETED */
    while (checkVSet(vSet, nV) == false) {
    // while (PQueueIsEmpty() == false) {
        s = leavePQueue(dist);
        // 挨个判断
        for (t = 0; t < nV; t++) {
            int weight = adjacent(g, s, t);
            // 如果大于0 ，表示存在一条边
            // Relaxation
            if (weight > 0 && vSet[t] == true && dist[s] + weight < dist[t]) {
                dist[t] = dist[s] + weight;
                pred[t] = s;
            }
        }
        vSet[s] = false;
    }

    for (s = 0; s < nV; s++) {
        if (dist[s] == VERY_HIGH_VALUE) {
            printf("%d: no path\n", s);
        } else {
            printf("%d: distance = %d, shortest path: ", s, dist[s]);
            printPath(source, s, pred);
            printf("\n");
        }
    }
}

void reverseEdge(Edge *e) {
    Vertex temp = e->v;
    e->v = e->w;
    e->w = temp;
}

int main(void) {
    Edge e;
    int n, source;

    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    Graph g = newGraph(n);

    printf("Enter the source node: ");
    scanf("%d", &source);
    printf("Enter an edge (from): ");
    while (scanf("%d", &e.v) == 1) {
        printf("Enter an edge (to): ");
        scanf("%d", &e.w);
        printf("Enter the weight: ");
        scanf("%d", &e.weight);
        insertEdge(g, e);
        reverseEdge(&e);               // ensure to add edge in both directions
        insertEdge(g, e);
        printf("Enter an edge (from): ");
    }
    printf("Done.\n");

    dijkstraSSSP(g, source);
    freeGraph(g);
    return 0;
}
