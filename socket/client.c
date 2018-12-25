#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

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
    result = write(sock, "fuck", 5);
    printf("result: %d\n", result);
    if (result == -1) {
        printf("write error");
        exit(-1);
    }
    
    int i;
    for(i = 0; i<3; i++) {
        result = write(sock, "cao", 4);
        printf("result: %d\n", result);
    
        if (result == -1) {
            printf("write error");
            exit(-1);
        }
    }
   
    read(sock, buffer, 100);
    printf("read from server: %s\n", buffer);
//    while(1) {
//        scanf("%s", buffer);
//        printf("receive: %s\n", buffer);
//
//        result = write(sock, buffer, sizeof(buffer));
//        if (result == -1) {
//            printf("write error");
//            exit(-1);
//        }
//        memset(buffer, 0, 1024);
//        // read(sock, buffer, 100);
//        // printf("read from server: %s\n", buffer);
//    }

    close(sock);

    return 0;
}
