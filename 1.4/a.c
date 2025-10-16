#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR 1

void* printer(void* arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    while (1) {
        printf("thread working...\n");
        sleep(1);
    }

    return NULL;
}

int main(void) {
    pthread_t tid;
    int err;

    err = pthread_create(&tid, NULL, printer, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return ERROR;
    }
    
    sleep(5);

    printf("main: try close thread...\n");

    pthread_cancel(tid);

    printf("main: maybe thread canceled\n");

    pthread_join(tid, NULL);

    printf("main: thread closed\n");
    return 0;
}
