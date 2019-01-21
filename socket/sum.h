struct args {
    long arg1;
    long arg2;
};

struct result {
    long sum;
};

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE];
    struct args args;
    struct result result;

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (sscanf(sendline, "%ld %ld", &args.arg1, &args.arg2) != 2) {
            printf("invalid input\n");
            continue;
        }

        written(sockfd, &args, sizeof(args));

        if (readn(sockfd, &result, sizeof(result)) == 0) 
            printf("readn error\n");
        else
            printf("result %ld\n", result.sum);
    }
}


