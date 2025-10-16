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

struct myStruct {
    int number;
    char *message;
};

void *mythread(void *arg) {
    sleep(10);
    struct myStruct *data = (struct myStruct *)arg;
    printf("mythread [tid: %d]: number = %d, message = %s\n", gettid(), data->number, data->message);
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_attr_t attr;
    int err;
    struct myStruct data = {228, "test message to thread!!!"};

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    err = pthread_attr_init(&attr);
    if (err) {
        printf("pthread_attr_init() failed: %s\n", strerror(err));
        return ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err) {
        printf("pthread_attr_setdetachstate() failed: %s\n", strerror(err));
        err = pthread_attr_destroy(&attr);
        if (err){
            printf("main: pthread_attr_destroy() failed: %s\n", strerror(err));
        }
        return ERROR;
    }

    err = pthread_create(&tid, &attr, mythread, &data);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return ERROR;
    }

    err = pthread_attr_destroy(&attr);
    if (err){
        printf("main: pthread_attr_destroy() failed: %s\n", strerror(err));
    }

    sleep(5);

    pthread_exit(NULL);
}