#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define THREAD_LIMIT 3

#define THREAD_DONE 1
#define THREAD_COLLECTION 2

static int ndone = 3;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

struct thread_instance {
    pthread_t tid;
    int flag;
};

void *exec(void *);

// 创建3个线程，当子线程执行完毕后通知主线程进行回收
int main()
{
    int i, ret;

    struct thread_instance threads[THREAD_LIMIT];

    // 创建3个线程
    for(i = 0; i < THREAD_LIMIT; i++) {
        threads[i].flag = 0;
        pthread_create(&threads[i].tid, NULL, exec, &threads[i]);
    }

    while (ndone > 0) {
        // 监听条件变量
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);

        for(i = 0; i < THREAD_LIMIT; i++) {
            if (threads[i].flag & THREAD_DONE) {
                ret = pthread_join(threads[i].tid, NULL);
                ndone--;
                threads[i].flag = THREAD_COLLECTION;
                printf("%ld desotry, ret = %d, ndone = %d\n", threads[i].tid, ret, ndone);
            }
        }

        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

// 睡眠任意秒数，然后通知主线程
void *exec(void *var) 
{
    int random = rand() % 3;
    struct thread_instance *ins = (struct thread_instance *)var;

    sleep(random);

    pthread_mutex_lock(&mutex);

    ins->flag = THREAD_DONE;
    pthread_cond_signal(&cond);
    printf("%ld sleep for %d seconds\n", pthread_self(), random);

    pthread_mutex_unlock(&mutex);
    
    return NULL;
}
