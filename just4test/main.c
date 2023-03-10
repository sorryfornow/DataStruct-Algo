#include <stdio.h>

int main() {
    int i;
    int array[] = {13,15,16};
    int max_v = array[0];
    for (i = 1; i <= 2; ++i) {
        if (array[i] > max_v){
            max_v = array[i];
        }
    }
    printf("%d",i);
    return 0;
}
