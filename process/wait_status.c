#define _GUN_SOURCE
#include <string.h>
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