#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
#include <sys/ioctl.h> 
#include <unistd.h> 
#include <stdlib.h>

int main() 
{ 
    int server_sockfd, client_sockfd; 
    int server_len, client_len, n; 
    struct sockaddr_in server_address; 
    struct sockaddr_in client_address; 

    char buffer[1024];
    
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_address.sin_port = htons(8888); 
    server_len = sizeof(server_address); 
    
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len); 
    
    listen(server_sockfd, 5); 

    // 处理连接的请求
    client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

    // 读取数据
    n = read(client_sockfd, buffer, 1024);
    printf("read %d bytes from client, data: %s\n", n, buffer);

    write(client_sockfd, buffer, n);

    return 0;
}
