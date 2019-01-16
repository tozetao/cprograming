#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char buffer[1024];
    int fd, count;

    bzero(buffer, sizeof(buffer));

//    fd = open("test.log", O_RDWR);
//    count = read(fd, buffer, 100);
//    char *tp, sp;
//    tp = buffer;
//
//    sp = *tp++;
//    printf("%c\n", *tp);
//    printf("%c\n", sp);
    
    while(fgets(buffer, 1024, stdin) != NULL) {
        //printf("%s", buffer);
        fputs(buffer, stdout);
    }

    return 0;
}
