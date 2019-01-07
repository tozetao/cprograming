#include <stdlib.h>
#include <stdio.h>

int main()
{
    union {
        short s;
        char c[2];
    } un;
    
    un.s = 0x0102;

    if (sizeof(short) == 2){
        if (un.c[0] == 1 && un.c[1] == 2) {
            printf("big endition\n");
        } else if (un.c[0] == 2 && un.c[1] == 1) {
            printf("small edi\n");
        } else {
            printf("unknow\n");
        }
    } else {
        printf("short size != 2\n");
    }    

    return 0;
}
