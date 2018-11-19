#include <stdio.h>

int main(int argc, char const *argv[])
{
    char buffer[1024];
    ssize_t numRead;

    numRead = read(STDIN_FILENO, buffer, 1024);
    buffer[numRed] = '\0';
    printf("%s", buffer);
    return 0;
}
