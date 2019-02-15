#include <pthread.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <fcntl.h>
#include "common.c"

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

// 将文件描述符设为非阻塞的
int set_non_blocking(int fd) 
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

// 将文件描述符上的EPOLLIN事件注册到epollfd指向的epoll内核事件中，enable_et指定fd参数是否启用ET模式
void addfd(int epollfd, int fd, short enable_et)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
     
    if (enable_et) {
        event.events |= EPOLLET;
    }

    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    set_non_blocking(fd);
}

// lt模式的工作流程
void lt(struct epoll_event *events, int number, int epollfd, int listenfd)
{
    char buffer[BUFFER_SIZE];
    int i, sockfd;

    for(i = 0; i < number; i++) {
        sockfd = events[i].data.fd;
        if (sockfd == listenfd) {
            struct sockaddr_in client_address;
            socklen_t client_len = sizeof(client_address);
            int connfd = accept(listenfd, (SA *)&client_address, &client_len);
            addfd(epollfd, connfd, 0);
        } else if (events[i].events & EPOLLIN) {
            // 在这里故意将缓冲区大小设置为10个字节，当发送过来的数据超过10个字节，这段代码仍然会被触发
            // socket只要有数据就会一直触发EPOLLIN事件
            printf("event trigger once\n");
            memset(buffer, '\0', BUFFER_SIZE);

            int ret = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
            if (ret <= 0) {
                close(sockfd);
                printf("close %d fd\n", sockfd);
                continue;
            }
            printf("get %d bytes of content: %s\n", ret, buffer);
        } else {
            printf("something else happened\n");
        }
    }
}

void et(struct epoll_event *events, int number, int epollfd, int listenfd)
{
    char buffer[BUFFER_SIZE];
    int i, sockfd, connfd, ret;

    for(i = 0; i < number; i++) {
        sockfd = events[i].data.fd;

        if (sockfd == listenfd) {
            struct sockaddr_in client_address;
            socklen_t client_len = sizeof(client_address);
            connfd = accept(listenfd, (SA *)&client_address, &client_len);
            // 使用ET模式
            addfd(epollfd, connfd, 1);
        } else if (events[i].events & EPOLLIN) {
            // 这段代码只会触发一次，因此需要把socket输入缓冲区中的数据读取完毕
            printf("event trigger onec\n");
            // 读取一个字节数据，让客户端写入俩次数据，查看EPOLLIN事件的触发情况
            recv(connfd, buffer, 1, 0);
            printf("content: %s\n", buffer);

//            while(1) {
//                memset(buffer, '\0', BUFFER_SIZE);

                // epoll_wait监视的文件描述符如果触发了事件，那么该文件描述符的输入缓冲区一定会有数据，
                // 即使socket设置为非阻塞，也是一定会读取到数据的。如果读取不到数据，且errno等于EAGAIN，
                // 则表示数据读取完毕。
                // ret = recv(connfd, buffer, BUFFER_SIZE - 1, 0);

//                if (ret < 0) {
//                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                        printf("read later\n");
//                        break;
//                    }
//                    printf("ret < 0, close connfd: %d\n", connfd);
//                    close(connfd);
//                } else if (ret == 0) {
//                    // 客户端发送FIN报文段，这里可以做一些清理工作
//                    printf("ret == 0, close connfd: %d\n", connfd);
//                    close(connfd);
//                    break;
//                } else {
//                    printf("get %d bytes of content: %s\n", ret, buffer);
//                }
//            }
        } else {
            printf("something else heppened\n");
        }
    }
}

int main()
{
    const char *ip = "127.0.0.1";
    int port = 9001, ret;

    // 初始化socket地址
    struct sockaddr_in serv_address;
    bzero(&serv_address, sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port);
    //inet_pton(AF_INET, ip, (void *)&serv_address.sin_addr);
    serv_address.sin_addr.s_addr = htonl(INADDR_ANY);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    
    ret = bind(listenfd, (SA *)&serv_address, sizeof(serv_address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    struct epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);

    addfd(epollfd, listenfd, 1);

    while(1) {
        // 检测是否有带处理的文件描述符
        int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if (ret < 0) {
            printf("epoll failure\n");
            break;
        }

        // 通过切换俩种模式，查看不同模式下事件的触发次数
        //lt(events, ret, epollfd, listenfd);
        et(events, ret, epollfd, listenfd);
    }

    close(listenfd);
    return 0;
}


/*
1. 为什么ET模式下，socket文件描述符必须时非阻塞的?    
在ET模式下使用epoll_wait()时，文件描述符必须是非阻塞的，这是为了避免阻塞读取使多个文件描述符处于饥饿状态，
即阻塞到其他文件描述符了。
*/
