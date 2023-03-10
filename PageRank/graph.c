//
// Created by Siqing Zhang on 2022/7/25.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graph.h"
#include "pagerank.h"
#include "dijkstra.h"

#define VERY_HIGH_VALUE 999999

// vertices are string
typedef string Vertex;

// edges are pairs of vertices (end-points) with a weight

typedef struct Graph_Node *GNode;
typedef struct Adjacency_List_Node *ALNode;
typedef int Index;  // index of the node

struct Adjacency_List_Node {
    GNode nodeW;    // vertex w
    size_t weight;  // weight in arc v->w
    ALNode nextALN; // next node in Adjacency List
    ALNode lastALN;
};

struct Graph_Node {
    Vertex v;       // vertex that node donate
    Index index;    // the index of the vertex
    GNode next;     // successive node in graph
    GNode lastN;    // preceding node in graph
    bool exist;     // where the node is ignored
    ALNode ALNHead; // store the node with weight
};

struct Graph_Repr {
    GNode nodeHead;    // array of lists
    int nV;     // #vertices
    int nE;     // #edges
    double *pageRankArray;  // pageRank
    int *dist;   // dijkstraSSSP
    Index *pred;   // dijkstraSSSP
    Index source;
};

// meta interface
/**
 * graph_create
 * allocate the required memory for a new graph
 * return a pointer to the new graph
 * return NULL on error
 */
graph graph_create (void) {
    graph newGraph;
    newGraph = malloc(sizeof (struct Graph_Repr));
    if (newGraph == NULL) {
        return NULL;
    }
    newGraph->nE = 0;
    newGraph->nV = 0;
    newGraph->nodeHead = NULL;
    newGraph->pageRankArray = NULL;
    newGraph->pred = NULL;
    newGraph->dist = NULL;
    newGraph->source = -1;
    return newGraph;
}

/**
 * graph_destroy
 * free all memory associated with a given graph
 */
void adjacencyDestroy(ALNode first) {
    // continue to free the successive nodes in the list
    if (first != NULL) {
        adjacencyDestroy(first->nextALN);
    }
    // deallocate the memory block of the current node
    if (first != NULL) {
        free(first);
    }
}

void nodeDestroy(GNode first) {
    // continue to free the successive nodes in the list
    if (first != NULL) {
        nodeDestroy(first->next);
    }
    // deallocate the memory block of the current node
    if (first != NULL) {
        free(first->v);
        adjacencyDestroy(first->ALNHead);
        free(first);
    }
}

void graph_destroy (graph G) {
    if (G != NULL) {
        if (G->nodeHead != NULL){
            nodeDestroy(G->nodeHead);
        }
        if (G->pageRankArray != NULL) {
            free(G->pageRankArray);
        }
        if (G->dist != NULL) {
            free(G->dist);
        }
        if (G->pred != NULL) {
            free(G->pred);
        }
        free(G);
    }
}

/**
 * graph_show
 * print the contents of the graph to the given file
 * If the given file is NULL, print to stdout
 * The graph should be printed in the following format:
 *      vertex1
 *      vertex2
 *      vertex3
 *      ...
 *      vertexN
 *      vertex1 vertex2 weight
 *      vertex2 vertex4 weight
 *      vertexN vertex4 weight
 * Where the name name of each vertex is first printed
 * Then the directed edges between each vertex along with the edge weight is printed
 *
 * An ignore list is also passed, indicating that certain URLs are required
 * to be excluded from the output. The each element in the list will
 * be a url string.
 */
