#define _GNU_SOURCE
#include <stdio.h>
#include "./../../mythread.h"
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 1

void *mythread() {
    printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
    return NULL;
}

int main() {
    mythread_t tid;
    int err;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    err = mythread_create(&tid, mythread, NULL);
    if (err) {
        printf("main: mythread_create() failed: %s\n", strerror(err));
        return ERROR;
    }

    err = mythread_join(tid, NULL);
    if (err) {
        printf("main: mythread_join() failed: %s\n", strerror(err));
        return ERROR;
    }

    return SUCCESS;
}