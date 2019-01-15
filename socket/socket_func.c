#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>

#define SA struct sockaddr
#define MAXLINE 1024
#define SERVE_PORT 9001

//网络字节序转换成IPv4地址字符串
char *sock_ntop(const struct sockaddr *sa)
{
    char post_str[8];
    static char str[128];

    if (sa->sa_family == AF_INET) {
        struct sockaddr_in *sin = (struct sockaddr_in*)sa;

        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
            return NULL;

        if (ntohs(sin->sin_port) != 0) {
            snprintf(post_str, sizeof(post_str), ":%d", ntohs(sin->sin_port));
            strcat(str, post_str);
        }

        return str;
    }
    
    return NULL;
}

//读取一定字节数据
ssize_t readn(int fd, void *vpstr, size_t n)
{
    size_t left = n;
    ssize_t read_size;
    char *buffer;

    buffer = (char *)vpstr;

    while (left > 0) {
        if ((read_size = read(fd, buffer, left)) < 0) {
            if (errno == EINTR) 
                read_size = 0;
            else
                return -1;
        } else if (read_size == 0){
            return 0;
        }
        left = left - read_size;
        buffer = buffer + read_size;
    }

    return n - left;
}

//向socket写入一定字节数据
ssize_t written(int sock, void *vpstr, ssize_t size)
{
    size_t write_size;    
    ssize_t left = size;
    char *buffer = (char *)vpstr;
    
    while (left > 0) {
        if ((write_size = write(sock, vpstr, left) <= 0)) {
            if (write_size < 0 && errno == EINTR) {
                write_size = 0;
            } else {
                return -1;
            }
        }
        left -= write_size;
        buffer += write_size;
    }

    return size - left;
}

void err_printf(char *str)
{
    printf("%s\n", str);
    exit(-1);
}
