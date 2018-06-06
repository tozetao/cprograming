#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 20

int main(int argc, char const *argv[])
{
    fd_set reads, temps;
    int result, str_len;

    char buffer[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);

    //设置标准输入
    FD_SET(0, &reads);

    while(1)
    {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temps, 0,0, &timeout);
        if(result == -1)
        {
            puts("select() error!");
            break;
        }

        if(result == 0)
        {
            puts("Time-out!");
        }
        else
        {
            if(FD_ISSET(0, &temps))
            {
                str_len = read(0, buffer, BUF_SIZE);
                buffer[str_len] = 0;
                printf("message from console: %s\n", buffer);
            }
        }
    }

    return 0;
}
