#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);

//客户端负责多次发数据
int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;

    struct sockaddr_in serv_addr, from_adr;

    if(argc != 3)
    {
        printf("argc != 3");
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    char msg1[] = "hello world'";
    char msg2[] = "fuck your month";
    char msg3[] = "this is messsage";

    sendto(sock, msg1, sizeof(msg1), 0, (struct *sockaddr)&serv_addr, sizeof(serv_addr));
    sendto(sock, msg2, sizeof(msg2), 0, (struct *sockaddr)&serv_addr, sizeof(serv_addr));
    sendto(sock, msg3, sizeof(msg3), 0, (struct *sockaddr)&serv_addr, sizeof(serv_addr));

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}