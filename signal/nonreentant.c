#define _XOPEN_SOURCE 600
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static char const *str;
static int handled = 0;

static void handler(int sig)
{
    crypt(str, "xx");
    handled++;
}

int main(int argc, char const *argv[])
{
    char *cr1;
    char *temp;
    char *temp1;
    int callNum, mismatch;
    struct sigaction sa;

    // if (argc != 3) {
    //     printf("argc error\n");
    //     exit(-1);
    // }

    // str = argv[2];
    // // cr1 = strdup(crypt(argv[1], "xx"));  //问题出现在这里
    // cr1 = crypt(argv[1], "xx");
    // printf("cr1 = %s\n", cr1);

    // if (cr1 == NULL) {
    //     printf("strdup error\n");
    //     exit(-1);
    // }

    // // 注册SIGINT的处理程序
    // sigemptyset(&sa.sa_mask);
    // sa.sa_flags = 0;
    // sa.sa_handler = handler;
    // if (sigaction(SIGINT, &sa, NULL) == -1) {
    //     printf("signation error\n");
    //     exit(-1);
    // }

    // for (callNum = 1, mismatch = 0; ; callNum++) {
    //     temp = crypt(argv[1], "xx");
    //     if (strcmp(temp, cr1) != 0) {
    //         mismatch++;
    //         printf("temp = %s; cr1 = %s; match = %d; handle = %d\n", temp, cr1, mismatch, handled);
    //     }
    // }

    // crypt使用了静态分配的内存来返回信息。
    // 这意味着它返回的指针总是指向同一个内存地址，如下测试
    // char *content = "hello";
    // temp = crypt(content, "xxx");
    // printf("temp = %x; value = %s\n", temp, temp);

    // temp1 = crypt("first", "xxx");
    // printf("temp = %x; value = %s\n", temp, temp);
    // printf("temp1 = %x; value = %s\n", temp1, temp1);

    return 0;
}
