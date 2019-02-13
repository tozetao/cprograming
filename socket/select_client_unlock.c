#include "socket_func.c"

#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>

void sig_pipe(int signo)
{
    printf("signo = %d\n", signo);
    return;
}

void str_cli(FILE *file, int sockfd)
{
    int maxfd, nready;
    fd_set rset, allset, wset;
    char sendline[MAXLINE], recvline[MAXLINE];

    FD_ZERO(&rset);
    FD_ZERO(&allset);

    FD_SET(fileno(file), &allset);
    FD_SET(sockfd, &allset);

    // 监听socket是否可写
    FD_SET(sockfd, &wset);

    while (1) {
        rset = allset;
        maxfd = max(fileno(file), sockfd) + 1;
        nready = select(maxfd, &rset, &wset, NULL, NULL);

        printf("nready = %d\n", nready);

        // 如果读取到服务端响应的数据，则进行输出
        if (FD_ISSET(sockfd, &rset)) {
            if (readline(sockfd, recvline, MAXLINE) == 0) 
                printf("str_cli: server is prematurely terminated\n");
            else
                fputs(recvline, stdout);
        }

        // 当前标准输入有数据
        if (FD_ISSET(fileno(file), &rset)) {
            if (fgets(sendline, MAXLINE, file) == NULL)
                return;

            written(sockfd, sendline, strlen(sendline));
        }

        // 这里只是测试什么情况下套接字是可写的
        if (FD_ISSET(sockfd, &wset)) {
            printf("%d file description is writable\n", sockfd);
            sleep(1);
        }
    }
}

/*
在非阻塞模式下，读与写的系统调用可能会执行返回EWOULDBLOCK错误，
因此程序需要考虑这种情况的发生。


在非阻塞IO中，读与写不是同步进行的，可能你读取成功，然后写入失败。
就像在从标准输入中读取数据，然后写入到socket中的过程，如果在写入socket过程中失败了，该如何处理?

to缓冲区的处理
    如果读取成功，to缓冲区中会有数据。如果socket写入成功，就相当于阻塞IO的流程，如果socket写入失败，
对于非阻塞产生的错误，会暂时忽略它。

    由于缓冲区有需要写入的数据，因此在下一次的循环中，
    会从新让select监听socket的可写情况，所以仍然会触发一次写入操作，直到缓冲区中的数据被写入成功。

    同样的，如果to缓冲区中没有可以存储的空间，socket的缓冲区将不会被处理，直到有空间存储时，
    select会再次触发可读事件，从而将数据读取到to缓冲区中。

from缓冲区的处理（socket）
    与to缓冲区的处理方式相似

*/
void unlock_str_cli(int sockfd)
{
    int maxfd, val, stdineof, nready;
    ssize_t  n, nwritten;

    char to[MAXLINE], fr[MAXLINE];
    char *toiptr, *tooptr, *friptr, *froptr;

    fd_set rset, wset;
    
    // 将sockfd，标准输入和标准输出设置为非阻塞
    val = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, val | O_NONBLOCK);

    val = fcntl(STDOUT_FILENO, F_GETFL, 0);
    fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);

    val = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);

    toiptr = tooptr = to;
    friptr = froptr = fr;
    stdineof = 0;

    maxfd = max(max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;

    while(1) {
        FD_ZERO(&rset);
        FD_ZERO(&wset);

        // 俩个缓冲区有空间可以放置读取数据
        if (stdineof == 0 && toiptr < &to[MAXLINE])
            FD_SET(STDIN_FILENO, &rset);

        if (friptr < &fr[MAXLINE])
            FD_SET(sockfd, &rset);

        // 俩个缓冲区有可以写入的数据
        if (tooptr != toiptr)
            FD_SET(sockfd, &wset);

        if (friptr != froptr)
            FD_SET(STDOUT_FILENO, &wset);

        nready = select(maxfd, &rset, &wset, NULL, NULL);
        printf("nready = %d\n", nready);

        // 从标准输入读入数据
        if (FD_ISSET(STDIN_FILENO, &rset)) {
            if ((n = read(STDIN_FILENO, toiptr, &to[MAXLINE] - toiptr)) < 0) {
                if (errno != EWOULDBLOCK) {
                    printf("read error on stdin\n");
                    exit(-1);
                }
            } else if (n == 0) {
                fprintf(stderr, "EOF on stdin\n");
                stdineof = 1;
                if (tooptr == toiptr)
                    shutdown(sockfd, SHUT_WR);
            } else {
                fprintf(stderr, "read %ld bytes from stdin\n", n);
                toiptr += n;
                FD_SET(sockfd, &wset);
            }
        }

        // 从sockfd读入数据
        if (FD_ISSET(sockfd, &rset)) {
            if ((n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0) {
                if (errno != EWOULDBLOCK) {
                    printf("read error on sockfd\n");
                    exit(-1);
                }
            } else if (n == 0) {
                // 如果EOF来自服务器，需要判断是否正常结束；如果标准输入碰到EOF，
                // 那么来自服务器的EOF就是正常的，否则是异常的。
                fprintf(stderr, "EOF on socket\n");
                if (stdineof)
                    return;
                else {
                    printf("str_cli: server terminated prematurely\n");
                    exit(-1);
                }
            } else {
                fprintf(stderr, "read %ld bytes from socket\n", n);
                friptr += n;
                FD_SET(STDOUT_FILENO, &wset);
            }
        }

        // 处理标准输出
        if (FD_ISSET(STDOUT_FILENO, &wset) && ((n = friptr - froptr) > 0)) {
            if ((nwritten = write(STDOUT_FILENO, froptr, n)) < 0) {
                if (errno != EWOULDBLOCK) {
                    printf("write error to stdout\n");
                    exit(-1);
                }
            } else {
                fprintf(stderr, "wrote %ld bytes to stdout\n", nwritten);
                froptr += nwritten;
                if (froptr == friptr)
                    froptr = friptr = fr;
            }
        }

        // 处理socket输出
        if (FD_ISSET(sockfd, &wset) && ((n = toiptr - tooptr) > 0)) {
            if ((nwritten = write(sockfd, tooptr, n)) < 0) {
                if (errno != EWOULDBLOCK) {
                    printf("write error to sockfd\n");
                    exit(-1);
                }
            } else {
                fprintf(stderr, "wrote %ld bytes to sockfd\n", nwritten);
                tooptr += nwritten;
                if (tooptr == toiptr) {
                    toiptr = tooptr = to;
                    if (stdineof)
                        shutdown(sockfd, SHUT_WR);
                }
            }
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

    unlock_str_cli(sockfd);
    //str_cli(stdin, sockfd);

    exit(0);
}
