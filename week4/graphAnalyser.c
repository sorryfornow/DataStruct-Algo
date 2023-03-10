
/** Algorithm & Complexity Analysis
 *  degreeAnalysis(Graph g):
 *      Input  Graph g with v vertices and e edges
 *      Output the minimum and maximum vertex degree
 *  -----------------------------------------------------------------
 *  Statement                                 # primitive operations
 *  -----------------------------------------------------------------
 *  vertexNumber = numOfVertices(g)           1
 *  for all i=0 up to vertexNumber-1 do       2v+1
 *      degree = 0                            1
 *      for all j=0 up to vertexNumber-1 do   v(2v+1)
 *          if adjacent(g, i, j) do           v^2
 *          degree++;                         v^2
 *          end if
 *      end for
 *      degreeOfVertex[i] = degree            1
 *  end for
 *
 *  maxDegree = degreeOfVertex[0]             1
 *  minDegree = degreeOfVertex[0]             1
 *  for all i=0 up to vertexNumber-1 do       2v+1
 *      if degreeOfVertex[i]>maxDegree do     v-1
 *          maxDegree = degreeOfVertex[i]     v-1
 *      end if
 *      else if degreeOfVertex[i]<minDegree do
 *          minDegree = degreeOfVertex[i]
 *      end else if
 *  end for
 *  print minimum degree                      1
 *  print maximum degree                      1
 *  -----------------------------------------------------------------
 *  Total: 4v^2+7v+7 which is O(v^2)
 *
 *  triangleDetect(Graph g):
 *      Input  Graph g with v vertices and e edges
 *      Output all triangle in Graph g
 *  -----------------------------------------------------------------
 *  Statement
 *  -----------------------------------------------------------------
 *  vN = numOfVertices(g)
 *  for all i=0 up to vN-3 do
 *      for all j=i+1 up to vN-2 do
 *          if adjacent(g, i, j) do
 *              for all k=j+1 up to vN-1 do
 *                  if adjacent(g, j, k) & adjacent(g, k, i) do
 *                      print i, j, k
 *  -----------------------------------------------------------------
 *  Total: v*(v-1)*(v-2)/6 which is O(v^3)
 **/

#include <stdio.h>
#include "Graph.h"

void degreeAnalysis(Graph g) {
    int vertexNumber = numOfVertices(g);
    int degreeOfVertex[vertexNumber];

    // type Vertex: int
    for (Vertex i = 0; i < vertexNumber; ++i) {
        int degree = 0;
        for (Vertex j = 0; j < vertexNumber; ++j) {
            if (adjacent(g, i, j)) {
                degree++;
            }
        }
        degreeOfVertex[i] = degree;
    }

    int maxDegree = degreeOfVertex[0];
    int minDegree = degreeOfVertex[0];

    for (Vertex i = 0; i < vertexNumber; ++i) {
        if (degreeOfVertex[i]>maxDegree){
            maxDegree = degreeOfVertex[i];
        }else if (degreeOfVertex[i]<minDegree) {
            minDegree = degreeOfVertex[i];
        }
    }
    printf("Minimum degree: %d\n"
           "Maximum degree: %d\n",minDegree,maxDegree);

    printf("Nodes of minimum degree:\n");
    for (Vertex i = 0; i < vertexNumber; ++i) {
        if (degreeOfVertex[i]==minDegree){
            printf("%d\n",i);
        }
    }

    printf("Nodes of maximum degree:\n");
    for (Vertex i = 0; i < vertexNumber; ++i) {
        if (degreeOfVertex[i]==maxDegree){
            printf("%d\n",i);
        }
    }
}

void triangleDetect(Graph g){
    int vertexNumber = numOfVertices(g);
    printf("Triangles:");
    for (Vertex i = 0; i < vertexNumber - 2; ++i) {
        for (Vertex j = i + 1; j < vertexNumber - 1; ++j) {
            if (adjacent(g, i, j)){
                for (Vertex k = j + 1; k < vertexNumber; ++k)
                    if (adjacent(g, j, k) && adjacent(g, k, i)) {
                        printf("\n%d-%d-%d", i, j, k);
                    }
            }
        }
    }
}

int main(){
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
        degreeAnalysis(g);
        triangleDetect(g);
        freeGraph(g);
    }
    return 0;
}
