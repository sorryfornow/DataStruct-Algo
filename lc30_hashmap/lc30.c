#include <stdlib.h>
#include <string.h>

typedef struct node Node;
struct node
{
    void* key;
    void* value;
    int hash;
    Node* next;
};

typedef struct hash_map
{
    int size;
    int length;
    Node** data;
    int(*hash_code)(void*);
    int(*equals)(void*,void*);
}HashMap;

/*
    扩容
*/
void resize(HashMap* hashMap)
{
    int oldSize = hashMap->size;
    int newSize = oldSize * 2;
    Node** newCup = (Node**)malloc(sizeof(Node*) * newSize);
    memset(newCup, 0, sizeof(Node*) * newSize);
    //拷贝到老数组
    for (int j = 0, size = hashMap->size; j < size; j++)
    {
        Node* e = hashMap->data[j];
        hashMap->data[j] = NULL;
        if (e == NULL)
        {
            continue;
        }
        if (e->next == NULL)
        {
            newCup[e->hash & (newSize - 1)] = e;
        }
        Node* loTail = NULL;
        Node* hiTail = NULL;
        Node* loHead = NULL;
        Node* hiHead = NULL;
        Node* next;
        do
        {
            next = e->next;
            //放在高位
            if ((e->hash & oldSize) != 0)
            {
                if (hiTail == NULL)
                {
                    hiHead = e;
                }
                else
                {
                    hiTail->next = e;
                }
                hiTail = e;
            }
            else
            {
                if (loHead == NULL)
                {
                    loHead = e;
                }
                else
                {
                    loTail->next = e;
                }
                loTail = e;
            }
        } while ((e = next) != NULL);
        newCup[j + oldSize] = hiHead;
        newCup[j] = loHead;
        if (hiTail != NULL)
            hiTail->next = NULL;
        if (loTail != NULL)
            loTail->next = NULL;
    }
    free(hashMap->data);
    hashMap->data = newCup;
    hashMap->size = newSize;
}

Node* createNode(int hash, void* key, void* value, Node* next)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->hash = hash;
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

void FREE0(Node* node)
{
    if (node == NULL)
    {
        return;
    }
    FREE0(node->next);
    free(node);
}

/*
    free
*/
void free_map(HashMap* hashMap)
{
    for (int i = 0, size = hashMap->size; i < size; i++)
    {
        Node* e = hashMap->data[i];
        if (e != NULL && e->next == NULL)
        {
            free(e);
        }
        else
        {
            FREE0(e);
        }
    }
    free(hashMap);
}

/*
    get
*/
void* get(HashMap* hashMap, void* key)
{
    int hash = hashMap->hash_code(key);
    int index = (hashMap->size - 1) & hash;
    Node* node = hashMap->data[index];
    if (node == NULL)
        return NULL;
    if (node->next == NULL)
        return hashMap->equals(node->key,key) ? node->value : NULL;
    Node* next;
    do
    {
        next = node->next;
        if (hashMap->equals(node->key,key))
            return node->value;
    } while ((node = next) != NULL);
    return NULL;
}

/*
    contains
*/
int contains(HashMap* hashMap, void* key)
{
    int hash = hashMap->hash_code(key);
    int index = (hashMap->size - 1) & hash;
    Node* node = hashMap->data[index];
    if (node == NULL)
    {
        return 0;
    }
    if (node->next == NULL)
        return hashMap->equals(node->key,key) ? 1 : 0;
    Node* next;
    do
    {
        next = node->next;
        if (hashMap->equals(node->key,key))
            return 1;
    } while ((node = next) != NULL);
    return 0;
}

/*
    新增
*/
void put(HashMap* hashMap,void* key, void* value)
{
    if (hashMap == NULL)
    {
        return;
    }
    if (hashMap->length == hashMap->size * 0.75)
    {
        resize(hashMap);
    }
    int hash = hashMap->hash_code(key);
    int index = hash & (hashMap->size - 1);
    Node** nodeIndex = &hashMap->data[index];
    //如果该槽位无元素则直接插入
    if (*nodeIndex == NULL)
    {
        *nodeIndex = createNode(hash, key, value, NULL);
        hashMap->length++;
        return;
    }
    Node* e = *nodeIndex;
    Node* next = e;
    do
    {
        e = next;
        next = e->next;
        if (hashMap->equals(e->key,key))
        {
            e->value = value;
            return;
        }
    } while (next != NULL);
    e->next = createNode(hash, key, value, NULL);
    hashMap->length++;
}

