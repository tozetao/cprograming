#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 20

int main(int argc, char const *argv[])
{
    int fds[2];
    char send[] = "what are you doing!";
    char response[] = "i'am find, think you!";
    char buffer[BUF_SIZE];
    pid_t pid;

    pipe(fds);

    pid = fork();
    if(pid == 0){
        write(fds[1], send, sizeof(send));
        sleep(2);
        read(fds[0], buffer, BUF_SIZE);
        printf("from parent message: %s\n", buffer);
    }else{
        read(fds[0], buffer, BUF_SIZE);
        printf("from child message: %s\n", buffer);
        
        write(fds[1], response, sizeof(response));
        sleep(3);
    }

    return 0;
}
