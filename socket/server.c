#include "socket_func.c"

#define MAXLINE 1024

void str_echo(int sock) {
    ssize_t n;
    char buffer[MAXLINE];

    again:
        while((n = read(sock, buffer, MAXLINE) > 0)) {
            written(sock, buffer, MAXLINE);
        }

        if (n < 0 && errno == EINTR)
            goto again;
        else if (n < 0) {
            err_printf("str_echo read error");
        }
}

int main() 
{
    int serve_sock, clnt_sock, pid, clnt_addr_size;
    struct sockaddr_in serve_addr, clnt_addr;

    bzero(&serve_addr, sizeof(serve_addr));
    bzero(&clnt_addr, sizeof(clnt_addr));

    //创建socket
    serve_sock =  socket(AF_INET, SOCK_STREAM, 0);

    //绑定协议族
    serve_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serve_addr.sin_family = AF_INET;
    serve_addr.sin_port = htons(9001);
    bind(serve_sock, (SA *)&serve_addr, sizeof(serve_addr));

    //开始监听
    listen(serve_sock, 100);

    //处理请求
    while(1) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serve_sock, (SA *)&clnt_addr, &clnt_addr_size);

        if ((pid = fork()) != 0) {
            //子进程处理客户端连接
            str_echo(clnt_sock);
            
            close(serve_sock);
            close(clnt_sock);
            exit(0);
        } else {
            close(clnt_sock);
        }
    }
}
