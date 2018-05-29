#include <stdio.h>
#include <arpa/inet.h>

int main(void)
{
    /*
    unsigned short host_port = 0x1234;
    unsigned short net_port;

    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("host_port: %#x\n", host_port);
    printf("net_port: %#x\n", net_port);
    printf("host_addr: %#x\n", host_addr);
    printf("net_addr: %#x\n", net_addr);
    */
    
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr == INADDR_NONE)
    {
        printf("Error occured!\n");
    }
    else
    {
        printf("%#lx \n\n", conv_addr);
    }

    conv_addr = inet_addr(addr2);
    if(conv_addr == INADDR_NONE)
    {
        printf("Error occured!\n");
    }
    else
    {
        printf("%#lx \n\n", conv_addr);
    }

    return 0;
}
