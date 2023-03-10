#include <stdio.h>
#include "WGraph.h"

void insertionSort(float inDegree[], int index[], int length) {
    for (int i = 1; i < length; i++) {
        float num = inDegree[i];
        int mid = index[i];
        int j = i-1;
        while (j >= 0 && inDegree[j]<num) {
            inDegree[j+1] = inDegree[j];
            index[j+1] = index[j];
            j--;
        }
        inDegree[j+1] = num;
        index[j+1] = mid;
    }
    printf("\nPopularity ranking:");
    for (int i = 0; i < length; ++i) {
        printf("\n%d %.1f",index[i],inDegree[i]);
    }
}

void popularityRank(Graph g) {
    int vertexNumber = numOfVertices(g);
    float inDegree[vertexNumber];
    float outDegree[vertexNumber];
    int index[vertexNumber];

    for (int i = 0; i < vertexNumber; ++i) {
        index[i] = i;
    }

    for (Vertex i = 0; i < vertexNumber; ++i) {
        outDegree[i] = 0;
        inDegree[i] = 0;
    }

    for (Vertex i = 0; i < vertexNumber; ++i) {
        for (Vertex j = 0; j < vertexNumber; ++j) {
            if (adjacent(g, i, j)) {
                outDegree[i]++;
                inDegree[j]++;
            }
        }
    }

    for (Vertex i = 0; i < vertexNumber; ++i) {
        if (!outDegree[i]){
            inDegree[i] *= 2;
        } else{
            inDegree[i] = inDegree[i] / outDegree[i];
        }
    }
    // sorted algorithm
    insertionSort(inDegree, index, vertexNumber);
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
            e.weight = 1;
            insertEdge(g, e);
        }
        printf("Done.\n");
        popularityRank(g);

        freeGraph(g);
    }
    return 0;
}
