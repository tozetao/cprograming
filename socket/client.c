#include "socket_func.c"

void str_cli(FILE *fp, int sock)
{
    char sendline[MAXLINE];

}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in serve_addr;

    bzero(&serve_addr, sizeof(serve_addr));
    serve_addr.sin_port = htons(9001);
    serve_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &serve_addr.sin_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sockfd, (SA*) &serve_addr, sizeof(serve_addr));

    str_cli(stdin, sockfd);
    exit(0);
}
