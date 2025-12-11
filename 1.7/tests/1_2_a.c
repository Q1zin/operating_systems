#define _GNU_SOURCE
#include <stdio.h>
#include "./../uthread.h"
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 1

void *mythread(void *arg) {
    (void)arg;
    printf("uthread: Hello from uthread!\n");
    return NULL;
}

int main() {
    uthread_t tid;
    int err;

    printf("main: Hello from main!\n");

    err = uthread_create(&tid, mythread, NULL);
    if (err) {
        printf("main: uthread_create() failed: %s\n", strerror(err));
        return ERROR;
    }

    uthread_run();

    err = uthread_join(tid, NULL);
    if (err) {
        printf("main: uthread_join() failed: %s\n", strerror(err));
        return ERROR;
    }

    return SUCCESS;
}