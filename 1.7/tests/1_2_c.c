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
    char* valueToReturn = "hello world\0";
    return (void *)valueToReturn;
}

int main() {
    uthread_t tid;
    int err;
    char *thread_result;

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

    printf("Received value in main: %s\n", thread_result);

    return SUCCESS;
}