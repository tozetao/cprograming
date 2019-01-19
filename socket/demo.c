#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <errno.h>

void sig_child(int signo)
{
    printf("signo %d\n", signo);
    return;
}

int main()
{
//    char buffer[1024];
//    char *p;
//    int fd, count;
//
//    bzero(buffer, sizeof(buffer));
//    
//    p = buffer;
//
//    *p++ = 'a';
//    *p++ = 'b';
//    *p++ = 'c';
//    *p++ = 'd';
//    printf("%s\n", buffer);
//    printf("%d\n", SIGCHLD);
//    printf("%d\n", EINTR);

    pid_t pid;
    signal(SIGCHLD, sig_child);
    if ((pid = fork()) == 0){
        printf("child process pid is %d\n", getpid());
        exit(0);
    } else {
        int i = 0;
        while (i < 10) {
            sleep(1);
            i++;
        }
        printf("the process is parent, child process is %d\n", pid);
        printf("errno %d\n", errno);
    }


//    fd = open("test.log", O_RDWR);
//    count = read(fd, buffer, 100);
//    char *tp, sp;
//    tp = buffer;
//
//    sp = *tp++;
//    printf("%c\n", *tp);
//    printf("%c\n", sp);
    
//    while(fgets(buffer, 1024, stdin) != NULL) {
//        printf("%d\n", (int)strlen(buffer));
//        fputs(buffer, stdout);
//    }

    return 0;
}
