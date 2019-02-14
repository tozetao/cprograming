#include "common.c"

#define BUFFER_SIZE 1024

int main(int argc, char * argv[])
{
    const char ip[] = "127.0.0.1";
    int port = 9001;

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int recvbuf = atoi(argv[1]);
    int len = sizeof(recvbuf);

    // 设置TCP接受缓冲区大小
    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, sizeof(recvbuf));
    getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, (socklen_t *)&len);
    printf("the tcp receive buffer size is %d\n", recvbuf);

    int ret = bind(sock, (SA *)&server_address, sizeof(server_address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int clientfd;

    clientfd = accept(sock, (SA *)&client, &client_len);
    printf("connection fd is %d\n", clientfd);

    if (clientfd < 0) {
        printf("error is %d\n", errno);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);
    while(recv(clientfd, buffer, BUFFER_SIZE - 1, 0) > 0) {}
    close(clientfd);

    return 0;
}
