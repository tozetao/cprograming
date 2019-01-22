
struct args {
    long arg1;
    long arg2;
};

struct result {
    long sum;
};

void number_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE];
    struct args args;
    struct result result;
    ssize_t n;

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (sscanf(sendline, "%ld %ld", &args.arg1, &args.arg2) != 2) {
            printf("invalid input\n");
            continue;
        }

        n = written(sockfd, &args, sizeof(args));
        printf("client written %ld\n", n);

        if (readn(sockfd, &result, sizeof(result)) == 0) 
            printf("readn error\n");
        else
            printf("result %ld\n", result.sum);
    }
}


void number_echo(int sockfd)
{
    ssize_t n;
    struct args args;
    struct result result;
    printf("number echo %d\n", sockfd);

    while ((n = readn(sockfd, &args, sizeof(args))) > 0) {
        result.sum = args.arg1 + args.arg2;
        printf("read %ld bytes from client, sum = %ld\n", n, result.sum);
        result.sum = result.sum + 100;
        written(sockfd, &result, sizeof(result));
    }

    printf("read end %ld\n", n);
}
