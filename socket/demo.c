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

void test_snprintf()
{
    char line[100];
    int i;

    i = snprintf(line, sizeof(line), "%012dend number", 12345);
    printf("%s\n", line);
    printf("i = %d; line = %s; size = %ld\n", i, line, sizeof(line));
}

void test_sscanf()
{
    char line[100] = "100 912";
    long arg1, arg2;
    int r;

    r = sscanf(line, "%ld %ld", &arg1, &arg2);
    printf("arg1 = %ld, arg2 = %ld, r = %d\n", arg1, arg2, r);
}

void test_signo()
{
//    pid_t pid;
//    signal(SIGCHLD, sig_child);
//    if ((pid = fork()) == 0){
//        printf("child process pid is %d\n", getpid());
//        exit(0);
//    } else {
//        int i = 0;
//        while (i < 10) {
//            sleep(1);
//            i++;
//        }
//        printf("the process is parent, child process is %d\n", pid);
//        printf("errno %d\n", errno);
//    }
}

void test_buffer()
{
    char buffer[1024];
    char *p;

    bzero(buffer, sizeof(buffer));
    
    printf("sizeof = %ld, length = %ld\n", sizeof(buffer), strlen(buffer));
    
    p = buffer;

    *p++ = 'a';
    *p++ = 'b';
    *p++ = 'c';
    *p++ = 'd';
}

struct args {
    long arg1;
    long arg2;
};

int main()
{
    //test_snprintf();
    //test_sscanf();
    //test_buffer();

    //struct args a1;
    //printf("%ld", sizeof(a1));
    int n = fileno(stdin);
    printf("%d", n);

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
