#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void cleanup(void *arg) {
    char *p = (char *)arg;
    if (p) {
        free(p);
    }
    printf("cleanup: freed memory\n");
}

void* worker(void *arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    const char *msg = "hello world";
    size_t len = strlen(msg) + 1;
    char *buf = (char *)malloc(len);
    if (!buf) {
        printf("malloc failed\n");
        return NULL;
    }
    memcpy(buf, msg, len);

    pthread_cleanup_push(cleanup, buf);

    while (1) {
        printf("%s\n", buf);
        sleep(1);
    }

    pthread_cleanup_pop(1);
    return NULL;
}

int main(void) {
    pthread_t tid;

    if (pthread_create(&tid, NULL, worker, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    sleep(3);

    puts("main: cancel thread...");
    pthread_cancel(tid);
    
    pthread_join(tid, NULL);
    puts("main: done");
    return 0;
}
