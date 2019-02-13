#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void alarm_handler(int signo)
{
    printf("sigal = %d\n", signo);
    return;
}

int main()
{
    int r;

    signal(SIGALRM, alarm_handler);
    r = alarm(5);
    printf("r = %d\n", r);

    int i = 0;
    while(i < 10) {
        printf("sleep %d\n", i+1);
        sleep(1);
        i++;
    }

    return 0;
}
