#include "common.c"

#define BUFFER_SIZE 512

int main(int argc, char* argv[])
{
    char ip[] = "127.0.0.1";
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));

    server_address.sin_port = htons(9001);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, (void *)&server_address.sin_addr);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    // 发送缓冲区大小
    int sendbuf, sendbuf_val;
    sendbuf = atoi(argv[1]);
    int len = sizeof(sendbuf);

    // 设置发送缓冲区大小
    setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, len);
    getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuf_val, (socklen_t *)&len);
    printf("the tcp send buffer size is %d\n", sendbuf_val);

    int ret = connect(sock, (SA *)&server_address, sizeof(server_address));
    if (ret == -1) {
        printf("connect error\n");
        return -1;
    }

    // 发送512字节数据
    char buffer[BUFFER_SIZE];
    memset(buffer, 'a', BUFFER_SIZE);
    send(sock, buffer, BUFFER_SIZE, 0);

    close(sock);

    return 0;
}
