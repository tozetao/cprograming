#include <sys/socket.h>
#include <arpa/inet.h>
#include "socket_func.c"

int main() {
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    int result;

    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(9001);
    serv_addr.sin_family = PF_INET;

    result = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (result == -1) {
        printf("connect error");
        exit(-1);
    }    

    char buffer[1024];
   
    read(sock, buffer, 100);
    printf("read from server: %s\n", buffer);

    close(sock);

    return 0;
}
