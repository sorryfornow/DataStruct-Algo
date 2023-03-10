//
// Created by Rui Mu on 31/7/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT_LENGTH 1024
#define TEXT_FORMAT_STRING "%[^\n]%*c"




int *lastOccurrence(char *pattern, char *alphabet) {
    // 一共最大有127个
    int *L = calloc(127, sizeof(int));
    for(int i = 0; i < strlen(alphabet); i++) {
        L[alphabet[i]] = -1;
    }

    for(int i = 0; i < strlen(pattern); i++) {
        L[pattern[i]] = i;
    }

    return L;
}

int BoyerMooreMatch(char *T , char * P, char *alphabet) {
    int *L = lastOccurrence(P,alphabet);

    printf("\n");
    for(int i = 0; i< strlen(alphabet); i++) {
        printf("L[%c] = %d\n", alphabet[i], L[alphabet[i]]);


        fprintf(stdout, "L[%c] = %d\n", alphabet[i], L[alphabet[i]]);
    }

    int m = strlen(P);
    int n = strlen(T);

    int i = m - 1;
    int j = m - 1;
    while (i < n) {
        if (T[i] == P[j]) {
            if (j == 0) {
                return i;
            } else {
                i--;
                j--;
            }
        } else {
            if (j < (1 + L[T[i]])) {
                i = i + m - j;
            } else {
                i = i + m - (1 + L[T[i]]);
            }
            j = m - 1;
        }
    }

    printf("\n");

    free(L);

    return -1;
}

int main(void) {
    char alphabet[MAX_TEXT_LENGTH];
    char T[MAX_TEXT_LENGTH];
    char P[MAX_TEXT_LENGTH];

    printf("Enter alphabet: ");
    scanf(TEXT_FORMAT_STRING, alphabet);
    printf("Enter text: ");
    scanf(TEXT_FORMAT_STRING, T);
    printf("Enter pattern: ");
    scanf(TEXT_FORMAT_STRING, P);

    int result = BoyerMooreMatch(T, P ,alphabet);
    if (result == -1) {
        printf("No Match\n");
    } else {
        printf("Match found at position %d.\n",result);
    }

    return 1;
}