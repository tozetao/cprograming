#include "common.c"

//网络字节序转换成IPv4地址字符串
char *sock_ntop(const struct sockaddr *sa)
{
    char post_str[8];
    static char str[128];

    if (sa->sa_family == AF_INET) {
        struct sockaddr_in *sin = (struct sockaddr_in*)sa;

        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
            return NULL;

        if (ntohs(sin->sin_port) != 0) {
            snprintf(post_str, sizeof(post_str), ":%d", ntohs(sin->sin_port));
            strcat(str, post_str);
        }

        return str;
    }
    
    return NULL;
}

//读取一定字节数据
ssize_t readn(int fd, void *vpstr, size_t n)
{
    size_t left = n;
    ssize_t read_size;
    char *buffer;

    buffer = (char *)vpstr;

    while (left > 0) {
        if ((read_size = read(fd, buffer, left)) < 0) {
            if (errno == EINTR) 
                read_size = 0;
            else
                return -1;
        } else if (read_size == 0){
            return 0;
        }
        left = left - read_size;
        buffer = buffer + read_size;
    }

    return n - left;
}

//向socket写入一定字节数据
ssize_t written(int sock, void *vpstr, ssize_t size)
{
    ssize_t write_size;    
    ssize_t left = size;
    char *buffer = (char *)vpstr;
    
    while (left > 0) {
        if ((write_size = write(sock, buffer, left)) <= 0) {
            if (write_size < 0 && errno == EINTR) {
                write_size = 0;
            } else {
                return -1;
            }
        }
        left -= write_size;
        buffer += write_size;
    }

    return size - left;
}

static int read_cnt;
static char *read_point;
static char read_buffer[MAXLINE];
//先把数据读取到静态缓冲区中，然后依次从该缓冲区中取出字节数据
ssize_t myread(int fd, char *c)
{
    //printf("read_cnt=%d\n", read_cnt);
    if (read_cnt <= 0) {
again:
        if ((read_cnt = read(fd, read_buffer, sizeof(read_buffer))) < 0) {
            if (errno == EINTR) 
                goto again;
            else
                return -1;
        } else if (read_cnt == 0) 
            return 0;

        read_point = read_buffer;
    }
    
    read_cnt--;
    *c = *read_point++;
    return 1;
}

//读取max个字节数据，如果碰到换行符或者EOF则停止读取
//readline是从read_buffer中读取数据的，碰到换行符则停止读取，下次调用继续从read_buffer中读取数据，
//这里有个问题，如果read_buffer读取的数据不干净，那么myread会从read_buffer中继续向后读取数据，这时候就会出错.
ssize_t readline(int fd, void *buffer, ssize_t max)
{
    ssize_t i, cnt = 0;
    char *pstr, c;

    pstr = (char *) buffer;
    for (i=0; i < max; i++) {
        if ((cnt = myread(fd, &c)) == 1) {
            *pstr++ = c;
            //printf("%c", c);
            if (c == '\n')
                break;
        } else if (cnt == 0) {
            *pstr = 0;
            return 0;   //EOF
        } else if (cnt == -1) {
            return -1;  //error
        }
    }
    //printf("max=%d, i=%d\n", (int)max, (int)i);

    *pstr = 0;
    read_cnt = 0;
    return i+1;
}

void err_printf(char str[])
{
    printf("%s\n", str);
    exit(-1);
}
