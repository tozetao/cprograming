#include "socket_func.c"
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#define SERV "80"
#define MAXFILES 20

struct file {
    char *f_name;
    char *f_host;
    int f_fd;
    int f_flags;
    pthread_t f_tid; 
} file[MAXFILES];

#define F_CONNECTION 1
#define F_READING 2
#define F_DONE 4

#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

int nconn, nfiles, nlefttoconn, nlefttoread;

void *do_get_read(void *);
void home_page(const char *, const char *);
void start_connect(struct file *);
void write_get_cmd(struct file *);
int tcp_connect(const char *, const char *);
struct addrinfo *host_serv(const char *, const char *, int, int);

// 非阻塞connect web客户端
// 命令行：./a.out 1 192.168.0.155 /home.html /a.html /b.html /c.html
int main(int argc, char **argv)
{
    int i, n, maxnconn;
    char buf[MAXLINE];
    
    pthread_t tid;
    struct file *fptr;
    
    if (argc < 4) {
        printf("argc error\n");
        exit(-1);
    }
    
    // 最大同时进行的连接
    maxnconn = atoi(argv[1]);
    // 请求的文件数
    nfiles = min(argc - 4, MAXFILES);

    // nlefttoconn是尚无TCP连接的文件数
    // maxnconn是最大并发数
    nlefttoread = nlefttoconn = nfiles;

    // nconn是当前打开着的连接数
    nconn = 0;

    for (i = 0; i < nfiles; i++) {
        file[i].f_name = argv[i + 4];
        file[i].f_flags = 0;
        file[i].f_host = argv[2];
    }
    printf("nfiles = %d\n", nfiles);

    // 请求home文件，参数2是host，参数3是home文件名
    home_page(argv[2], argv[3]);

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
                exit(-1);
            }

            file[i].f_flags = F_CONNECTION;
            pthread_create(&tid, NULL, &do_get_read, &file[i]);
            file[i].f_tid = tid;

            // 当前连接数加1
            nconn++;
            // 尚无建立TCP连接的文件数-1
            nlefttoconn--;
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

void *do_get_read(void *vptr)
{
    int    fd, n;
    char   line[MAXLINE];
    struct file *fptr;

    fptr = (struct file *)vptr;
    fd = tcp_connect(fptr->f_host, SERV);
    fptr->f_fd = fd;

    printf("do_get_read for %s, fd %d, thread %ld\n", fptr->f_name, fptr->f_fd, fptr->f_tid);

    write_get_cmd(fptr);

    while (1) {
        if ((n = read(fd, line, MAXLINE)) == 0) 
            break;
        printf("read %d bytes from %s\n", n, fptr->f_name);
    }
    
    printf("end-of-file on %s\n", fptr->f_name);
    close(fd);
    fptr->f_flags = F_DONE;
    return fptr;
}

void write_get_cmd(struct file *fptr)
{
    int    n;
    char   line[MAXLINE];

    n = snprintf(line, sizeof(line), GET_CMD, fptr->f_name);
    written(fptr->f_fd, line, n);
    printf("wrote %d bytes for %s\n", n, fptr->f_name);

    fptr->f_flags = F_READING;
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
