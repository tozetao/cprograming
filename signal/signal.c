#include "<stdio.h>"
#include "<signal.h>"

void sigHandler(int sig)
{
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
