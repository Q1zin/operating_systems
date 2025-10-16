#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h>
#include <unistd.h>
#include <signal.h>

#define SUCCESS 0
#define ERROR 1

static _Atomic unsigned long long counter = 0;
volatile sig_atomic_t wake = 0;

void signal_handler(int sig) {
    printf("run handler %d\n", sig);
    wake = 1;
}

void* worker(void* arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;
    sigaction(SIGUSR1, &sa, NULL);

    pthread_sigmask(SIG_UNBLOCK, &set, NULL);

    while (!wake) {
        counter++;
    }

    return NULL;
}

int main(void) {
    pthread_t tid;
    pthread_create(&tid, NULL, worker, NULL);

    sleep(2);
    printf("main: kill...\n");
    pthread_kill(tid, SIGUSR1);

    printf("main: join...\n");
    pthread_join(tid, NULL);
    printf("main: finished, counter=%llu\n", counter);
    return 0;
}
