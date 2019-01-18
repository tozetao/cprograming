#include "socket_func.c"
//#include "sigfunc.c"
#include <signal.h>
#include <wait.h>

void str_echo(int sock) {
    ssize_t n;

    //buffer是重复写的，因此除了接收客户端的数据外，也可能包含上一次接收未被覆盖到的数据，
    //所以输出内容的时候会产生多余的内容或乱码
    char buffer[MAXLINE];
    
    again:
        while((n = read(sock, buffer, MAXLINE)) > 0) {
            printf("read %d bytes from client\n", (int)n);
            written(sock, buffer, MAXLINE);
        }
        printf("read end: %d\n", (int)n);

        if (n < 0 && errno == EINTR)
            goto again;
        else if (n < 0) {
            printf("%s\n", "str_echo error");
        }
}

void sig_child(int signo)
{
    pid_t pid;
    int stat;
    
    pid = wait(&stat);
    
    printf("pid = %d, stat = %d, signo = %d\n", pid, stat, signo);
    return;
}

void sig_int(int signo)
{
    printf("signo %d\n", signo);
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

    //开始监听
    listen(serve_sock, 100);

    //注册信号处理函数
    if (signal(SIGCHLD, sig_child) == SIG_ERR) {
        printf("%s\n", "signal error");
        exit(-1);
    }

    signal(SIGINT, sig_int);

    //处理请求
    while(1) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serve_sock, (SA *)&clnt_addr, &clnt_addr_size);
        printf("client socket %d\n", clnt_sock);

        if ((pid = fork()) != 0) {
            printf("child process id = %d, then doing something\n", pid);
            //子进程处理客户端连接
            str_echo(clnt_sock);

            printf("%s; clnt_sock = %d\n", "close clnt_sock on child", clnt_sock);           
            close(serve_sock);
            close(clnt_sock);
            exit(0);
        } else {
            printf("%s; clnt_sock = %d\n", "close clnt_sock on parent", clnt_sock);
            close(clnt_sock);
        }
    }
}
