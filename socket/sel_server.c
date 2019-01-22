#include "socket_func.c"
#include "sum.h"

#include <signal.h>
#include <wait.h>

void str_echo(int sock) {
    ssize_t n;

    //buffer是重复写的，因此除了接收客户端的数据外，也可能包含上一次接收未被覆盖到的数据，
    //所以输出内容的时候会产生多余的内容或乱码
    char buffer[MAXLINE];
    
    again:
        while((n = readline(sock, buffer, MAXLINE)) > 0) {
            printf("read %d bytes from client\n", (int)n);
            written(sock, buffer, n);
        }
        printf("read end: %d\n", (int)n);

        if (n < 0 && errno == EINTR)
            goto again;
        else if (n < 0) {
            printf("%s\n", "str_echo error");
        }
}

void str_echo_num(int sock)
{
    char buffer[MAXLINE];
    ssize_t n;
    long arg1, arg2;

again:
    while ((n = readline(sock, buffer, MAXLINE)) > 0 ) {
        printf("read %ld bytes from client\n", n);
        
        if (sscanf(buffer, "%ld %ld", &arg1, &arg2) == 2) {
            snprintf(buffer, sizeof(buffer), "%ld\n", arg1 + arg2);
        } else {
            snprintf(buffer, sizeof(buffer), "%s\n", "input error");
        }
        written(sock, buffer, strlen(buffer));
    }

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        printf("echo error\n");
}

void sig_child(int signo)
{
    pid_t pid;
    int stat;
    
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {    
        printf("pid = %d, stat = %d, signo = %d\n", pid, stat, signo);
    }
    return;
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
    //serve_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serve_addr.sin_family = AF_INET;
    serve_addr.sin_port = htons(SERVE_PORT);
    bind(serve_sock, (SA *)&serve_addr, sizeof(serve_addr));

    //注册信号处理函数
    signal(SIGCHLD, sig_child);

    //开始监听
    listen(serve_sock, 100);

    //处理请求
    while(1) {
        clnt_addr_size = sizeof(clnt_addr);

        printf("start accept...\n");
        clnt_sock = accept(serve_sock, (SA *)&clnt_addr, &clnt_addr_size);
        printf("clnt_sock: %d, errno: %d, EINTR=%d\n", clnt_sock, errno, EINTR);
        if (clnt_sock < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                printf("accept error\n");
                exit(-1);
            }
        }

        if ((pid = fork()) == 0) {
            //子进程处理客户端连接
            //str_echo(clnt_sock);
            //str_echo_num(clnt_sock);
            number_echo(clnt_sock);

            printf("I am child; close clnt_sock on child; clnt_sock = %d\n", clnt_sock);           
            close(serve_sock);
            close(clnt_sock);
            exit(0);
        } 

        printf("I am parent, child pid is %d; clnt_sock is %d\n", pid, clnt_sock);
        close(clnt_sock);
    }
}
