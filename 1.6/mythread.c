#include "mythread.h"

static void* create_stack(size_t size) {
    return NULL;
}

static void free_stack(void* stack, size_t size) {
    return NULL;
}

static int thread_wrapper(void* arg) {
    return 0;
}

int mythread_create(mythread_t* tid, start_routine_t routine, void* arg) {
    return 0;
}

pid_t mythread_self(void) {
    return (pid_t)syscall(SYS_gettid);
}

void mythread_cancel(mythread_t tid) {
    
}

void mythread_testcancel(mythread_t thread) {
    
}

int mythread_join(mythread_t tid, void** retval) {

    return 0;
}

int mythread_detach(mythread_t tid) {
    return 0;
}