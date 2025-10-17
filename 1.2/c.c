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

void *mythread() {
    char* valueToReturn = "hello world\0";
    return (void *)valueToReturn;
}

int main() {
    pthread_t tid;
    int err;
    char *thread_result;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    err = pthread_create(&tid, NULL, mythread, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return ERROR;
    }

    err = pthread_join(tid, (void **)&thread_result);
    if (err) {
        printf("main: pthread_join() failed: %s\n", strerror(err));
        return ERROR;
    }

    printf("Received value in main: %s\n", thread_result);

    return SUCCESS;
}