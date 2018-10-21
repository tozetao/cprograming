#define _GUN_SOURCE
#include <stdio.h>
#include <signal.h>

void sighandle(int sig)
{
    printf("signal: %d\n", sig);
    if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
        printf("sig_del error\n");
    }
}

int main(int argc, char const *argv[])
{
    /* code */
    int i;
    sigset_t blockSet, prevSet, pendSet;

    // 注册sigint信号的处理程序
    if (signal(SIGINT, &sighandle) == SIG_ERR) {
        printf("registe sigint error.\n");
        return -1;
    }

    sigemptyset(&blockSet);
    sigaddset(&blockSet, SIGINT);

    // 阻塞SIGINT信号
    if (sigprocmask(SIG_BLOCK, &blockSet, &prevSet) == -1) {
        printf("block set error\n");
        return -1;
    }

    // 睡眠5秒，在睡眠期间触发SIGINT信号，看看该信号的处理程序是否会调用
    for(i=0; i<5; i++) {
        sleep(1);
        printf("sleep %d\n", i);
    }

    // 获取阻塞的信号
    if (sigpending(&pendSet) == -1) {
        printf("sigpending error\n");
        return -1;
    }

    // 判断SIGINT信号是否在信号掩码中
    if (sigismember(&pendSet, SIGINT)) {
        printf("SIGINT is in set\n");
    }

    // 将信号掩码重置为原先的状态以解除对信号的阻塞
    printf("end pending...\n");
    if (sigprocmask(SIG_SETMASK, &prevSet, NULL) == -1) {
        printf("setmask error\n");
        return -1;
    }

    return 0;
}
