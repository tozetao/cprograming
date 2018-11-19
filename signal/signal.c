#include <stdio.h>
#include <signal.h>

void sigHandler(int sig)
{
    int i;
    for(i = 1; i <= 3; i++) {
        printf("handler%d\n", i);
        sleep(2);
    }
    printf("Ouch\n");
}

int main(int argc, char const *argv[])
{
    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        printf("error signal\n");
        return 1;
    }

    int i;
    for(i=0; i<3; i++) {
        printf("%d\n", i);
        sleep(3);
    }

    return 0;
}
