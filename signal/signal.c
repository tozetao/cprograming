#include <stdio.h>
#include <signal.h>

void sigHandler(int sig)
{
    printf("signal %d\n", sig);
    // int i;
    // printf("current signal: %d\n", sig);
    // for(i = 1; i <= 3; i++) {
    //     printf("handler%d\n", i);
    //     sleep(2);
    // }
    // printf("end %d.\n", sig);
}

int main(int argc, char const *argv[])
{
    // if (signal(SIGINT, sigHandler) == SIG_ERR) {
    //     printf("error signal\n");
    //     return 1;
    // }

    int i;
    sigset_t blockSet, prevSet;

    printf("pid = %d\n", getpid());

    //为所有信号注册处理函数
    for(i = 1; i < NSIG; i++) {
        signal(i, sigHandler);
    }

    //阻塞所有信号
    // sigfillset(&blockSet);
    // sigemptyset(&prevSet);
    // sigprocmask(SIG_BLOCK, &blockSet, &prevSet);

    // 睡眠
    for(i = 0; i < 5; i++) {
        printf("%d\n", i);
        sleep(3);
    }

    // 解除阻塞，查看信号处理过程
    // sigprocmask(SIG_SETMASK, &prevSet, NULL);

    return 0;
}

// 发送多种信号，测试在解除阻塞下信号的处理顺序