#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    int sigNum, nums, j;
    pid_t pid;

    if (argc < 4) {
        printf("%s pid nums sig_num sig_num2", argv[0]);
        return -1;
    }

    // 接收信号的进程ID
    pid = atol(argv[1]);

    // 发送信号的次数
    nums = atoi(argv[2]);

    // 信号
    sigNum = atoi(argv[3]);

    // 向指定的进程发送信号
    printf("pid: %d; nums: %d; sig_num1: %d\n", pid, nums, sigNum);
    for (j=0; j<nums; j++) {
        if (kill(pid, sigNum) == -1) {
            printf("kill() error\n");
            exit(-1);
        }
    }

    printf("argc: %d\n", argc);

    // 发送SIGINT信号
    if (argc > 4) {
        int lastSig = atoi(argv[4]);
        if (kill(pid, lastSig) == -1) {
            printf("send last signal error\n");
            exit(-1);
        }
    }

    printf("exiting...\n");
    return 0;
}
