#ifndef SOCKET_H

#define SOCKET_H

int tcp_create(void *server_address, void *host, int port, int family);

int tcp_connect();

int tcp_listen(int socketfd, int backlog);

int tcp_accept(int socketfd, void *clent_address, int family);

/**
 * 当读取到指定的结束标记符时停止读取，
 * 如果一直读取不到结束标记符，那么最多读取size个字节。
 */
int read_end_of(char *buffer, int size, char *identifier);

#endif
