#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 1

static _Atomic unsigned long long counter = 0;
static _Atomic int cancel_flag = 0;

void* worker(void* arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    while (!cancel_flag) {
        counter++;
    }
    return NULL;
}

int main(void) {
    pthread_t tid;
    
    pthread_create(&tid, NULL, worker, NULL);

    sleep(2);
    printf("main: cancel_flag = 1\n");
    cancel_flag = 1;

    printf("main: join...\n");
    pthread_join(tid, NULL);
    printf("main: finished, counter=%llu\n", counter);
    return 0;
}