void graph_show (graph G, FILE *file, list ignore){
    if (G != NULL) {
        // stdout print
        GNode NL;   // last node
        NL = G->nodeHead;
        while (NL != NULL && NL->next != NULL) {
            NL = NL->next;
        }
        GNode N1 = NL;
        // print out the vertices
        while (N1 != NULL) {
            if (!list_contains(ignore,N1->v)) {
                if (file == NULL) {
                    printf("%s\n", N1->v);
                } else {
                    fprintf(file,"%s\n", N1->v);
                }
            }
            N1 = N1->lastN;
        }

        // print out the adjacency pairs
        N1 = NL;
        // print out the vertices
        while (N1 != NULL) {
            if (!list_contains(ignore, N1->v)) {
                ALNode TNode = N1->ALNHead; // destination node
                // go to the last node
                while (TNode != NULL && TNode->nextALN != NULL) {
                    TNode = TNode->nextALN;
                }
                while (TNode != NULL) {
                    if (!list_contains(ignore, TNode->nodeW->v)) {
                        if (file == NULL) {
                            printf("%s %s %zu\n", N1->v, TNode->nodeW->v, TNode->weight);
                        } else {
                            fprintf(file, "%s %s %zu\n", N1->v, TNode->nodeW->v, TNode->weight);
                        }
                    }
                    TNode = TNode->lastALN;
                }
            }
            N1 = N1->lastN;
        }
    }
}

// vertex interface
/**
 * graph_add_vertex
 * Add a new vertex with a particular value to the graph
 * if a vertex with the same value already exists do not add a new vertex
 */
GNode nodeGenerate(string item, int numV) {
    GNode newNode = malloc(sizeof (struct Graph_Node));
    // to program execution in case failure to allocate memory
    if (newNode == NULL) {
        return NULL;
    }
    // initialize the created node
    newNode->ALNHead = NULL;
    newNode->exist = true;
    newNode->next = NULL;
    newNode->lastN = NULL;
    newNode->index = numV;  // first node with index 0
    // transcribe the data in the given string into node's value
    newNode->v = malloc(sizeof(char) * (strlen(item) + 1));
    strcpy(newNode->v, item);
    return newNode;
}

void graph_add_vertex (graph G, string vertex) {
    if (G != NULL && vertex != NULL) {
        GNode tail;
        tail = G->nodeHead;
        while (tail != NULL) {
            if (strcmp(tail->v, vertex) == 0) {
                break;  // vertex existed
            }
            tail = tail->next;
        }
        if (tail == NULL) { // vertex non-existed
            GNode newVertex;
            newVertex = nodeGenerate(vertex,G->nV);
            if (newVertex != NULL) {
                // add the newVertex in the front
                newVertex->next = G->nodeHead;
                G->nodeHead = newVertex;
                GNode secondN = G->nodeHead->next;
                if (secondN != NULL) {
                    secondN->lastN = newVertex;
                }
                // number of vertices add 1
                G->nV++;
            }
        }
    }
}

/**
 * graph_has_vertex
 * return True if a vertex with a particular value exists in the graph, False otherwise
 * return False on error
 */
bool graph_has_vertex (graph G, string vertex) {
    if (G != NULL && vertex != NULL) {
        GNode tail = G->nodeHead;
        while (tail != NULL) {
            if (strcmp(tail->v, vertex) == 0) {
                return true;  // vertex existed
            }
            tail = tail->next;
        }
    }
    return false;
}

/**
 * graph_vertices_count
 * return the number of vertices in the graph
 * return 0 on error
 */
size_t graph_vertices_count (graph G) {
    if (G != NULL) {
        return G->nV;
    }
    return 0;
}

// edge interface
/**
 * graph_add_edge
 * Add a new edge between two vertices with a weight to the graph
 * if a edge between two vertices already exists do not add a new edge
 */
ALNode arcCreate(size_t weightW, GNode W){
    ALNode arc = malloc(sizeof (struct Adjacency_List_Node));
    // execution in case failure to allocate memory
    if (arc == NULL) {
        return NULL;
    }
    // initialize the created node
    arc->nextALN = NULL;
    arc->lastALN = NULL;
    // transcribe the data in the given string into node's value
    arc->weight = weightW;
    // record target node
    arc->nodeW = W;
    return arc;
}

