#include <unistd.h>

static int idata = 120;

int main(int argc, char const *argv[])
{
    int stack = 10;
    pid_t childId;

    childId = fork();

    if (childId == -1) {
        printf("fork error.");
        exit(-1);
    }

    if (childId == 0) {
        idata *= 10;
        stack += 10;
    } else {
        // 希望父进程睡眠，让CPU去调度子进程
        sleep(5);
    }

    printf("This is %s process. PID = %d, idata=%d, stack = %d\n",
        (childId == 0 ? "child" : "parent"), (long)getpid(), idata, stack);
    return 0;
}