#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    int receivePid = 0, signal = 0, num = 10, i;
    printf("msend pid is %d\n", getpid());

    //参数1是接收方进程的ID
    receivePid = atoi(argv[1]);
    //参数2是要发送的信号
    signal = atoi(argv[2]);
    //参数3是要发送的次数
    num = atoi(argv[3]);

    for(i=0; i<num; i++) {
        if (kill(receivePid, signal) == -1) {
            printf("kill error.");
            exit(-1);
        }
    }

    return 0;
}


/*

接收方进程的信号处理程序会统计触发信号的次数，直到发送方进程发送SIGINT信号，结束主程序的阻塞，打印每个信号出现的次数。

发送方进程会一直发送N多个信号，直到发送SIGINT信号。
*/