void graph_add_edge (graph G, string vertex1, string vertex2, size_t weight) {
    if (G != NULL) {
        GNode N1 = G->nodeHead;
        GNode N2 = G->nodeHead;
        // check out whether V1, V2 existed
        while (N1 != NULL) {
            if (strcmp(N1->v,vertex1) == 0) {
                break;
            }
            N1 = N1->next;
        }
        while (N2 != NULL) {
            if (strcmp(N2->v,vertex2) == 0) {
                break;
            }
            N2 = N2->next;
        }
        if (N1 != NULL && N2 != NULL) {
            // check out whether arc V1->V2 existed
            ALNode targetNode;
            targetNode = N1->ALNHead;
            while (targetNode != NULL) {
                if (targetNode->nodeW == N2) {
                    break;
                }
                targetNode = targetNode->nextALN;
            }
            if (targetNode == NULL) {
                // V1->V2 is not in V1's Adjacency List
                ALNode newALN;
                newALN = arcCreate(weight, N2);
                if (newALN != NULL) {
                    // add the newVertex in the front
                    newALN->nextALN = N1->ALNHead;
                    N1->ALNHead = newALN;
                    ALNode secondN = N1->ALNHead->nextALN;
                    if (secondN != NULL) {
                        secondN->lastALN = newALN;
                    }
                    // number of edge add 1
                    G->nE++;
                }
            }
        }
    }
}

/**
 * graph_has_edge
 * return True if a edge between two vertices exists in the graph, False otherwise
 * return False on error
 */
bool graph_has_edge (graph G, string vertex1, string vertex2) {
    if (G!=NULL) {
        GNode N1 = G->nodeHead;
        GNode N2 = G->nodeHead;
        while (N1 != NULL) {
            if (strcmp(N1->v,vertex1) == 0) {
                break;
            }
            N1 = N1->next;
        }
        while (N2 != NULL) {
            if (strcmp(N2->v,vertex2) == 0) {
                break;
            }
            N2 = N2->next;
        }
        if (N1 != NULL && N2 != NULL) {
            // check out whether arc V1->V2 existed
            // find out node of vertex2 in N1 adjacency list
            ALNode targetNode;
            targetNode = N1->ALNHead;
            while (targetNode != NULL) {
                if (strcmp(targetNode->nodeW->v,N2->v) == 0) {
                    return true;
                }
                targetNode = targetNode->nextALN;
            }
        }
    }
    return false;
}

/**
 * graph_set_edge
 * Update the weight of a edge between two vertices, if the edge doesn't exist do nothing
 */
void graph_set_edge (graph G, string vertex1, string vertex2, size_t weight) {
    if (G!=NULL) {
        GNode N1 = G->nodeHead;
        GNode N2 = G->nodeHead;
        // check out whether V1, V2 existed
        while (N1 != NULL) {
            if (strcmp(N1->v,vertex1) == 0) {
                break;
            }
            N1 = N1->next;
        }
        while (N2 != NULL) {
            if (strcmp(N2->v,vertex2) == 0) {
                break;
            }
            N2 = N2->next;
        }
        if (N1 != NULL && N2 != NULL) {
            // check out whether arc V1->V2 existed
            ALNode targetNode;
            targetNode = N1->ALNHead;
            while (targetNode != NULL) {
                if (strcmp(targetNode->nodeW->v, N2->v)==0) {
                    break;
                }
                targetNode = targetNode->nextALN;
            }
            if (targetNode != NULL) {
                // V1->V2 is in V1's Adjacency List
                targetNode->weight = weight;
            }
        }
    }
}

/**
 * graph_get_edge
 * return the weight of the edge between two vertices
 * return 0 on error
 */
size_t graph_get_edge (graph G, string vertex1, string vertex2) {
    if (G != NULL) {
        GNode N1 = G->nodeHead;
        GNode N2 = G->nodeHead;
        // check out whether V1, V2 existed
        while (N1 != NULL) {
            if (strcmp(N1->v,vertex1) == 0) {
                break;
            }
            N1 = N1->next;
        }
        while (N2 != NULL) {
            if (strcmp(N2->v,vertex2) == 0) {
                break;
            }
            N2 = N2->next;
        }
        if (N1 != NULL && N2 != NULL) {
            // check out whether arc V1->V2 existed
            ALNode targetNode;
            targetNode = N1->ALNHead;
            while (targetNode != NULL) {
                if (targetNode->nodeW == N2) {
                    break;
                }
                targetNode = targetNode->nextALN;
            }
            if (targetNode != NULL) {
                // V1->V2 is in V1's Adjacency List
                return targetNode->weight;
            }
        }
    }
    return 0;
}

