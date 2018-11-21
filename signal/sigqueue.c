#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    int i, sig, numSigs, sigData, pid;
    union sigval sv;

    // 打印进程ID和用户ID
    printf("%s: PID = %d, UID = %d\n", argv[0], getpid(), getuid());

    // 进程ID
    pid = atoi(argv[1]);
    // 要传递的信号
    sig = atoi(argv[2]);
    // 携带的数据
    sigData = atoi(argv[3]);
    // 发送信号的次数
    numSigs = (argc > 4) ? atoi(argv[4]) : 1;

    for(i = 1; i < numSigs; i++) {
        // 指定携带的数据
        sv.sival_int = sigData + i;

        if (sigqueue(pid, sig, sv) == -1) {
            printf("sigqueue error; errno = %d\n", errno);
            exit(-1);
        }
    }
    return 0;
}

// 为了让处理器程序更好的输入，我们进行了睡眠，如果不睡眠，程序会一起处理然后进行输出。
// ./send 8299 55 200 5