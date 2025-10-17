#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sys/resource.h>
#include <errno.h>
#include <time.h>

#define SUCCESS 0
#define ERROR 1

static _Atomic unsigned long long counter = 0;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* worker(void* arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    struct timespec time_now;
    while (1) {
        clock_gettime(CLOCK_REALTIME, &time_now);
        time_now.tv_sec += 0.1;
        
        pthread_mutex_lock(&mtx);
        printf("worker: waiting...\n");
        int res = pthread_cond_timedwait(&cond, &mtx, &time_now);
        
        if (res == 0) {
            printf("worker: signaled...\n");
        } else if (res == ETIMEDOUT) {
            printf("worker: timeout...\n");
        } else {
            printf("worker: error...\n");
        }

        counter++;
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

int main(void) {
    pthread_t tid;
    pthread_create(&tid, NULL, worker, NULL);

    sleep(2);
    printf("main: cancel...\n");
    pthread_cancel(tid);

    printf("main: join...\n");
    pthread_join(tid, NULL);
    
    int err = pthread_mutex_trylock(&mtx);
    if (err == 0) {
        printf("Мьютекс был свободен, теперь мы его захватили\n");
        pthread_mutex_unlock(&mtx);
    } else if (err == EBUSY) {
        printf("Мьютекс уже занят другим потоком\n");
        pthread_mutex_unlock(&mtx);
    } else {
        printf("Ошибка: %d\n", err);
    }
    
    printf("main: finished, counter=%llu\n", counter);
    return 0;
}
