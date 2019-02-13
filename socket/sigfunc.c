#include <signal.h>

typedef void sigfunc(int);

sigfunc * signal(int signo, sigfunc *func)
{
    struct sigaction act, oact;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = func;

    //SIGALRM: 该信号表示长时间的超时信号，一般不希望受阻塞的系统调用占用进程，所以可以为I/O操作设置超时信号
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
        //SA_RESTART标志位指的是，当内核执行完信号处理函数后，会回复中断的系统调用的执行
        //默认阻塞的系统调用在被信号中断时，这些信号在被捕获处理完毕后，之前的系统调用因为触发信号处理而返回EINTR错误。
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
    
    if (sigaction(signo, &act, &oact) < 0)
            return SIG_ERR;

    return oact.sa_handler;
}
