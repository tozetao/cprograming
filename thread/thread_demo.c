#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>

static pthread_once_t onceptr = PTHREAD_ONCE_INIT;
static pthread_key_t KEY;

void destructor(void *value)
{
    printf("%ld destoryed\n", pthread_self());
    if (value != NULL) {
        printf("the destoryed value is %ld\n", *(long *)value);
        free(value);
    }
}

// 初始化线程特定数据的KEY
void init_key()
{
    pthread_key_create(&KEY, destructor);
}

// 线程的执行函数
void *exec(void *arg)
{
    void *thread_value;

    printf("thread id: %ld\n", pthread_self());
    pthread_detach(pthread_self());

    pthread_once(&onceptr, init_key);
    printf("KEY is %d\n", KEY);

    if ((thread_value = pthread_getspecific(KEY)) == NULL) {
        thread_value = malloc(sizeof(long));
        pthread_setspecific(KEY, thread_value);
    }

    *(long *)thread_value = pthread_self();

    return NULL;
}

int main() 
{
    pthread_t tid;

    pthread_create(&tid, NULL, exec, NULL);
    pthread_create(&tid, NULL, exec, NULL);
    pthread_create(&tid, NULL, exec, NULL);
    
    sleep(5);
    return 0;
}
