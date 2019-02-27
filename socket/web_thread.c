#include "socket_func.c"

#define SERV "80"
#define MAXFILES 20

struct file {
    char *f_name;
    char *f_hsot;
    int f_fd;
    int f_flags;
    pthread_t f_tid;
} file[MAXFILES];

#define F_CONNECTION 1
#define F_READING 2
#define F_DONE 4

#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

int nconn, nfiles, nlefttoconn, nlefttoread;

void *to_get_read(void *);
void home_page(const char *, const char *);
void write_get_cmd(struct file *);

int main(int argc, char **argv)
{
    int i, n, maxnconn;

    pthread_t tid;
    struct file *fptr;

    if (argc < 5) {
        printf("argc error\n");
        exit(-1);
    }

    maxnconn = 10;
    nfiles = MAXFILES;

    for (i = 0; i < nfiles; i++) {
        file[i].f_name = argv[i + 4];
    }

    return 0;
}

void home_page(const char *host, const char *fname)
{
    int fd, n;
    char line[MAXLINE];

    int sockfd;
    char ip[] = "127.0.0.1";
    struct sockaddr_in serve_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serve_addr, sizeof(serve_addr));
    serve_addr.sin_port = htons(SERV);
    serve_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &serve_addr.sin_addr);

    if (connect(sockfd, (SA*) &serve_addr, sizeof(serve_addr)) == -1) {
        printf("%s\n", "connect error");
        exit(-1);
    }

    n = snprintf(line, sizeof(line), GET_CMD, fname);
    written(fd, line, n);

    while (1) {
        if ((n = read(fd, line, MAXLINE)) == 0)
            break;

        printf("read %d bytes of home page\n", n);
    }

    printf("end-of-file on home page\n");
    close(fd);
}

void write_get_cmd(struct file *fptr)
{
    int n;
    char line[MAXLINE];

    n = snprintf(line, sizeof(line), GET_CMD);
    written(fptr->f_fd, line, n);
    printf("wrote %d bytes for %s\n", n, fptr->f_name);

    fptr->f_flags = F_READING;
}
