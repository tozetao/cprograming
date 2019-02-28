#include "socket_func.c"
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>

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

// 非阻塞connect web客户端
// 命令行：./a.out 1 192.168.0.155 /home.html /a.html /b.html /c.html
int main(int argc, char **argv)
{
    int i, n, fd, maxnconn, flags, error;
    char buf[MAXLINE];
    fd_set rs, ws;

    if (argc < 4) {
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

    FD_ZERO(&rset);
    FD_ZERO(&wset);
    maxfd = -1;

    // nlefttoconn是尚无TCP连接的文件数
    // nconn是当前打开着的连接数
    // maxnconn是最大并发数
    nlefttoread = nlefttoconn = nfiles;
    nconn = 0;

    /* 
        nlefttoread是仍需读取的文件数，初始时等于要读取的文件个数，
        当到达0时程序任务完成，在循环中select每次处理好一个文件，则会减1
    */
    while (nlefttoread > 0) {
        // 如果没有到达最大并行连接数，而且还有连接需要建立，那就找到一个未处理的文件，f_flags=0的文件
        // 然后调用start_connect另发起一个连接
        
        // nconn和maxnconn俩个变量控制并发数，nlefttoconn控制是否要开始一个连接。
        while (nconn < maxnconn && nlefttoconn > 0) {
            for (i = 0; i < nfiles; i++) 
                if (file[i].f_flags == 0)
                    break;

            // 所有文件都处理中
            if (i == nfiles) {
                printf("nlefttoconn = %d but nothing found\n", nlefttoconn);
                sleep(5);
                exit(-1);
            }

            start_connect(&file[i]);
            // 当前连接数加1
            nconn++;
            // 尚无建立TCP连接的文件数-1
            nlefttoconn--;
        }

        rs = rset;
        ws = wset;
        n = select(maxfd + 1, &rs, &ws, NULL, NULL);

        // 处理I/O事件
        for (i = 0; i < nfiles; i++) {
            flags = file[i].f_flags;
            if (flags == 0 || flags & F_DONE)
                continue;

            fd = file[i].f_fd;
            
            // 处理刚建立连接的文件
            if (flags & F_CONNECTION && (FD_ISSET(fd, &rs) || FD_ISSET(fd, &ws))) {
                n = sizeof(error);

                if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &n) < 0 || error != 0) {
                    printf("nonblocking connect failed for %s", file[i].f_name);
                    exit(-1);
                }

                printf("connection established for %s\n", file[i].f_name);
                FD_CLR(fd, &wset);
                write_get_cmd(&file[i]);
            } else if (flags & F_READING && FD_ISSET(fd, &rs)) {
                // 连接建立成功，处理服务器相应数据
                if ((n = read(fd, buf, sizeof(buf))) == 0) {
                    printf("end-of-file on %s\n\n", file[i].f_name);
                    close(fd);
                    file[i].f_flags = F_DONE;
                    FD_CLR(fd, &rset);
                    nconn--;
                    nlefttoread--;
                } else {
                    printf("read %d bytes from %s\n", n, file[i].f_name);
                }
            }
        }
    }

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

    printf("end-of-file on home page \n\n");
    close(fd);
}

// 建立一个连接
void start_connect(struct file *fptr)
{
    int fd, flags, n;
    struct addrinfo *ai;

    ai = host_serv(fptr->f_host, SERV, 0, SOCK_STREAM);
    
    fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    fptr->f_fd = fd;
    printf("start_connect for %s, fd %d\n", fptr->f_name, fd);

    flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    // 非阻塞下会直接返回一个错误
    if ((n = connect(fd, ai->ai_addr, ai->ai_addrlen)) < 0) 
    {
        if (errno != EINPROGRESS) {
            printf("nonblocking connect error\n");
            exit(-1);
        }
        // 将该文件结构标志设置连接中，然后监听描述符的I/O事件
        fptr->f_flags = F_CONNECTION;
        FD_SET(fd, &rset);
        FD_SET(fd, &wset);

        if (fd > maxfd)
            maxfd = fd;
    } else if (n >= 0)
        // 在局域网下，RTT的时间非常短暂，尽快描述符设置了非阻塞，然而也可能立即建立成功
        // 因此要处理连接成功的情况
        write_get_cmd(fptr);
}

void write_get_cmd(struct file *fptr)
{
    int    n;
    char   line[MAXLINE];

    n = snprintf(line, sizeof(line), GET_CMD, fptr->f_name);
    written(fptr->f_fd, line, n);
    printf("wrote %d bytes for %s\n", n, fptr->f_name);

    fptr->f_flags = F_READING;

    // 监听描述符的可读事件
    FD_SET(fptr->f_fd, &rset);

    if (fptr->f_fd > maxfd)
        maxfd = fptr->f_fd;
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
