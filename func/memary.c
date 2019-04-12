#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Part {
    char *name;
    int age;
};

void *concat(const char *s1, const char *s2);

int main()
{
    struct Part *part;

    part = calloc(1, sizeof(struct Part));

    part->name = "hello";
    part->age = 100;

    printf("%s %d\n", part->name, part->age);

    return 0;
}

void *concat(const char *s1, const char *s2) 
{
    int length = strlen(s1) + strlen(s2) + 1;

    char *result, *pr;
    result = malloc(length);
    pr = result;

    while(*s1 != '\0')
        *pr++ = *s1++;

    while(*s2 != '\0')
        *pr++ = *s2++;
    
    *pr = '\0';
    return result;
}