/*
    创建hashMap,需指定计算hashcode和equals函数
*/
HashMap* newHashMap(int(*hash_code)(void*),int(*equals)(void*,void*))
{
    HashMap* hashMap = (HashMap*)malloc(sizeof(HashMap));
    hashMap->size = 16;
    hashMap->length = 0;
    hashMap->data = (Node**)malloc(sizeof(Node*) * hashMap->size);
    hashMap->hash_code = hash_code;
    hashMap->equals = equals;
    memset(hashMap->data, 0, sizeof(Node*) * hashMap->size);
    return hashMap;
}

static int equals(void* a,void* b)
{
    return !strcmp((char*)a, (char*)b);
}

static int hashCode(void* key)
{
    unsigned int max = 0xFFFFFFFF;
    long long h = 0;
    char* str = (char*)key;
    int str_len = (int)strlen(str);
    if (h == 0 && str_len > 0) {
        for (int i = 0 ; i < str_len; i++) {
            h = 31 * h + str[i];
            h &= max;
        }
    }
    return (int)h;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
static int* findSubstring(char * s, char ** words, int wordsSize, int* returnSize){
    //初始化
    *returnSize = 0;
    if (!wordsSize)
        return NULL;
    const int word_size = (int)strlen(words[0]);
    const int str_len = (int)strlen(s);
    if (!wordsSize)
        return NULL;
    if (!str_len)
        return NULL;
    //使用map
    HashMap* hashMap = newHashMap(hashCode,equals);
    //标记数组
    int* const flag_array = (int*)malloc(sizeof(int) * wordsSize * 2);
    //标记比较数组
    int* const flag_array_cmp = flag_array + wordsSize;
    memset(flag_array, 0 , sizeof(int) * wordsSize * 2);
    //返回数组
    int* const return_array = (int*)malloc(sizeof(int) * 100);
    for (int i = 0,j = 0; i < wordsSize; i++)
    {
        if (strlen(words[i]) != word_size)
        {
            free_map(hashMap);
            return NULL;
        }
        int* c =(int*)get(hashMap, words[i]);
        if (c != NULL)
        {
            flag_array[*c]++;
            continue;
        }
        int* tmp_j = (int*)malloc(sizeof(int));
        *tmp_j = j;
        char* tmp_str = (char*)malloc(sizeof(char) * word_size + 1);
        strcpy(tmp_str, words[i]);
        put(hashMap,tmp_str,tmp_j);
        flag_array[j++] = 1;
    }
    int sum_w = 0;
    for (int i = 0; i < wordsSize; i++)
        sum_w += words[i][0];
//    for (int i = 0; i < 4; i++)
//    {
//        printf("%d ",flag_array[i]);
//    }
//    printf("\n");
    char* tmp_cmp_str = (char*)malloc(sizeof(char) * word_size + 1);
    memset(tmp_cmp_str, 0, sizeof(char) * word_size + 1);
    for (int k = 0;; k++)
    {
        int flag = 1;
        int sum_s = 0;
        for (int i = 0,t_k = k; i < wordsSize; i++,t_k += word_size)
        {
            if (t_k >= str_len)
            {flag = 0;break;}
            sum_s += s[t_k];
        }
        if (!flag)
            break;
        if (sum_s != sum_w)
            continue;
        for (int j = 0,f = k; j < wordsSize; j++,f += word_size)
        {
            strncpy(tmp_cmp_str, s + f, sizeof(char) * word_size);
            int* c = (int*)get(hashMap, tmp_cmp_str);
            if (c == NULL)
                goto loop;
            if (flag_array_cmp[*c] == flag_array[*c])
                goto loop;
            flag_array_cmp[*c]++;
        }
        return_array[(*returnSize)++] = k;
        memset(flag_array_cmp, 0 , sizeof(int) * wordsSize);
        continue;
    loop:
        memset(flag_array_cmp, 0 , sizeof(int) * wordsSize);
    }
    free(flag_array);
    free(tmp_cmp_str);
    free_map(hashMap);
    return return_array;
}