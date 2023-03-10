#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool isHeterogram(char A[],int len){
    // 初始化一个数组，长度为26 用来记录字母 a-z出现的次数
    int letterCounter[26];
    for (int i = 0; i < 26; ++i) {
        letterCounter[i] = 0;//最开始的时候counter字母个数都是0
    }
    // 算法的思想是 数组初始化的值全都是0 也就是 a-z都没有出现过， 在遍历每一个letter的时候
    // 先检查当前letter有没有变成1 如果已经是1了 那就证明这个letter之前出现过了 整个函数直接终止并返回 false
    // 如果是0 证明这是一个新的letter 记录它出现了 也就是把他的counter对应的值 置为1

    //然后开始遍历这个word的每一个字母
    for (int i = 0; i < len; ++i) {
        if (letterCounter[A[i]-'a']){
            //如果当前字母的counter已经是1（或者大于一 但是此时不可能大于一 因为一旦大于一 代码早前就已经终止了）了
            return false;
        } else{ // 这里的else指的是 当前计数器LC['*' - 'a'] == 0 即，这个字母没见过
            // else也就是说 counter记录到的当前字母没有出现过，那就记录他出现了
            letterCounter[A[i]-'a']++;
        }
    }
    return true;
}


int main() {
    printf("Enter a word: ");
    char word[32];
    scanf("%s", word);
    int length = strlen(word);
    if (isHeterogram(word, length)){
        printf("yes");
    } else {
        printf("no");
    }
    return 0;
}
