#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

static volatile int handlerSleepTime;
static volatile int sigCnt = 0;
static volatile int allDone = 0;

static void siginfoHandler(int sig, siginfo_t *si, void *ucontext)
{
    if (sig == SIGINT || sig == SIGTERM) {
        allDone = 1;
        return;
    } else  {
        printf("signal = %d; value = %d\n", sig, si->si_value.sival_int);
    }
}


// 1. 测试不阻塞情况下，相同信号是否以队列的方式来处理。
// 2. 测试阻塞情况下，不同信号是否有按照信号值的升序来处理。
int main(int argc, char const *argv[])
{
    struct sigaction sa;
    int sig;
    sigset_t prevMask, blockMask;

    sa.sa_sigaction = siginfoHandler;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);

    // 注册信号处理器程序
    for(sig = 1; sig < NSIG; sig++) {
        sigaction(sig, &sa, NULL);
    }

    // 参数大于1表示要测试不同信号阻塞的情况
    if (argc > 1) {
        sigfillset(&blockMask);
        sigemptyset(&prevMask);
        
        //阻塞信号
        if (sigprocmask(SIG_SETMASK, &blockMask, &prevMask) == -1) {
            printf("sigprocmask error\n");
            exit(-1);
        }

        //睡眠一定时间
        sleep(atoi(argv[1]));

        //停止阻塞信号
        if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1) {
            printf("unblock mask error\n");
            exit(-1);
        }
    }

    // allDone为1时停止进程睡眠
    while(!allDone) 
    {
        int p = pause();
        printf("pause = %d; errno = %d; EINTR value = %d\n", p, errno, EINTR);
    }

    return 0;
}