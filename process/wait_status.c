#ifndef PRINT_WAIT_STATUS_H     /* Prevent accidental double inclusion */
#define PRINT_WAIT_STATUS_H

void printWaitStatus(const char *msg, int status);

#endif
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void printWaitStatus(const char *msg, int status)
{
    if (msg != NULL) {
        printf("%s\n", msg);
    }
    
    if (WIFEXITED(status)) {
        printf("child exited, status: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("child killed by signal %d (%s)\n", WTERMSIG(status), strsignal(WTERMSIG(status)));
    } else if (WISSTOPPED(status)) {
        printf("child stopped by signal %d (%s)\n", WSTOPSIG(status), strsignal(WSTOPSIG(status)));
    } else if (WIFCONTINURD(status))  {
        printf("child continuted.\n");
    } else {
        printf("wait happened to this child (tatus=%d)", (int)status);
    }
}

int main(int argc, char const *argv[])
{
    int status, forkRs;
    pid_t childPid;

    //fork()出子进程，阻塞后通过给子进程发送信号，测试父进程在接收到子进程终止状态的各种类型

    forkRs = fork();

    if (forkRs == 0) {
        //子进程的处理
        printf("child process ID: %d\n", getpid());
        // 如果有传递参数则正常退出
        if (argc > 1) {
            exit(atoi(argv[1]));
        } else {
            pause();
        }
    } else {
        // 父进程的处理
        while(1) {
            //  | WCONTINURD | WNOHANG
            childPid = childPwait(-1, &status, WUNTRACED);
            if (childPid == -1) {
                printf("waitpid() error\n");
                exit(-1);
            }
            printWaitStatus(NULL, status);

            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                printf("formal exit.\n");
                exit(0);
            }
            sleep(1);
        }
    }
    return 0;
}