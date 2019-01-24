#include "socket_func.c"
#include "sum.h"

#include <signal.h>
#include <wait.h>

int main() 
{
    int serve_sock, clnt_sock, pid, clnt_addr_size;
    struct sockaddr_in serve_addr, clnt_addr;

    fd_set rset, allset;
    int client[FD_SETSIZE], i, maxi, maxfd, nready;

    bzero(&serve_addr, sizeof(serve_addr));
    bzero(&clnt_addr, sizeof(clnt_addr));

    //创建socket
    serve_sock =  socket(AF_INET, SOCK_STREAM, 0);

    //绑定协议族
    serve_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //serve_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serve_addr.sin_family = AF_INET;
    serve_addr.sin_port = htons(SERVE_PORT);
    bind(serve_sock, (SA *)&serve_addr, sizeof(serve_addr));

    //初始化FDSET、client
    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;

    FD_ZERO(&allset);
    maxfd = serve_sock;
    maxi = 0;

    FD_SET(serve_sock, &allset);

    //开始监听
    listen(serve_sock, 100);

    //处理请求
    while(1) {
        sleep(1);
        // 监听socket描述符是否就绪
        rset = allset;        
        printf("select waitting...\n");
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        printf("nready = %d\n", nready);

        // 如果是监听描述符变为可读，表示有新的连接建立
        if (FD_ISSET(serve_sock, &rset)) {
            printf("connection process\n");

            clnt_addr_size = sizeof(clnt_addr);
            clnt_sock = accept(serve_sock, (SA *)&clnt_addr, &clnt_addr_size);

            // 记录新的连接描述符
            for(i = 0; i < FD_SETSIZE; i++) {
                if (client[i] == -1) {
                    client[i] = clnt_sock;
                    break;
                }
            }

            // 加入到监听集合中
            FD_SET(clnt_sock, &allset);

            if (i > maxi)
                maxi = i;

            if (clnt_sock > maxfd)
                maxfd = clnt_sock;
            
            printf("maxi = %d; maxfd = %d\n", maxi, maxfd);

            // 判断是否有可读的连接描述符要处理
            if (--nready <= 0)
                continue;
        }

        int n = 0;
        char buffer[MAXLINE];

        // 处理可读的连接描述符
        for (i = 0; i <= maxi; i++) {
            if ((clnt_sock = client[i]) == -1)
                continue;

            printf("clnt_sock %d process\n", clnt_sock);

            if (FD_ISSET(clnt_sock, &rset)) {
                if ((n = read(clnt_sock, buffer, MAXLINE)) == 0) {
                    close(clnt_sock);
                    FD_CLR(clnt_sock, &allset);
                    client[i] = -1;
                    printf("close clnt_sock = %d\n", clnt_sock);
                } else {
                    printf("read %d bytes from client\n", n);
                    written(clnt_sock, buffer, n); 
                }
            }

            //if (--nready <= 0)
            //    break;
        }

    }
}
