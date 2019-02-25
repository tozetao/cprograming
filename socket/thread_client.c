#include "socket_func.c"
#include <pthread.h>

static int sockfd;
static FILE *fp;

void *copyto(void *);

// 主线程负责从socket读取数据并输出，子线程负责读取标准输入并输出给服务器
void thread_str_cli(FILE *fp_arg, int sockfd_arg)
{
    char recvline[MAXLINE];
    pthread_t tid;

    sockfd = sockfd_arg;
    fp = fp_arg;

    // 创建子线程，执行copyto
    pthread_create(&tid, NULL, copyto, NULL);
    
    // 读取socket数据并输出
    while (readline(sockfd, recvline, MAXLINE) > 0)
        fputs(recvline, stdout);
}

void *copyto(void * arg)
{
    char sendline[MAXLINE];
    
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        written(sockfd, sendline, strlen(sendline));
    }

    shutdown(sockfd, SHUT_WR);
    return NULL;
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

    if (connect(sockfd, (SA*) &serve_addr, sizeof(serve_addr)) == -1) {
        printf("%s\n", "connect error");
        exit(-1);
    }
    
    thread_str_cli(stdin, sockfd);

    exit(0);
}
