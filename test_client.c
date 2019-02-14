//客户端
#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/time.h>

int main() 
{ 
    int client_sockfd; 
    int len; 
    struct sockaddr_in address;//服务器端网络地址结构体 

    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);//建立客户端socket 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8888); 
    len = sizeof(address); 

    if(connect(client_sockfd, (struct sockaddr *)&address, len)) { 
         perror("oops: client2"); 
         exit(-1); 
    } 
    
    char content[] = "hello world!";
    char receive_buffer[1024];

    write(client_sockfd, content, sizeof(content)); 

    read(client_sockfd, receive_buffer, 1024);
    printf("%s\n", receive_buffer);

    close(client_sockfd); 
    return 0; 
}
