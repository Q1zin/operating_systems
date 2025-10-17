#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define SUCCESS 0
#define ERROR 1


void sigint_handler(int sig) {
    printf("\n[Поток 2] Получен сигнал SIGINT (%d)\n", sig);
    printf("[Поток 2] PID: %d, u: %u\n", getpid(), gettid());
}

void* thread1_func(void* arg) {
    sigset_t set;
    
    sigfillset(&set);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
    
    printf("[Поток 1] Запущен. Все сигналы заблокированы.\n");
    printf("[Поток 1] PID: %d, u: %u\n", getpid(), gettid());
    
    while(1) {
        printf("[Поток 1] Работаю...\n");
        sleep(5);
    }
    
    return NULL;
}

void* thread2_func(void* arg) {
    sigset_t set;
    struct sigaction sa = {0};
    
    printf("[Поток 2] Запущен. Ожидаю SIGINT (Ctrl+C).\n");
    printf("[Поток 2] PID: %d, u: %u\n", getpid(), gettid());
    
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_UNBLOCK, &set, NULL);

    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);

    while(1) {
        printf("[Поток 2] Работаю...\n");
        sleep(5);
    }
    
    return NULL;
}

void* thread3_func(void* arg) {
    sigset_t set;
    int sig;
    
    printf("[Поток 3] Запущен. Ожидаю SIGQUIT (Ctrl+\\).\n");
    printf("[Поток 3] PID: %d, u: %u\n", getpid(), gettid());
    
    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    while(1) {
        printf("[Поток 3] Вызываю sigwait()...\n");
        
        if (sigwait(&set, &sig) == 0) {
            printf("\n[Поток 3] Получен сигнал %d (SIGQUIT) через sigwait()\n", sig);
            printf("[Поток 3] PID: %d, u: %u\n", getpid(), gettid());
        } else {
            perror("sigwait");
        }
        
        sleep(1);
    }
    
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    sigset_t set;
    
    printf("PID: %d\n", getpid());
    
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
    
    printf("Ctrl+C = SIGINT (поток 2)\n");
    printf("Ctrl+\\ = SIGQUIT (поток 3)\n");

    int err = pthread_create(&t1, NULL, thread1_func, NULL);
    if (err != 0) {
        printf("failed pthread_create thread1");
        return ERROR;
    }

    err = pthread_create(&t2, NULL, thread2_func, NULL);
    if (err != 0) {
        printf("failed pthread_create thread2");
        return ERROR;
    }

    err = pthread_create(&t3, NULL, thread3_func, NULL);
    if (err != 0) {
        printf("failed pthread_create thread3");
        return ERROR;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    
    return SUCCESS;
}