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
    int result;
    char buffer[1024];

    while(1) {
        clnt_sock = accept(sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        printf("current clnt_sock: %d\n", clnt_sock);
        
        sleep(5);

        result = read(clnt_sock, &buffer, 100);
        if (result == -1) {
            printf("read error");
            exit(-1);
        }
        printf("receive from client: %s\n", buffer);

        result = write(clnt_sock, &buffer, sizeof(buffer));
        if (result == -1) {
            printf("write error");
            exit(-1);
        }

        memset(&clnt_addr, 0, clnt_addr_size);
        close(clnt_sock);
    }

    return 0;
}
