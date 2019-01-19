#include "socket_func.c"

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

int main(int argc, char **argv)
{
    int sockfds[5], sockfd, i;
    struct sockaddr_in serve_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serve_addr, sizeof(serve_addr));
    serve_addr.sin_port = htons(SERVE_PORT);
    serve_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &serve_addr.sin_addr);
    
    if (connect(sockfd, (SA*) &serve_addr, sizeof(serve_addr)) == -1) {
        printf("%s\n", "connect error");
        exit(-1);
    }
   
    str_cli(stdin, sockfd);
    
    // 创建多个客户端进行连接
//    for(i = 0; i<5; i++) {
//        sockfds[i] = socket(AF_INET, SOCK_STREAM, 0);
//        bzero(&serve_addr, sizeof(serve_addr));
//        serve_addr.sin_port = htons(SERVE_PORT);
//        serve_addr.sin_family = AF_INET;
//        inet_pton(AF_INET, argv[1], &serve_addr.sin_addr);
//        
//        if (connect(sockfds[i], (SA*) &serve_addr, sizeof(serve_addr)) == -1) {
//            printf("%s\n", "connect error");
//            exit(-1);
//        }
//    }
//   
//    str_cli(stdin, sockfds[0]);

    exit(0);
}
