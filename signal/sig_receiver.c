#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int sigCount[NSIG];
static volatile sig_atomic_t gotSigint = 0;

static void handler(int sig)
{
    if (sig == SIGINT) {
        gotSigint = 2;
    } 
    else {
        sigCount[sig]++;
    }
}

void printfSigset(const sigset_t *set)
{
    int cnt = 0, sig;
    
    for (sig = 1; sig < NSIG; sig++) {
        if (sigismember(set, sig)) {
            cnt++;
            printf("%d %s; ", sig, strsignal(sig));
        }
    }

    if (cnt == 0) {
        printf("empty signal set\n");
    }
}

int main(int argc, char const *argv[])
{
    int n, numSecs;
    sigset_t pendMask, blockMask, emptyMask;

    printf("NSIG: %d\n", NSIG);
    printf("%s: PID is %d\n", argv[0], getpid());


    // 为每个信号注册处理程序
    for(n = 1; n < NSIG; n++) {
        (void)signal(n, handler);
    }

    if (argc > 1) {
        numSecs = atoi(argv[1]);
        
        // 填充所有信号
        sigfillset(&blockMask);

        // 阻塞所有信号
        if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1) {
            printf("set blockmask error\n");
            exit(-1);
        }

        // 阻塞信号后，在睡眠时间内发送不同的信号，查看标准信号是否有排队情况。
        printf("%s sleeping for %d\n", argv[0], numSecs);
        int index = 0;
        while (index < numSecs) {
            sleep(1);
            index++;
            printf("sleep: %d\n", index);
        }

        // 获取处于pending状态的信号集
        if (sigpending(&pendMask) == -1) {
            printf("sigpending error\n");
            exit(-1);
        }

        // 打印正在阻塞的信号
        printf("%s pending signal are :", argv[0]);
        printfSigset(&pendMask);
        printf("\n");

        // 清空要阻塞的信号
        sigemptyset(&emptyMask);
        if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1) {
            printf("unblock error\n");
            exit(1);
        }
    }

    printf("getSigint: %d\n", gotSigint);
    while(!gotSigint) {
        continue;
    }
    printf("end getSigint\n");

    printf("display signal nums\n");
    printf("START NSIG: %d\n", NSIG);

    // 显示已经接受的信号
    for (n = 1; n < NSIG; n++) {
        if (sigCount[n] != 0) {
            printf("signal = %d; count = %d\n", n, sigCount[n]);
            // printf("%s: signal %d caught %d time %s\n", argv[0], n, sigCount[n], (sigCount[n] == 1 ? ' ' : 's'));
        }
    }

    printf("END NSIG: %d\n", NSIG);
    return 0;
}