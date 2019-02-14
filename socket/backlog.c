#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

static short stop = 0;

static void handle_term(int sig)
{
    stop = 1;
}

int main()
{
    // kill命令传递的默认信号，相当于询问当前进程是否要执行关闭
    signal(SIGTERM, handle_term);

    const char ip[] = "192.168.0.136";
    
    int port = 9001;
    int backlog = 5;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 创建IPv4 socket地址
    struct sockaddr_in address;
    bzero(&address, sizeof(address));

    inet_pton(AF_INET, ip, (void *)&address.sin_addr);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    int ret = bind(sockfd, (struct sockaddr*) &address, sizeof(address));
    assert(ret != -1);

    ret = listen(sockfd, backlog);
    assert(ret != -1);

    while (!stop)
        sleep(1);

    close(sockfd);

    return 0;
}
