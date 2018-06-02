#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char const *argv[])
{
    /*
    int tcp_sock, upd_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);

    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    upd_sock = socket(PF_INET, SOCK_DGRAM, 0);

    printf("SOCK_STREM: %d\n", SOCK_STREAM);
    printf("SOCK_STREM: %d\n", SOCK_DGRAM);

    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
    if(state)
        error_handling("getsocket() error\n");
    printf("socket type one: %d\n", sock_type);

    state = getsockopt(upd_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
    if(state)
            error_handling("getsocket() error\n");
    printf("socket type one: %d\n", sock_type);
    */

    int sock;
    int send_buf = 1024 * 3, send_buf_tmp, rcv_buf = 1024 * 10, rcv_buf_tmp, state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    //设置输出缓冲区大小
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&send_buf, sizeof(send_buf));
    if(state == -1)
        error_handling("setsockopt error\n");

    //返回输出缓冲区大小
    len = sizeof(send_buf_tmp);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&send_buf_tmp, &len);
    if(state == -1)
        error_handling("setsockopt error\n");
    
    //设置输入缓冲区的大小
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf, sizeof(rcv_buf));
    if(state == -1)
        error_handling("setsockopt error\n");

    //返回输入缓冲区的大小
    len = sizeof(rcv_buf_tmp);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf_tmp, &len);
    if(state == -1)
        error_handling("getsockopt() error");

    printf("Input buffer size: %d \n", rcv_buf_tmp);
    printf("Output buffer size: %d \n", send_buf_tmp);
    
    return 0;
}


void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
