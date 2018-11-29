#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main(int argc, char const *argv[])
{
    // char **temp;
    
    // for(temp = environ; temp != NULL; temp++){
    //     printf("%s\n", *temp);
    // }

    char *val = getenv("SHELL");
    printf("%s\n", val);

    return 0;
}


