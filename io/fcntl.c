#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void printf_err(char *s)
{
    printf("%s, errno=%d\n", s, errno);
    exit(-1);
}

void getfl_test() 
{
    int fd, flags, rs, mode;

    flags = O_RDWR | O_CREAT | O_APPEND | O_TRUNC | O_SYNC;
    fd = open("tesg.log", flags);

    if (fd == -1) {
        printf_err("open error");
    }

    rs = fcntl(fd, F_GETFL);
    printf("%x\n", rs);

    rs &= O_APPEND;
    printf("%x\n", O_APPEND);
    printf("%x\n", rs);

    rs = fcntl(fd, F_GETFL);
    mode = rs & O_ACCMODE;
    printf("%x\n", O_ACCMODE);
    printf("%x\n", mode);
    printf("%d\n", mode == O_RDWR);
    printf("%d\n", mode == O_RDONLY);
}

void setfl_test()
{
    int fd, flags, rs;

    fd = open("test.log", O_RDWR | O_CREAT);
    flags = fcntl(fd, F_GETFL);

    flags |= O_APPEND;
    if(fcntl(fd, F_SETFL, flags) == -1){
        printf_err("set fd error.");
    }
}

int main(int argc, char const *argv[])
{


    return 0;
}