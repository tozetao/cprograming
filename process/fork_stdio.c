#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    printf("hello world.\n");
    write(STDOUT_FILENO, "Ciao\n", 5);

    if (fork() == -1) {
        printf("fork error\n");
        exit(-1);
    }

    return 0;
}