#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    int status;

    pid = fork();

    if(pid == 0){
        sleep(15);
        return 25;
    }else{
        int result = waitpid(-1, &status, WNOHANG);
        printf("result: %d\n", result);

        sleep(1);

        result = waitpid(-1, &status, WNOHANG);
        printf("result: %d\n", result);

        sleep(1);

        while(!waitpid(-1, &status, WNOHANG)){
            sleep(3);
            puts("sleep 3sec");
        }

        if(WIFEXITED(status))
            printf("Child send one: %d\n", WEXITSTATUS(status));
    }
    
/*
    if(-1){
        printf("true\n");
    }else{
        printf("false\n");
    }
    
    if(0){
        printf("true\n");
    }else{
        printf("false\n");
    }

    if(1){
        printf("true\n");
    }else{
        printf("false\n");
    }

    if(!-1){
        printf("true\n");
    }else{
        printf("false\n");
    }

    if(!1){
        printf("true\n");
    }else{
        printf("false\n");
    }

    if(!0){
        printf("true\n");
    }else{
        printf("false\n");
    }
*/
    return 0;
}
