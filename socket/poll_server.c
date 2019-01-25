#include "socket_func.c"
#include <poll.h>
#include <sys/stropts.h>
#include <limits.h>

static long OPEN_MAX = 0;
int open_max()
{
    if (OPEN_MAX == 0) {
     OPEN_MAX = sysconf(_SC_OPEN_MAX);
    }
    return OPEN_MAX;
}

int main()
{
    int i, maxi, listenfd, connfd;
    int nready, n;

    int cliaddr_size;

    char buffer[MAXLINE];
    struct pollfd client[open_max()];
    
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVE_PORT);
    
    // 创建socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    // 绑定
    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    listen(listenfd, 100);

    // 初始化
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    
    for (i = 1; i < OPEN_MAX; i++)
        client[i].fd = -1;
    
    maxi = 0;

    while(1) {
        // 开始监听socket描述符
        nready = poll(client, maxi + 1, -1);

        // 处理新的连接
        if (client[0].revents & POLLRDNORM) {
            cliaddr_size = sizeof(cliaddr);
            connfd = accept(listenfd, (SA *)&cliaddr, &cliaddr_size);

            for (i = 1; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    break;
                }
            }

            // 连接过多
            if (i == OPEN_MAX) {
                printf("too many connections\n");
                close(connfd);
                continue;
            }

            client[i].events = POLLRDNORM;
            
            if (i > maxi)
                maxi = i;

            if (--nready <= 0)
                continue;
        }

        // 处理就绪的socket描述符
        for(i = 1; i <= maxi; i++) {
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(client[i].fd, buffer, MAXLINE) < 0)) {
                    if (errno == ECONNRESET) {
                        close(client[i].fd);
                        client[i].fd = -1;
                    } else {
                        printf("read error\n");
                    }
                } else if (n == 0) {
                    client[i].fd = -1;
                    close(client[i].fd);
                } else {
                    written(client[i].fd, buffer, n);
                }
            }
        }
    }

    return 0;
}