/**
 * graph_edges_count
 * return the number of outgoing edges from a particular vertex in the graph
 * return 0 on error
 */
size_t graph_edges_count (graph G, string vertex) {
    if (G!=NULL) {
        GNode N1 = G->nodeHead;
        // check out whether V1, V2 existed
        while (N1 != NULL) {
            if (strcmp(N1->v,vertex) == 0) {
                break;
            }
            N1 = N1->next;
        }
        // check out number of adjacency
        ALNode targetNode = N1->ALNHead;
        int count;
        count = 0;  // initialization
        while (targetNode != NULL) {
            count++;
            targetNode = targetNode->nextALN;
        }
        return count;
    }
    return 0;
}


// count the out edges of the vertex without link to ignore list nodes
int edgesCounter (graph G, string vertex, list ignore) {
    int countALN = 0;
    GNode targetNode = G->nodeHead;
    while (targetNode != NULL) {
        if(strcmp(targetNode->v,vertex) == 0){
            // find the target node with the vertex
            ALNode AN0 = targetNode->ALNHead;
            while (AN0 != NULL){
                if(list_length(ignore) == 0) {
                    countALN++;
                } else {
                    if(list_contains(ignore,AN0->nodeW->v) == false) {
                        countALN++;
                    }
                }
                AN0 = AN0->nextALN;
            }
            break;
        }
        targetNode = targetNode->next;
    }
    return countALN;
}
/*
 * PageRank algorithm
 * */
void graph_pagerank(graph G, double damping, double delta, list ignore) {
    //make sure that node remove will not make influence on the node deduct
    GNode Nin = G->nodeHead;
    int countNode = 0;
    while (Nin != NULL) {
        Nin->index = countNode;
        countNode++;
        Nin = Nin->next;
    }

    int N = countNode;
    double oldRank[N];
    if (G->pageRankArray != NULL) {
        free(G->pageRankArray);
    }
    // allocate the pageRank;
    double * pageRank = malloc(N * sizeof(double));
    if (pageRank != NULL) {
        G->pageRankArray = pageRank;
    }
    // initialization
    int ignore_count = 0;
    // set ignored vertices
    GNode Ni = G->nodeHead;
    while (Ni != NULL) {
        if (list_contains(ignore,Ni->v)) {
            Ni->exist = false;
            //G->pageRankArray[Ni->index] = 0;    // ignore node set 0
            ignore_count++;
        } else {
            Ni->exist = true;
        }
        Ni = Ni->next;
    }
    N = N - ignore_count;  // deduct the ignored Nodes

    for (int i = 0; i < countNode; ++i) {
        //if (G->pageRankArray[i] == -1) {
        //    G->pageRankArray[i] = 1/(N*1.0);
        //}
        oldRank[i] = 0;
        G->pageRankArray[i] = 1/(N*1.0);
    }
    // while |pageRank[v]-oldRank[v]| of ny V in vertices of G > epsilon
    bool ckpt = true;
    while(ckpt) {
        int count = 0;
        // NEED EXCLUDE IGNORE
        for (int j = 0; j < countNode; ++j) {
            // check if j is ignored
            bool curIsIgnored = false;
            GNode Ni0 = G->nodeHead;
            while (Ni0 != NULL) {
                if (Ni0->index == j && Ni0->exist == false) {
                    curIsIgnored = true;
                    break;
                }
                Ni0 = Ni0->next;
            }
            if (!curIsIgnored && fabs(G->pageRankArray[j]-oldRank[j]) > delta) {
                count++;
                break;
            }
        }
        if (count == 0) {
            ckpt = false;
            break;
        }


        // for all V in vertices of G
        for (int i = 0; i < countNode; ++i) {
            oldRank[i] = G->pageRankArray[i];
        }


        double sink_rank = 0;
        // for all vertices of G that have no outbound edges
        GNode N1 = G->nodeHead;
        while (N1 != NULL) {
            if (N1->exist == true) {
                if (edgesCounter(G,N1->v,ignore) == 0){
                    sink_rank += (damping * (oldRank[N1->index]/N));
                }
            }
            N1 = N1->next;
        }
        // for all V in vertices of G
        GNode N_V = G->nodeHead;
        while (N_V != NULL) {
            if (N_V->exist == true) {
                G->pageRankArray[N_V->index] = sink_rank + ((1 - damping) / N);
                // for all I in vertices of G (that have an edge from I to V)
                GNode N_I = G->nodeHead;
                while (N_I != NULL) {
                    if (strcmp(N_V->v, N_I->v) != 0 && N_I->exist == true) {
                        // check out number of adjacency
                        int outI = edgesCounter(G,N_I->v,ignore);
                        if (outI > 0 && graph_has_edge(G, N_I->v, N_V->v) == true) {
                            G->pageRankArray[N_V->index] += ((damping * oldRank[N_I->index]) / outI);
                        }
                    }
                    N_I = N_I->next;
                }
            }
            N_V = N_V->next;
        }

    }
}


