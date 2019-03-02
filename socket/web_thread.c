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
#define F_JOINED 8

#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

int ndone = 0;
pthread_mutex_t ndone_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ndone_cond = PTHREAD_COND_INITIALIZER;

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
    int i, maxnconn;
    
    pthread_t tid;
    
    if (argc < 4) {
        printf("argc error\n");
        exit(-1);
    }
    
    // 最大连接数 
    maxnconn = atoi(argv[1]);

    // 请求的文件数
    nfiles = min(argc - 4, MAXFILES);

    // nlefttoconn是尚无TCP连接的文件数
    // nlefttoread是仍需读取的文件书
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

    // 是否有文件需要读取，nlefttoread可以认为是线程总数
    while (nlefttoread > 0) {
        // nlefttoconn是要发出请求的总数，nconn和maxconn俩个变量用于控制最大并发数(线程数)
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

        // 处理逻辑
        pthread_mutex_lock(&ndone_mutex);

        // 如果没有线程执行完毕，ndone等于0，程序将会阻塞在这里
        // 当有条件满足后，线程发送信号，程序将会再次执行，这是ndone不等于0，
        // 将退出循环；如果ndone不等于0，将直接执行回收线程的动作
        while (ndone == 0) {
            printf("block on condition variable\n");
            pthread_cond_wait(&ndone_cond, &ndone_mutex);
        }

        // 为什么要有ndone变量?
        // 我们需要这个变量，来知道是否有线程执行完毕，这样主线程就知道是否需要阻塞，
        // 还是不阻塞直接回收子线程。
        // 如果没有这个变量，可能在主线程执行之前有其他线程执行完毕了，这时候主线程仍然傻傻阻塞在这里不去回收子线程，这是不好的。
        // 互斥锁能够保持原子性操作，即当前线程获得互斥锁后，如果当前线程未执行完毕时，其他线程是没办法执行的(会被阻塞)

        for (i = 0; i < nfiles; i++) {
            if (file[i].f_flags & F_DONE) {
                pthread_join(file[i].f_tid, NULL);
                file[i].f_flags = F_JOINED;
                nconn--;
                nlefttoread--;
                ndone--;    //没有该变量，在回收完一个线程后，将导致主线程是轮询的，效率很低。
            }
        }

        pthread_mutex_unlock(&ndone_mutex);
        printf("nlefttoread = %d, ndone = %d\n", nlefttoread, ndone); 
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
    
    printf("end-of-file on %s\n\n", fptr->f_name);
    close(fd);
    fptr->f_flags = F_DONE;
    
    // 递增ndone，并发送信号到该条件变量
    pthread_mutex_lock(&ndone_mutex);
    ndone++;
    pthread_cond_signal(&ndone_cond);
    pthread_mutex_unlock(&ndone_mutex);

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
