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
    struct myStruct *data = (struct myStruct *)arg;

    printf("mythread [tid: %d]: number = %d, message = %s\n", gettid(), data->number, data->message);

    free(data->message);
    free(data);
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_attr_t attr;
    int err;

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

    struct myStruct *data = malloc(sizeof *data);
    if (!data) {
        printf("malloc failed: %s\n", strerror(errno));
        pthread_attr_destroy(&attr); 
        return ERROR; 
    }
    data->number = 228;
    data->message = strdup("test message to thread!!!");
    if (!data->message) {
        printf("strdup failed: %s\n", strerror(errno));
        free(data); 
        pthread_attr_destroy(&attr); 
        return 1; 
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

    pthread_exit(NULL);

    return SUCCESS;
}