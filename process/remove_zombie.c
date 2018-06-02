#include <stdio.h>
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
    sigaction(SIGALRM, &act, 0);

    pid = fork();
    if(pid == 0){
        puts("Hi, I'm a child process");
        sleep(10);
        return 110;
    }else{
        
    }

    return 0;
}
