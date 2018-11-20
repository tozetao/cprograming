#define _GUN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static int sigCount[NSIG];
static volatile sig_atomic_t out = 0;

void handler(int sig) {
    if (sig == 2) {
        out = 1;
    } else {
        sigCount[sig]++;
    }
}

int main(int argc, char const *argv[])
{
    int i;

    printf("mreceive pid is %d\n", getpid());

    for (i = 1; i < NSIG; i++) {
        signal(i, handler);
    }

    while(!out) {
        // sleep(1);    //有意思的现象，如果使用sleep()睡眠的话，会因为CPU资源占用少，接收方进程能够接受到更多的信号，估计是因为接收方进程获得更多的执行时间来处理信号。
        continue;
    }
    
    for (i = 1; i < NSIG; i++) {
        if(sigCount[i] != 0) {
            printf("siginal = %d; count = %d\n", i, sigCount[i]);
        }
    }

    return 0;
}