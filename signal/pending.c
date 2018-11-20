#define _GUN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sighandle(int sig)
{
    printf("signal: %d\n", sig);
}

void printPendingSig(sigset_t *pendSet)
{
    int i;
    for(i = 1; i < NSIG; i++) {
        if (sigismember(pendSet, i)) {
            printf("pending signal %d\n", i);
        }
    }
}

//阻塞某个信号，在睡眠期间，多次发送该信号，测试解除阻塞信号后的处理情况
int main(int argc, char const *argv[])
{
    sigset_t pendSet, prevSet, blockSet; 
    int i, cnt;

    sigemptyset(&prevSet);

    // 为所有信号注册处理程序
    for(i = 1; i < NSIG; i++) {
        signal(i, sighandle);
    }

    // 阻塞所有信号
    sigfillset(&blockSet);
    sigprocmask(SIG_BLOCK, &blockSet, &prevSet);

    // 睡眠，在睡眠期间手动去触发信号，测试效果
    for(cnt = 0; cnt < 3; cnt++) {
        printf("sleep %d\n", cnt + 1);
        sleep(1);
    }

    // 打印正在阻塞的信号
    sigpending(&pendSet);
    printPendingSig(&pendSet);

    // 解除阻塞信号
    sigprocmask(SIG_SETMASK, &prevSet, NULL);

    return 0;
}