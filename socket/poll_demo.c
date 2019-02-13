#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>

int main()
{
    struct pollfd client[1024];
    char buffer[1024];

    int nready, n;
    int STDIN = fileno(stdin);

    client[0].fd = STDIN;
    //client[0].events = POLLRDNORM | POLLIN;
    client[0].events = POLLIN;

    printf("POLLRDNORM = %d; POLLIN = %d\n", POLLRDNORM, POLLIN);

    while (1) {
        nready = poll(client, 1, -1);
        printf("nready = %d; client.revents = %d; result = %d\n", nready, client[0].revents, client[0].revents & POLLIN);

        if (client[0].revents & (POLLIN)) {
            if ((n = read(client[0].fd, buffer, 1024)) == 0) {
                printf("read null from stdin\n");
                return 0;
            }
            printf("read %d bytes\n", n);
            write(fileno(stdout), buffer, n);
        }
    }

    return 0;
}
