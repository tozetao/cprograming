#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 20

void error_handling(char *message);

int main(int argc, char const *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];
    if(argc != 2)
    {
        error_handling("argc error!");
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    printf("serv_sock=%d\n", fd_max);

    while(1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5;

        if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;

        if(fd_num == 0)
            continue;
        
        printf("fd_num=%d\n", fd_num);

        //遍历发生变化的套接字
        for(i=0; i< fd_max+1; i++)
        {
            if(FD_ISSET(i, &cpy_reads))
            {
                //服务端套接字发生变化，表示有连接请求
                if(i == serv_sock)
                {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads);

                    if(fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    
                    printf("fd_max = %d\n", fd_max);
                    printf("connected client: %d\n", clnt_sock);
                }
                else    //与客户端连接的套接字发生变化
                {
                    //如果客户端连接发送的是EOF数据，关闭该套接字并清除在FD_SET数组中的注册信息
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0)
                    {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d\n", i);
                    }
                    else
                    {
                        write(i, buf, str_len);
                    }
                }
            }
            else
            {
                printf("don't catch socket change.\n");
            }
        }
    }

    close(serv_sock);
    return 0;
}


void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
