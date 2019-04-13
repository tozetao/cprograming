#include <sys/socket.h>
#include <stdlib.h>

#include "socket.h"

int tcp_create(void *address, void *host, int port, int family)
{
    struct sockaddr_in *server_address = (struct sockaddr_in *) address;
    int socketfd;

    //IPv4协议
    if (family == AF_INET) {
        socketfd = socket(family, SOCK_STREAM, 0);

        bzero(server_address, sizeof(server_address));

        server_address->sin_addr.s_addr = htonl(INADDR_ANY);
        

        return socketfd;
    }

    return -1;
}
