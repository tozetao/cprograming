#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qunue.h"

#define BUF_SIZE 1024
#define MAX(x, y) x > y ? x : y

int read_line(char[], int);

int main()
{
    int a = 10, b = 100;

    printf("%d\n", MAX(a, b));

    return 0;
}

int read_line(char str[], int n)
{
    int ch, i = 0;

    while ((ch = getchar()) != '\n') {
        if (i >= n) {
            str[i] = '\0';
            return i;
        }

        str[i++] = ch;
    }

    str[i] = '\0';
    return i;
}
