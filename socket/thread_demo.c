#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static pthread_once_t onceptr = PTHREAD_ONCE_INIT;

void once()
{
    printf("run once\n");
}

// 线程的执行函数
void *exec(void *arg)
{
    printf("thread id: %ld\n", pthread_self());
    
    // 测试是否执行一次
    pthread_once(&onceptr, once);
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
