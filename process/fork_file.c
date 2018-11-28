#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

void printf_error(char *content) {
    printf("%s", content);
    exit(-1);
}

int main(int argc, char const *argv[])
{
    int fd, flags;

    fd = open("./test.log", O_RDWR | O_CREAT);
    printf("File offset before fork(): %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

    flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        printf_error("fcntl error\n");
    }
    printf("O_APPEND flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

    pid_t childId = fork();
    if (childId == -1) {
        printf_error("fork error\n");
    }

    if (childId == 0) {
        // 子进程的处理逻辑
        if (lseek(fd, 1000, SEEK_SET) == -1) {
            printf_error("lseek error\n");
        }

        flags = fcntl(fd, F_GETFL);
        if (flags == -1) {
            printf_error("child process fcntl error\n");
        }
        flags |= O_APPEND;
        if (fcntl(fd, F_SETFL, flags) == -1) {
            printf_error("fcntl - F_SETFL\n");
        }
        _exit(0);
    } else {
        // 父进程的处理逻辑
        if (wait(NULL) == -1) {
            printf_error("wait error\n");
        }
        printf("child has exited\n");
        printf("The file offset in parent: %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

        flags = fcntl(fd, F_GETFL);
        if (flags == -1) {
            printf_error("fcntl - F_GETFL\n");
        }
        printf("O_APPEND flag in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");
    }

    return 0;
}