#define _GUN_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>

static volatile sig_atomic_t canJump = 0;
static sigjmp_buf senv;
// #ifdef USE_SIGSETJMP
// static sigjmp_buf senv;
// #else
// static jmp_buf env;
// #endif

void handler(int sig)
{
    printf("received signal=%d(%s)\n", sig, strsignal(sig));

    if (!canJump) {
        printf("env buffer not yet set, doing a simple return.\n");
        return;
    }

    siglongjmp(senv, 1);

// #ifdef USE_SIGSETJMP
//     siglongjmp(senv, 1);
// #else
//     longjmp(env, 1);
// #endif
}

int main(int argc, char const *argv[])
{
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    // 注册信号处理器
    if (sigaction(SIGINT, &act, NULL) == -1) {
        printf("sigaction error\n");
        exit(-1);
    }

    // 返回0表示已做标记
    if (sigsetjmp(senv, 0) == 0) {
        printf("call sigsetjmp\n");
        canJump = 1;
    }
    // 不为0表示从siglongjmp()调用返回 
    else {
        printf("after jump from handler\n");
    }

    for(;;)
        pause();

    return 0;
}