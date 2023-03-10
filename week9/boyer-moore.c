#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT_LENGTH 1024
#define TEXT_FORMAT_STRING "%[^\n]%*c"


int *lastShow(char *pattern, char *alphabet) {
    int *last = calloc(127, sizeof(int));
    for(int i = 0; i < strlen(alphabet); i++) {
        int mid = alphabet[i];
        last[mid] = -1;
    }
    for(int i = 0; i < strlen(pattern); i++) {
        int mid = pattern[i];
        last[mid] = i;
    }
    return last;
}

void BoyerMooreMatch(char *T , char * P, char *alphabet) {
    int *last = lastShow(P,alphabet);
    printf("\n");
    for(int in = 0; in< strlen(alphabet); in++) {
        int indexA = alphabet[in];
        printf("L[%c] = %d\n", alphabet[in], last[indexA]);
    }

    int ckpt = -1;
    int lengthP = strlen(P);
    int i = lengthP - 1;
    int j = lengthP - 1;

    while (i < strlen(T)) {
        if (T[i] == P[j]) {
            if (j == 0) {
                ckpt = i;
                break;
            } else {
                i--;
                j--;
            }
        } else {
            int indexT = T[i];
            int num = 1 + last[indexT];
            if (j < num) {
                i = i + lengthP - j;
            } else {
                i = i + lengthP - num;
            }
            j = lengthP - 1;
        }
    }
    printf("\n");
    if (ckpt != -1) {
        printf("Match found at position %d.",ckpt);
    } else {
        printf("No Match.");
    }
    free(last);
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

    BoyerMooreMatch(T, P ,alphabet);
    return 1;
}