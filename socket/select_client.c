#include "socket_func.c"

#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>

void sig_pipe(int signo)
{
    printf("signo = %d\n", signo);
    return;
}

void str_cli(FILE *file, int sockfd)
{
    int maxfd;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];

    FD_ZERO(&rset);
    
    while (1) {
        FD_SET(fileno(file), &rset);
        FD_SET(sockfd, &rset);

        maxfd = max(fileno(file), sockfd) + 1;
        select(maxfd, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            if (readline(sockfd, recvline, MAXLINE) == 0) 
                printf("str_cli: server is prematurely terminated\n");
            else
                fputs(recvline, stdout);
        }

        if (FD_ISSET(fileno(file), &rset)) {
            if (fgets(sendline, MAXLINE, file) == NULL)
                return;

            written(sockfd, sendline, strlen(sendline));
        }
    }
}

int main()
{
    int sockfd;
    char ip[] = "127.0.0.1";
    struct sockaddr_in serve_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serve_addr, sizeof(serve_addr));
    serve_addr.sin_port = htons(SERVE_PORT);
    serve_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &serve_addr.sin_addr);
   
    signal(SIGPIPE, sig_pipe);

    if (connect(sockfd, (SA*) &serve_addr, sizeof(serve_addr)) == -1) {
        printf("%s\n", "connect error");
        exit(-1);
    }
   
    str_cli(stdin, sockfd);

    exit(0);
}
