#include "socket_func.c"
#include <netdb.h>

#define SERV "80"
#define MAXFILES 20

struct file {
    char *f_name;
    char *f_host;
    int f_fd;
    int f_flags;
} file[MAXFILES];

#define F_CONNECTION 1
#define F_READING 2
#define F_DONE 4

#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

int nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
fd_set rset, wset;

void home_page(const char *, const char *);
void start_connect(struct file *);
void write_get_cmd(struct file *);
int tcp_connect(const char *, const char *);
struct addrinfo *host_serv(const char *, const char *, int, int);

// web 3 127.0.0.1 file....
int main(int argc, char **argv)
{
    int i, n, , fd, maxnconn, flags, error;
    char buf[MAXLINE];
    fd_set rs, ws;

    if (argc < 5) {
        printf("argc error\n");
        exit(-1);
    }
    
    // 最大同时进行的连接
    maxnconn = atoi(argv[1]);
    // 请求的文件数
    nfiles = min(argc - 4, MAXFILES);

    for (i = 0; i < nfiles; i++) {
        file[i].f_name = argv[i + 4];
        file[i].f_flags = 0;
        file[i].f_host = argv[2];
    }
    printf("nfiles = %d\n", nfiles);

    // 请求home文件，参数2是host，参数3是home文件名
    home_page(argv[2], argv[3]);

    FD_ZERO($rset);
    FD_ZERO(&wset);
    maxfd = -1;

    // nlefttoread是仍需读取的文件数，当到达0时程序任务完成
    // nlefttoconn是尚无TCP连接的文件数
    // nconn是当前打开着的连接数
    nlefttoread = nlefttoconn = nfiles;
    nconn = 0;

    return 0;
}

void home_page(const char *host, const char *fname)
{
    int fd, n;
    char line[MAXLINE];

    fd = tcp_connect(host, SERV);

    n = snprintf(line, sizeof(line), GET_CMD, fname);
    written(fd, line, n);

    while (1) {
        if ((n = read(fd, line, MAXLINE)) == 0)
            break;

        printf("read %d bytes of home page\n", n);
    }

    printf("end-of-file on home page \n");
    close(fd);
}

void start_connect(struct file *fptr)
{
    int fd, flags, n;
    struct addrinfo *ai;

    ai = host_serv(fptr->f_host, SERV, 0, SOCK_STREAM);
    fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

    printf("start_connect for %s, fd %d\n", fptr->f_name, fd);

    flags = fcntl();
}

int tcp_connect(const char *host, const char *port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serve_addr;
    bzero(&serve_addr, sizeof(serve_addr));
    serve_addr.sin_port = htons(atoi(port));
    serve_addr.sin_family = AF_INET;
    inet_pton(AF_INET, host, &serve_addr.sin_addr);
    
    connect(sockfd, (SA*) &serve_addr, sizeof(serve_addr));

    return sockfd;
}

struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype)
{
    int n;
    struct addrinfo hints, *res;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = AI_CANONNAME;
    
    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        return NULL;

    return res;
}
