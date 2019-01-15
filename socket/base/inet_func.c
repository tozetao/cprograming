#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int m_inet_pton(int family, const char *str, void *addrptr);

int main()
{
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));

    char ip[] = "112.25.90.170";

    inet_pton(AF_INET, ip, &serv_addr.sin_addr);
    printf("%d\n", (int) serv_addr.sin_addr.s_addr);

    struct in_addr in_val2;
    m_inet_pton(AF_INET, ip, &in_val2);
    printf("%d\n", (int) in_val2.s_addr);

    // test2
    struct in_addr in_val;
    struct in_addr temp;

    if (inet_aton(ip, &in_val)){
        memcpy(&temp, &in_val, sizeof(struct in_addr));
        printf("%d\n", (int)temp.s_addr);
        printf("%s\n", inet_ntoa(temp));
    }
    
    return 0;
}

int m_inet_pton(int family, const char *str, void *addrptr)
{
    if (family == AF_INET) {
        struct in_addr int_val;
        if (inet_aton(str, &int_val)) {
            memcpy(addrptr, &int_val, sizeof(struct in_addr));
            return 1;
        }
    }
    errno = EAFNOSUPPORT;
    return -1;
}
