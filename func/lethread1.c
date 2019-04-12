#include <stdio.h>

int imax(int, int);

int main() 
{
    int sq[][] = {1, 2, 3, 4, 51, 2, 3, 4, 5};

    int i, j;

    for(i = 0; i < 2; i++) {
        for(j = 0; j < 3; j++) {
            printf("%d\n", sq[i][j]);
        }
    }

    return 0;
}

int imax(int a, int b) 
{
    return a > b ? a : b;
}
