#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <signal.h>

void sig_child(int signo)
{
    printf("signo %d", signo);
}

int main()
{
    char buffer[1024];
    char *p;
    int fd, count;

    bzero(buffer, sizeof(buffer));
    
    p = buffer;

    *p++ = 'a';
    *p++ = 'b';
    *p++ = 'c';
    *p++ = 'd';
    printf("%s\n", buffer);
    printf("%d\n", SIGCHLD);

    
    signal(SIGCHLD, sig_child);
    pid_t pid;
    if ((pid = fork()) == 0){
        sleep(10);
        printf("%s", "the process is parent\n");
    } else {
        printf("child process pid is %d\n", pid);
        exit(0);
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
