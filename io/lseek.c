#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void printf_err(char *s)
{
    printf("%s\n", s);
    exit(-1);
}

int main(int argc, char const *argv[])
{
    int fd, openFlags, filePerms;
    int length = 1024;
    char buffer[length];

    // openFlags = O_CREAT | O_RDWR | O_TRUNC;
    openFlags = O_CREAT | O_RDWR;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    fd = open("./test.log", openFlags, filePerms);
    if (fd == -1) {
        printf_err("open error.");
    }

    int seek = lseek(fd, 0, SEEK_END);     //相对于文件尾部的下一个字节
    // lseek(fd, -1, SEEK_END);    //相对于文件的最后一个字节
    // lseek(fd, -2, SEEK_END);    //可以在test.log内容进行测试

    int num = read(fd, buffer, length);
    printf("%d\n", seek);   //假设文件有hello5个字符，将会输出6，因为文件结尾包含一个字节，同时SEEN_END指向文件结尾的下一个字节，所以是6
    printf("%d\n", num);
    printf("%s", buffer);

    // 读取一个文件的内容
    // int num;
    // while((num = read(fd, buffer, length)) != -1 && num != 0){
    //     printf("%s", buffer);
    // }
    
    close(fd);

    return 0;
}
