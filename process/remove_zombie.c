#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <signal.h>

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    
    if(WIFEXITED(status))
    {
        printf("Removed proc id: %d\n", id);
        printf("Child send %d\n", WEXITSTATUS(status));
    }
}

int main(int argc, char const *argv[])
{
    pid_t pid;
    struct sigaction act;

    //初始化信号注册信息
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    //注册
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if(pid == 0){
        puts("Hi, I'm a child process");
        sleep(10);
        return 110;
    }else{
        printf("Child proc id:%d\n", pid);
        pid = fork();
        if(pid == 0){
            puts("Hi, I'm a child process");
            sleep(10);
            exit(20);
        }else{
            printf("Child proc id:%d\n", pid);
            int i;
            for(i=0; i<5; i++){
                puts("wait...");
                sleep(5);
            }
        }
    }

    return 0;
}
