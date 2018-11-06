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
    }

    sigCnt++;

    printf("caught signal: %d\n", sig);

    printf("   si_signo = %d, si_code = %d (%s), ", si->si_signo, si->si_code, 
        (si->si_code == SI_USER) ? "SI_USER" :
        (si->si_code == SI_QUEUE) ? "SI_QUEUE" : "other");

    printf("si_value = %d\n", si->si_value.sival_int);
    printf("   si_pid = %ld, si_uid = %ld\n", (long)si->si_pid, (long)si->si_uid);

    printf("   sigCnt = %d\n", sigCnt);
    // sleep(handlerSleepTime);
}

int main(int argc, char const *argv[])
{
    struct sigaction sa;
    int sig;
    sigset_t prevMask, blockMask;

    // handlerSleepTime = (argc > 2) ? atoi(argv[]);
    handlerSleepTime = 1;

    sa.sa_sigaction = siginfoHandler;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);

    // 注册信号处理器程序
    for(sig = 1; sig < NSIG; sig++) {
        if (sig != SIGTSTP || sig != SIGQUIT) {
            sigaction(sig, &sa, NULL);
        }
    }

    //如果有传递1个参数，则进行睡眠
    if (argc > 1) {
        sigfillset(&blockMask);
        sigdelset(&blockMask, SIGINT);
        sigdelset(&blockMask, SIGTERM);

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
        printf("pause = %d\n", p);
    }

    return 0;
}