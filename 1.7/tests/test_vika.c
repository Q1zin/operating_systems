#include <stdio.h>
#include "../uthread.h"

void recursive_func(int depth, int thread_id);

void* counter_thread(void *arg) {
    int thread_id = *(int*)arg;
    
    for (int i = 0; i < 5; i++) {
        printf("Поток %d: шаг %d\n", thread_id, i);
        uthread_yield();
    }
    recursive_func(20, thread_id);
    
    printf("Поток %d завершен\n", thread_id);
    return (void*)(long)(thread_id * 10);
}

void recursive_func(int depth, int thread_id) {
    char buf[512];
    snprintf(buf, sizeof(buf), "поток %d, глубина %d", thread_id, depth);

    if (depth > 0) {
        recursive_func(depth - 1, thread_id);
    }
}

int main() {
    uthread_t t1, t2;
    int id1 = 1, id2 = 2;
    void *ret1, *ret2;
        
    uthread_create(&t1, counter_thread, &id1);
    uthread_create(&t2, counter_thread, &id2);

    uthread_run();
    
    uthread_join(t1, &ret1);
    uthread_join(t2, &ret2);
    
    printf("\nРезультаты\n");
    printf("Поток 1 вернул: %ld\n", (long)ret1);
    printf("Поток 2 вернул: %ld\n", (long)ret2);
    
    return 0;
}