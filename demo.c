#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

void example1()
{
    char const *keyTables[] = {
        "dasheng",
        "xiaoli",
        "name",
        "ab"
    };

    int i, length;
    char const **ele = keyTables;
    
    length = sizeof(keyTables) / sizeof(keyTables[0]);
    printf("%d\n", length);
    for(i = 0; i < length; i++ ){
        printf("%s\n", keyTables[i]);
    }
}

void example2()
{
    char const *keyTables[] = {
        "a",
        "b",
        "abcd",
        NULL
    };

    char const **val = keyTables;

    printf("%s\n", *val);
    printf("%c\n", **val);

    val = val + 2;
    printf("%s\n", *val);
    printf("%c\n", *(*val + 2));

    // for(val = keyTables; *val != NULL; val++) {
    //     printf("%c\n", **val);
    // }

    /*
    int printf(char const *format, ...);
    format：格式化字符串
    ...是格式化字符所对应的具体的值，如果是int型则是具体的值，如果是字符串要对应字符串所指向的指针。
    */
}

int main()
{
//    struct in_addr in;
//    in.s_addr = inet_addr("192.168.2.11");
//    printf("%d\n", in.s_addr);
//    
//    char *val1 = inet_ntoa(in);
//    printf("%s\n", val1);
//
//    struct sockaddr_in server_addr;
//    printf("%ld, %d\n", sizeof(server_addr), INET6_ADDRSTRLEN);

    char buf[16];
    struct in_addr s;
    int r;

    r = inet_pton(AF_INET, "21.152.33.12", (void *)&s);
    printf("r = %d, %d\n", r, s.s_addr);
    
    inet_ntop(AF_INET, (void *)&s, buf, INET_ADDRSTRLEN);
    printf("ip: %s\n", buf);

    return 0;
}
