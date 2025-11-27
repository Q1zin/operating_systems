#include "./../mythread.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

void* simple_thread(void* arg, mythread_t thread) {
    int* value = (int*)arg;
    *value = 42;
    usleep(300000);
    return (void*)100;
}

void test1_create_and_join() {
    printf("\n\n\nТест 1\n");
    
    mythread_t tid;
    int value = 0;
    void* retval;
    printf("До mythread_create\n");
    assert(mythread_create(&tid, simple_thread, &value) == 0);
    printf("После mythread_create, перед mythread_join\n");
    assert(mythread_join(tid, &retval) == 0);
    printf("После mythread_join\n");
    assert(value == 42);
    assert((long)retval == 100);
}

void* counter_thread(void* arg, mythread_t thread) {
    int* counter = (int*)arg;
    for (int i = 0; i < 1000; i++) {
        __atomic_add_fetch(counter, 1, __ATOMIC_SEQ_CST);
    }
    return NULL;
}

void test2_multiple_threads() {
    printf("\n\n\nТест 2\n");
    int counter = 0;
    mythread_t threads[5];

    for (int i = 0; i < 5; i++) {
        assert(mythread_create(&threads[i], counter_thread, &counter) == 0);
    }

    for (int i = 0; i < 5; i++) {
        assert(mythread_join(threads[i], NULL) == 0);
    }

    assert(counter == 5000);
}

void* cancelable_thread(void* arg, mythread_t thread) {
    for (int i = 0; i < 10; i++) {
        mythread_testcancel(thread);
        printf("mythread_testcancel %d\n", i);
        usleep(10000);
    }
    return (void*)999;
}

void test3_cancel() {
    printf("\n\n\nТест 3\n");
    mythread_t tid;
    void* retval;
    
    assert(mythread_create(&tid, cancelable_thread, NULL) == 0);
    usleep(20000);
    
    mythread_cancel(tid);
    assert(mythread_join(tid, &retval) == 0);
    
    assert((long)retval == -1);
}

_Atomic int detached_finished = 0;

void* detached_thread(void* arg, mythread_t thread) {
    usleep(50000);
    atomic_store(&detached_finished, 1);
    return NULL;
}

void test4_detach() {
    printf("\n\n\nТест 4\n");
    atomic_store(&detached_finished, 0);
    mythread_t tid;
    assert(mythread_create(&tid, detached_thread, NULL) == 0);
    assert(mythread_detach(tid) == 0);
    printf("1 detached_finished: %d\n", detached_finished);
    while (!atomic_load(&detached_finished)) {
        printf("2 detached_finished: %d\n", detached_finished);
        usleep(10000);
    }
    printf("3 detached_finished: %d\n", detached_finished);
    usleep(50000);
    printf("4 detached_finished: %d\n", detached_finished);
}

void* self_thread(void* arg, mythread_t thread) {
    pid_t* tid_ptr = (pid_t*)arg;
    *tid_ptr = mythread_self();
    return NULL;
}

void test5_self() {
    printf("\n\n\nТест 5\n");
    pid_t main_tid = mythread_self();
    pid_t thread_tid = 0;
    mythread_t tid;
    
    assert(mythread_create(&tid, self_thread, &thread_tid) == 0);
    assert(mythread_join(tid, NULL) == 0);
    
    assert(main_tid != thread_tid);
    assert(thread_tid > 0);
    
    printf("main_tid=%d, thread_tid=%d\n\n", main_tid, thread_tid);
}

int main() {    
    test1_create_and_join();
    test2_multiple_threads();
    test3_cancel();
    test4_detach();
    test5_self();

    return 0;
}