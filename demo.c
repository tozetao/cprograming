#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

struct User {
    char *name;
};

struct User test()
{
    char content[] = "hello";

    struct User u;

    u.name = content;

    return u;
}

struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype)
{
    int n;
    struct addrinfo hints, *res;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = AI_CANONNAME;
    
    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        return NULL;

    return res;
}

int main()
{
//    struct User user = test();
//   printf("%s\n", user.name);
//    char host[] = "192.168.0.155";
//    char serv[] = "80";
//
//    struct addrinfo *ai;
//    ai = host_serv(host, serv, 0, SOCK_STREAM);
//    printf("family: %d, protocal: %d, socktype: %d\n", ai->ai_family, ai->ai_protocol, ai->ai_socktype);

    printf("%d\n", rand() % 5);

    return 0;
}
