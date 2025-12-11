#define _GNU_SOURCE
#include <stdio.h>
#include "./../mythread.h"
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR 1

void *mythread() {
    int *valueToReturn = malloc(sizeof(int));
    if (!valueToReturn) {
        printf("mythread: malloc error");
        return NULL;
    }

    *valueToReturn = 42;
    printf("Value to return from mythread: %d\n", *valueToReturn);
    return (void *)valueToReturn;

    // или
    // return (void *)42;
}

int main() {
    mythread_t tid;
    int err;
    int *thread_result;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    err = mythread_create(&tid, mythread, NULL);
    if (err) {
        printf("main: mythread_create() failed: %s\n", strerror(err));
        return ERROR;
    }

    err = mythread_join(tid, (void **)&thread_result);
    if (err) {
        printf("main: mythread_join() failed: %s\n", strerror(err));
        return ERROR;
    }

    printf("Received value in main: %d\n", *thread_result);

    // или
    // printf("Received value in main: %ld\n", (long)thread_result);

    free(thread_result);
    return SUCCESS;
}