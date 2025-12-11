#define _GNU_SOURCE
#include <stdio.h>
#include "./../uthread.h"
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR 1

void *mythread(void *arg) {
    (void)arg;
    int *valueToReturn = malloc(sizeof(int));
    if (!valueToReturn) {
        printf("mythread: malloc error");
        return NULL;
    }

    *valueToReturn = 42;
    printf("Value to return from uthread: %d\n", *valueToReturn);
    return (void *)valueToReturn;
}

int main() {
    uthread_t tid;
    int err;
    int *thread_result;

    printf("main: Hello from main!\n");

    err = uthread_create(&tid, mythread, NULL);
    if (err) {
        printf("main: uthread_create() failed: %s\n", strerror(err));
        return ERROR;
    }

    uthread_run();

    err = uthread_join(tid, (void **)&thread_result);
    if (err) {
        printf("main: uthread_join() failed: %s\n", strerror(err));
        return ERROR;
    }

    printf("Received value in main: %d\n", *thread_result);

    free(thread_result);
    return SUCCESS;
}