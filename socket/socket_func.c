#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>

//将网络字节序转换成IPv4地址

//从socket中读取一定字节数据

//int main()
//{
//    char *ip;
//    struct sockaddr_in server_addr;
//    memset(&server_addr, '0', sizeof(server_addr));
//
//    server_addr.sin_port = htons(8080);
//    server_addr.sin_family = AF_INET;
//    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    
//    ip = sock_ntop((struct sockaddr*)&server_addr, sizeof(server_addr));
//    printf("%s\n", ip);

//    return 0;
//}

char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
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
        printf("str: %s; post_str: %s\n", str, post_str);

        return str;
    }
    
    return NULL;
}

ssize_t readn(int fd, void *vpstr, size_t n)
{
    size_t left = n;
    ssize_t readSize;
    char *buffer;

    buffer = (char *)vpstr;

    while (left > 0) {
        if ((readSize = read(fd, buffer, left)) < 0) {
            if (errno == EINTR) 
                readSize = 0;
            else
                return -1;
        } else if (readSize == 0){
            return 0;
        }
        left = left - readSize;
        buffer = buffer + readSize;
    }

    return n - left;
}