bool hasNodeUnvisited(graph G) {
    GNode N0 = G->nodeHead;
    while (N0 != NULL) {
        if (N0->exist == true) {
            return true;
        }
        N0 = N0->next;
    }
    return false;
}

// firstly find the largest pagerank then sorted as alphabet and set visited
void graph_viewrank(graph G, FILE *file, list ignore) {
    // init
    GNode N0 = G->nodeHead;
    while (N0 != NULL) {
        if (list_contains(ignore, N0->v)) {
            N0->exist = false;
        } else {
            N0->exist = true;
        }
        N0 = N0->next;
    }

    // find the node with the highest rank
    // print out the vertices
    // while still have vertices not be output except ignore list
    while (hasNodeUnvisited(G) == true) {
        // find the starting node
        GNode highRankNode = G->nodeHead;
        while (highRankNode != NULL) {
            if (highRankNode->exist == true) {
                break;  // found the first existing node
            }
            highRankNode = highRankNode->next;
        }

        // find the node with the highest rank
        GNode nextRankNode = G->nodeHead;
        while (nextRankNode != NULL) {
            if (nextRankNode->exist == true &&
                G->pageRankArray[nextRankNode->index] >= G->pageRankArray[highRankNode->index]) {
                highRankNode = nextRankNode;
            }
            nextRankNode = nextRankNode->next;
        }
        // for now got the highest rank

        if (highRankNode != NULL) {
            double highRank = G->pageRankArray[highRankNode->index];
            GNode N1 = G->nodeHead;
            while (N1 != NULL) {
                // strcmp (str1,str2) str1 < str2 return neg.
                // if V of n1<highRank in alphabet
                if (N1->exist == true && G->pageRankArray[N1->index] == highRank
                    && strcmp(N1->v, highRankNode->v) < 0) {
                    highRankNode = N1;
                }
                N1 = N1->next;
            }
            if (file == NULL) {
                printf("%s (%.3lf)\n", highRankNode->v, highRank);
            } else {
                fprintf(file, "%s (%.3lf)\n", highRankNode->v, highRank);
            }
        }
        // set as visited
        highRankNode->exist = false;
    }
}


// DijkstraSSSP Algorithm
void initSSSP (graph G) {
    // allocate the pred;
    if (G->pred != NULL) {
        free(G->pred);
    }
    Index * p = malloc(G->nV * sizeof(Index));
    if (p != NULL) {
        G->pred = p;
    }
    // allocate the dist;
    if (G->dist != NULL) {
        free(G->dist);
    }
    int * q = malloc(G->nV * sizeof(int));
    if (q != NULL) {
        G->dist = q;
    }
}

