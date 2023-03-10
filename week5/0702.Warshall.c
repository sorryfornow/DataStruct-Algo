

// Warshall's algorithm ... COMP9024 21T3
#include <stdio.h>
#include <stdbool.h>

#define NODES 4
#define N_V 6

int digraph[NODES][NODES] = {{0, 1, 1, 1},
                             {1, 0, 1, 0},
                             {0, 1, 0, 0},
                             {0, 0, 0, 0}};
int tc[NODES][NODES];

void warshall() {
    int i, s, t;
    for (s = 0; s < NODES; s++)
        for (t = 0; t < NODES; t++)
            tc[s][t] = digraph[s][t];

    for (i = 0; i < NODES; i++)
        for (s = 0; s < NODES; s++)
            for (t = 0; t < NODES; t++)
                if (tc[s][i] && tc[i][t])
                    tc[s][t] = 1;
}

// reachable
void warshall_01() {

    int digraph01[N_V][N_V] = {
            {1, 0, 0, 1, 1, 0},
            {0, 1, 1, 1, 0, 0},
            {0, 1, 1, 0, 0, 0},
            {1, 1, 0, 1, 0, 1},
            {1, 0, 0, 0, 1, 0},
            {0, 0, 0, 1, 0, 1}

    };

    int tc01[N_V][N_V];
    int i, s, t;
    for (s = 0; s < N_V; s++)
        for (t = 0; t < N_V; t++)
            tc01[s][t] = digraph01[s][t];
    for (i = 0; i < N_V; i++) {
        printf("第 %d 次 iteration: \n", i);
        for (s = 0; s < N_V; s++)
            for (t = 0; t < N_V; t++)
                if (tc01[s][i] && tc01[i][t]) {
                    tc01[s][t] = 1;
                    if (tc01[s][t] != digraph01[s][t]) {
                        printf("%d %d\n", s, t);
                    }
                }
    }
}

int main01(void) {
    warshall();
    int i, j;
    for (i = 0; i < NODES; i++) {
        for (j = 0; j < NODES; j++) {
            printf("%d ", tc[i][j]);
        }
        putchar('\n');
    }
    return 0;
}

int main(void) {
    warshall_01();
}
