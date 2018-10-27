#include <stdlib.h>
#include <stdio.h>

void size_demo()
{
    char const *keys[] = {
        "dasheng",
        "xiaoli",
        "name",
        "ab"
    };

    int i, size, length;
    char const **ele = keys;
    
    size = sizeof(keys[0]);     // 指针的大小
    length = sizeof(keys) / size;

    printf("size = %d\n", size);
    printf("length = %d\n", length);

    for(i = 0; i < length; i++) {
        printf("%s\n", keys[i]);
    }
}

void loop()
{
    char const *keys[] = {
        "a",
        "abc",
        "abcd",
        NULL
    };

    char const **val;

    // printf each element.
    for(val = keys; *val != NULL; val++) {
        printf("%s\n", *val);
    }

    // printf the second element
    char const *ele = *(keys + 2);
    char t;
    while((t = *ele++) != '\0') {
        printf("%c\n", t);
    }
}

int main(int argc, char const *argv[])
{
    size_demo();
    return 0;
}