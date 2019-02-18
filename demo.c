#include <stdlib.h>
#include <stdio.h>

void example1()
{
    char const *keyTables[] = {
        "dasheng",
        "xiaoli",
        "name",
        "ab"
    };

    int i, length;
    char const **ele = keyTables;
    
    length = sizeof(keyTables) / sizeof(keyTables[0]);
    printf("%d\n", length);
    for(i = 0; i < length; i++ ){
        printf("%s\n", keyTables[i]);
    }
}

void example2()
{
    char const *keyTables[] = {
        "a",
        "b",
        "abcd",
        NULL
    };

    char const **val = keyTables;

    printf("%s\n", *val);
    printf("%c\n", **val);

    val = val + 2;
    printf("%s\n", *val);
    printf("%c\n", *(*val + 2));

    // for(val = keyTables; *val != NULL; val++) {
    //     printf("%c\n", **val);
    // }

    /*
    int printf(char const *format, ...);
    format：格式化字符串
    ...是格式化字符所对应的具体的值，如果是int型则是具体的值，如果是字符串要对应字符串所指向的指针。
    */
}

int main(int argc, char const *argv[])
{
    example2();
    return 0;
}
