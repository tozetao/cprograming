#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    int sock;
    
    // 创建socket
    sock = socket(PF_INET, SOCK_STREAM, 0);

    // 初始化IP与端口信息
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(9001);

    // 将套接字与端口和IP进行绑定
    bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // 进入监听状态
    listen(sock, 100);

    // 处理请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock;
    char ip[10];
    short port;

    while(1) {
        clnt_sock = accept(sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        inet_ntop(AF_INET, &clnt_addr.sin_addr, ip, sizeof(ip));
        port = ntohs(clnt_addr.sin_port);

        printf("current clnt_sock: %d\n", clnt_sock);
        printf("connection from %s, port: %hd\n", ip, port);
        
        //响应客户端数据
        write(clnt_sock, "hello", 6);

        memset(&clnt_addr, 0, clnt_addr_size);
        close(clnt_sock);
    }

    return 0;
}
