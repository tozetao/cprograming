#include "socket_func.c"
#include <fcntl.h>

/*
 * 测试socket的阻塞特性，
 * 编译并允许该程序，然后使用telnet连接该程序，将会看到程序阻塞在read()函数这里
 */

int main() 
{
    int serve_sock, clnt_sock;
    socklen_t clnt_addr_size;
    struct sockaddr_in serve_addr, clnt_addr;
    char buffer[MAXLINE];
    int opt = 0;

    bzero(&serve_addr, sizeof(serve_addr));
    bzero(&clnt_addr, sizeof(clnt_addr));
    memset(buffer, '\0', sizeof(buffer));

    //创建socket
    serve_sock =  socket(AF_INET, SOCK_STREAM, 0);

    //绑定协议族
    serve_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //serve_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serve_addr.sin_family = AF_INET;
    serve_addr.sin_port = htons(SERVE_PORT);
    bind(serve_sock, (SA *)&serve_addr, sizeof(serve_addr));

    //开始监听
    listen(serve_sock, 100);

    //处理请求
    while(1) {
        clnt_addr_size = sizeof(clnt_addr);

        printf("start accept...\n");
        clnt_sock = accept(serve_sock, (SA *)&clnt_addr, &clnt_addr_size);
    
        if (clnt_sock < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                printf("accept error\n");
                exit(-1);
            }
        }
        
        printf("block on here\n"); 
        opt = fcntl(clnt_sock, F_GETFL, 0);
        fcntl(clnt_sock, F_SETFL, opt | O_NONBLOCK); 
        read(clnt_sock, buffer, MAXLINE); 
        printf("content: %s\n", buffer);
        close(clnt_sock);
    }
    
    close(serve_sock);
    return 0;
}