void graph_shortest_path(graph G, string source, list ignore) {
    // initialization start here
    GNode Nin = G->nodeHead;
    int countNode = 0;
    while (Nin != NULL) {
        Nin->index = countNode;
        countNode++;
        Nin = Nin->next;
    }
    // index reverse and check end

    G->nV = countNode;
    int vSet[G->nV];    // vertices visited check
    initSSSP(G);
    // Vertex v has not been visited if vSet[v] = 1
    int numOfV = G->nV;
    for (int i = 0; i < numOfV; ++i) {
        G->dist[i] = VERY_HIGH_VALUE;
        G->pred[i] = -1;    // source node
        vSet[i] = 1;    // 1 donate unvisited
    }

    // set ignored vertices
    GNode Ni = G->nodeHead;
    while (Ni != NULL) {
        if (list_contains(ignore,Ni->v)) {
            Ni->exist = false;
            vSet[Ni->index] = 0;
        } else {
            Ni->exist = true;
        }
        Ni = Ni->next;
    }

    // find the index of the source node
    Index srcIndex = -1;
    GNode srcNode = NULL;
    if (source != NULL) {
        GNode tail = G->nodeHead;
        while (tail != NULL) {
            if (strcmp(tail->v, source) == 0) {
                srcIndex = tail->index;  // vertex existed
                srcNode = tail;
                break;
            }
            tail = tail->next;
        }
    }
    G->source = srcIndex;
    G->dist[srcIndex] = 0;
    if (vSet[srcIndex] == 1) {
        vSet[srcIndex] = 0;
    }

    list PQueue = list_create();
    list_push(PQueue,srcNode->v);
    // initialization end here

    // while there is remaining unvisited vertex
    bool ckpt = true;
    while(ckpt) {
        int count = 0;
        // HAVE EXCLUDED IGNORE
        for (int j = 0; j < numOfV; ++j) {
            if (vSet[j] == 1) {
                count++;
                break;
            }
        }
        if (count == 0|| list_is_empty(PQueue)) {
            ckpt = false;
            break;
        }

        string curStr = list_dequeue(PQueue);
        GNode curNode = G->nodeHead;
        while (curNode != NULL && curNode->next!=NULL) {
            curNode = curNode->next;
        }
        while (curNode != NULL) {
            if (strcmp(curNode->v,curStr) == 0){
                break;  // find the node string pair
            }
            curNode = curNode->lastN;
        }
        // traverse in reverse of adjacency list
        GNode adjNode = G->nodeHead;
        while (adjNode != NULL && adjNode->next!=NULL) {
            adjNode = adjNode->next;
        }
        while (adjNode != NULL) {
            if (adjNode->exist == true && strcmp(adjNode->v,curNode->v) != 0 &&
            graph_has_edge(G,curNode->v,adjNode->v)){
                // find the node string pair
                if (vSet[adjNode->index] == 1) {
                    list_enqueue(PQueue, adjNode->v);
                }
                vSet[adjNode->index] = 0;
                // change the dist
                if (G->dist[curNode->index]+1 < G->dist[adjNode->index]){
                    int newDist = G->dist[curNode->index]+1;
                    G->dist[adjNode->index] = newDist;
                    G->pred[adjNode->index] = curNode->index;
                }
            }
            adjNode = adjNode->lastN;
        }
        free(curStr);
    }
    list_destroy(PQueue);
}


void output_path(graph G, Index cur_i, list ignore){
    // stop case pred = -1 (source node)

    if (cur_i >= 0) {
        GNode N1 = G->nodeHead;
        while (N1 != NULL) {
            if (N1->index == cur_i) {
                break;
            }
            N1 = N1->next;
        }

        if(N1 != NULL && list_contains(ignore,N1->v) == false) {
            // output the path recursively
            Index last_i = G->pred[N1->index];
            output_path(G,last_i,ignore);
            printf("%s\n",N1->v);
        }
    }
}

bool able2ReachSrc(graph G, int dest) {
    // find out if it is able to reach the src from dest
    int lastNode = G->pred[dest];
    while (lastNode >= 0){
        if (lastNode == G->source) {
            return true;
        }
        lastNode = G->pred[lastNode];
    }
    return false;
}

void graph_view_path(graph G, string destination, list ignore) {
    // find the node of destination
    GNode Ni = G->nodeHead;
    while (Ni != NULL) {
        if (strcmp(Ni->v,destination) == 0) {
            break;
        }
        Ni = Ni->next;
    }
    if (Ni != NULL && Ni->index == G->source){
        printf("%s\n",Ni->v);   // src == dest
    }
    else if (Ni != NULL && able2ReachSrc(G, Ni->index)) {
        output_path(G,Ni->index,ignore);
    }
}

