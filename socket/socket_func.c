#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

//将网络字节序转换成IPv4地址
char *sock_ntop(const struct sockaddr *sa, socklen_t salen);

int main()
{
    return 0;
}

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

        return str;
    }
    
    return NULL;
}
