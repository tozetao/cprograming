#include <stdlib.h>
#include <stdio.h>

void myclear() {
    printf("myclear\n");
}

int main(int argc, char const *argv[])
{
    printf("start main\n");
    atexit(myclear);
    return 0;
}