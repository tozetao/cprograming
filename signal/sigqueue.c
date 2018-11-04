#define _POSIX_C_SOURCE 199309
#include <signal.h>

int main(int argc, char const *argv[])
{
    int i, sig, numSigs, sigData, pid;
    union sigval sv;

    for(i=0; i < 3; i++) {
        // 指定携带的数据
        sv.sival_int = 10 + i;

        // 同一信号发送多次
        if (sigqueue(pid, sig, sv) == -1) {
            printf("sigqueue error\n");
            exit(-1);
        }
    }
    
    return 0;
}