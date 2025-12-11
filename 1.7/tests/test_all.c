#include "./../uthread.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

void* simple_thread(void* arg) {
    int* value = (int*)arg;
    *value = 42;
    uthread_sleep(1);
    return (void*)100;
}

void test1_create_and_join() {
    printf("\n=== Тест 1: Создание и join ===\n");
    
    uthread_t tid;
    int value = 0;
    void* retval;
    printf("До uthread_create\n");
    assert(uthread_create(&tid, simple_thread, &value) == 0);
    printf("После uthread_create, запуск планировщика\n");
    uthread_run();
    printf("После uthread_run, перед uthread_join\n");
    assert(uthread_join(tid, &retval) == 0);
    printf("После uthread_join\n");
    assert(value == 42);
    assert((long)retval == 100);
    printf("Тест 1 пройден!\n");
}

void* counter_thread(void* arg) {
    int* counter = (int*)arg;
    for (int i = 0; i < 1000; i++) {
        __atomic_add_fetch(counter, 1, __ATOMIC_SEQ_CST);
        if (i % 100 == 0) {
            uthread_yield();
        }
    }
    return NULL;
}

void test2_multiple_threads() {
    printf("\n=== Тест 2: Несколько потоков ===\n");
    int counter = 0;
    uthread_t threads[5];

    for (int i = 0; i < 5; i++) {
        assert(uthread_create(&threads[i], counter_thread, &counter) == 0);
    }

    uthread_run();

    for (int i = 0; i < 5; i++) {
        assert(uthread_join(threads[i], NULL) == 0);
    }

    assert(counter == 5000);
    printf("Счётчик: %d\n", counter);
    printf("Тест 2 пройден!\n");
}

void* sleeping_thread(void* arg) {
    int id = *(int*)arg;
    printf("Поток %d: начало работы\n", id);
    uthread_sleep(1);
    printf("Поток %d: проснулся после sleep\n", id);
    return (void*)(long)id;
}

void test3_sleep() {
    printf("\n=== Тест 3: uthread_sleep ===\n");
    uthread_t threads[3];
    int ids[3] = {1, 2, 3};
    
    for (int i = 0; i < 3; i++) {
        assert(uthread_create(&threads[i], sleeping_thread, &ids[i]) == 0);
    }
    
    uthread_run();
    
    for (int i = 0; i < 3; i++) {
        void* retval;
        assert(uthread_join(threads[i], &retval) == 0);
        printf("Поток %d завершился\n", (int)(long)retval);
    }
    printf("Тест 3 пройден!\n");
}

void* yielding_thread(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; i++) {
        printf("Поток %d: итерация %d\n", id, i);
        uthread_yield();
    }
    return NULL;
}

void test4_yield() {
    printf("\n=== Тест 4: uthread_yield ===\n");
    uthread_t threads[3];
    int ids[3] = {1, 2, 3};
    
    for (int i = 0; i < 3; i++) {
        assert(uthread_create(&threads[i], yielding_thread, &ids[i]) == 0);
    }
    
    uthread_run();
    
    for (int i = 0; i < 3; i++) {
        assert(uthread_join(threads[i], NULL) == 0);
    }
    printf("Тест 4 пройден!\n");
}

int main() {    
    test1_create_and_join();
    test2_multiple_threads();
    test3_sleep();
    test4_yield();

    printf("\n=== Все тесты пройдены! ===\n");
    return 0;
}