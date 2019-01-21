#include "socket_func.c"
#include <signal.h>

void str_cli(FILE *fp, int sock)
{
    ssize_t n;
    char sendline[MAXLINE], recvline[MAXLINE];
    
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (written(sock, sendline, strlen(sendline)) == -1) {
            printf("%s\n", "written error");
            exit(-1);
        }

        if ((n = readline(sock, recvline, MAXLINE)) <= 0) {
            printf("%s\n", "readline error");
            exit(-1);
        }

        fputs(recvline, stdout);
    }
}

void str_cli_pipe(FILE *fp, int sock)
{
    ssize_t n;
    char sendline[MAXLINE], recvline[MAXLINE];

    while ((fgets(sendline, MAXLINE, fp)) != NULL) {
        // 写入一个字节
        written(sock, sendline, 1);
        sleep(2);
        // 再写入剩余数据
        written(sock, sendline + 1, strlen(sendline) - 1);

        if ((n = readline(sock, recvline, MAXLINE)) == 0 ){
            printf("%s\n", "readline error");
            exit(-1);
        }

        fputs(recvline, stdout);
    }
}

void sig_pipe(int signo)
{
    printf("signo = %d\n", signo);
    return;
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in serve_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serve_addr, sizeof(serve_addr));
    serve_addr.sin_port = htons(SERVE_PORT);
    serve_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &serve_addr.sin_addr);
   
    signal(SIGPIPE, sig_pipe);

    if (connect(sockfd, (SA*) &serve_addr, sizeof(serve_addr)) == -1) {
        printf("%s\n", "connect error");
        exit(-1);
    }
   
    str_cli(stdin, sockfd);
    //str_cli_pipe(stdin, sockfd);

    exit(0);
}
