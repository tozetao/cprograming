#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct
{
    char *name;
    int age;
}User;

void *test(void *arg)
{
    User *user = (User *)arg;
    printf("name: %s, age: %d, thread id: %ld\n", user->name, user->age, pthread_self()); 
    return NULL;
}

int main()
{
    User user = {
        "zhangsan",
        25
    };

    printf("this is my demo");

    pthread_t tid;
    pthread_create(&tid, NULL, test, &user);
    sleep(2);

    return 0;
}
