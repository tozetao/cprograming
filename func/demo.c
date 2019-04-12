#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.c"

typedef int *Func(void);
typedef Func *FuncPointer;

void list_test(char **c)
{
    char *name = malloc(10);
    name = "test";
    *c = name;
    printf("in function: %s\n", name);
}

int test()
{
    printf("function name is test\n");
    return 0;
}

void test1()
{
    printf("function name is test1\n");
}

typedef int (*Pa)[5];
Pa test2(int i) {
    printf("test2, i = %d\n", i);
    return NULL;
}

float *test3()
{
    printf("test3\n");
}

float *(*y(void))(void)
{
    printf("this function is y\n");
    return test3;
}

int main()
{
    return 0;
}
