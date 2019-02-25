#include <pthread.h>
#include <malloc.h>
#include "socket_func.c"

static void *doit(void *);  /* echo thread executes this function */
void str_echo(int sockfd);  /* echo string to client */

int main()
{
    int listenfd, *connfd;

    pthread_t tid;
    socklen_t addrlen;

    struct sockaddr_in serve_addr, clnt_addr;

    bzero(&serve_addr, sizeof(serve_addr));
    serve_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serve_addr.sin_family = AF_INET;
    serve_addr.sin_port = htons(SERVE_PORT);

    // 创建socket，绑定，监听
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenfd, (SA *)&serve_addr, sizeof(serve_addr));
    listen(listenfd, 100);

    addrlen = sizeof(clnt_addr);

    // accept
    while (1) {
        connfd = malloc(sizeof(int));
        *connfd = accept(listenfd, (SA *)&clnt_addr, &addrlen);
        pthread_create(&tid, NULL, doit, connfd);
    }
    
    return 0;
}

static void *doit(void *arg)
{
    int socket = *(int *)arg;
    free(arg);
    printf("sockfd = %d\n", socket);

    pthread_detach(pthread_self());
    str_echo(socket);
    close(socket);
    return NULL;
}

void str_echo(int socket)
{
    // 一直读取数据，直到客户端主动关闭连接
    int n;
    int buffer[MAXLINE];

again:
    while ((n = readline(socket, buffer, MAXLINE)) > 0) {
        printf("read %d bytes from client\n", n);
        written(socket, buffer, n);
    }
    // 防止被信号中断
    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0) 
        printf("str_echo error");
}
