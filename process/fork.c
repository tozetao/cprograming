#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int gval = 10;

int main(int argc, char const *argv[])
{    
    int status;
    int lval = 20;
    pid_t pid;

    pid = fork();

    if(pid == 0){
        return 3;
    }else{
        printf("child process pid: %d\n", pid);

        pid = fork();

        if(pid == 0){
            sleep(15);
            exit(7);
        }else{
            printf("child process pid: %d\n", pid);

            wait(&status);
            if(WIFEXITED(status))
                printf("Child send one: %d\n", WEXITSTATUS(status));

            // wait(&status);
            // if(WIFEXITED(status))
            //     printf("Child send two: %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
