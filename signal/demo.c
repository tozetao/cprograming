#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void printfSigset(const sigset_t *set)
{
    int cnt = 0, sig;
    
    for (sig = 1; sig < NSIG; sig++) {
        if (sigismember(set, sig)) {
            cnt++;
            // printf("%d; %s", sig, strsignal(sig));
        }
    }

    if (cnt == 0) {
        printf("empty signal set\n");
    }
}

int main(int argc, char const *argv[])
{
    int i;
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, 10);
    if (sigismember(&set, 10)) {
        printf("is member\n");
    }
    printfSigset(&set);

    char *result = strsignal(2);
    printf("%s\n", result);

    return 0;
}
