#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

void test_read(int fd)
{
    size_t length = 10;
    char buffer[length];

    // 读取10个字符
    read(fd, buffer, length);
    printf("%s\n", buffer);

    // 再读取10个字符
    read(fd, buffer, length);
    printf("%s\n", buffer);

    // 根据测试结果可以看到read()在读取数据的同时在改变偏移量。
}

void test_pread(int fd)
{
    size_t length = 10;
    char buffer[length + 1];

    if(pread(fd, buffer, length, 0) == 10) {
        buffer[length] = '\0';
        printf("%s\n", buffer);
    }
    
    if(pread(fd, buffer, length, 0) == 10) {
        buffer[length] = '\0';
        printf("%s\n", buffer);
    }
}

int main(int argc, char const *argv[])
{
    int filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int fd = open("test.log", O_CREAT | O_RDWR, filePerms);
    test_pread(fd);
    return 0;
}
