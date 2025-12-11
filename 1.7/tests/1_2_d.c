#define _GNU_SOURCE
#include <stdio.h>
#include "./../uthread.h"
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

#define SUCCESS 0
#define ERROR 1

void *mythread(void *arg) {
    int thread_num = *(int *)arg;
    printf("uthread [%d]: Hello from uthread!\n", thread_num);
    return NULL;
}

int main() {
    uthread_t tid;
    int err;
    int threads_num = 0;
    static int thread_ids[MAX_THREADS];

    printf("main: Hello from main!\n");

    while (threads_num < MAX_THREADS) {
        thread_ids[threads_num] = threads_num;
        err = uthread_create(&tid, mythread, &thread_ids[threads_num]);
        if (err) {
            printf("main: uthread_create() failed: %s\n", strerror(err));
            break;
        }
        threads_num++;
    }

    uthread_run();

    printf("main: Created %d threads\n", threads_num);
    return SUCCESS;
}