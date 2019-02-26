static pthread_key_t rl_key;
static pthread_once_t rl_once = PTHREAD_ONCE_INIT;

static void readline_destructor(void *val)
{
    free(val);
}

static void readline_once()
{
    pthread_key_create(&rl_key, readline_destructor);
}

typedef struct {
    int rl_cnt;
    char *rl_bufptr;
    char rl_buffer[MAXLINE];
} Rline;

/*
    从描述符中读取数据，首先会将数据读取到Rline结构中的缓冲中并返回缓存中第一个字符，
    之后每次调用my_read都是从缓存中逐个读取字符，直到没有数据读取。
*/
static ssize_t thread_read_one(Rline *tsd, int fd, char *c)
{
    if (tsd->rl_cnt <= 0) {
again:
        if ((tsd->rl_cnt = read(fd, tsd->rl_buffer, MAXLINE)) < 0) {
            if (errno == EINTR)
                goto again;
            else
                return -1;
        } else if (tsd->rl_cnt == 0) 
            return 0;

        // 将rl_bufptr指针指向buffer的起始位置
        tsd->rl_bufptr = tsd->rl_buffer;
    }

    tsd->rl_cnt--;
    *c = *tsd->rl_bufptr++;
    return 1;
}

// 读取一行数据或者最多读取到maxlen - 1个字节数据
static ssize_t thread_readline(int fd, void *buffer, size_t maxlen)
{
    Rline *tsd;
    
    ssize_t cnt = 0;
    int i, result;

    char c;
    char *pstr = (char *)buffer;

    // 在多线程下，要考虑并发性问题。
    pthread_once(&rl_once, readline_once);
    if (pthread_getspecific(rl_key) == NULL) {
        tsd = malloc(sizeof(Rline));
        pthread_setspecific(rl_key, tsd);
    }
    
//    thread_read_one(tsd, fd, &c);
//    printf("%c\n", c);
//    *pstr++ = c;
//    return 1;

    // 读取数据，直到碰到换行符或者读取了maxlen - 1个字节数据
    for (i = 1; i < maxlen; i++) {
        if ((result = thread_read_one(tsd, fd, &c)) == 1) {
            *pstr++ = c;
            cnt++;
            if (c == '\n')
                break;
        }

        if (result == 0) {
            *pstr = 0;
            return 0;
        }
        
        if (result == -1)
            return -1;
    }

    // 数据结尾
    *pstr = 0;
    return cnt;
}

