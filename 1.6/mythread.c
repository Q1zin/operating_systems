#include "mythread.h"
#include <stdint.h>

static inline int futex_wait(int *uaddr, int val) {
    return syscall(SYS_futex, uaddr, FUTEX_WAIT, val, NULL, NULL, 0);
}

static inline int futex_wake(int *uaddr, int n) {
    return syscall(SYS_futex, uaddr, FUTEX_WAKE, n, NULL, NULL, 0);
}

static void* create_stack(size_t size) {
    void* stack = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED) {
        perror("mmap failed");
        return NULL;
    }

    if (mprotect(stack, PAGE, PROT_NONE) == -1) {
        perror("mprotect failed");
        munmap(stack, size);
        return NULL;
    }

    return stack;
}

static void free_stack(void* stack, size_t size) {
    if (stack) {
        munmap(stack, size);
    }
} 

static int thread_wrapper(void* arg) {
    mythread_t thread = (mythread_t)arg;

    if (!atomic_load(&thread->canceled)) {
        if (setjmp(thread->exit) == 0) {
            thread->retval = thread->start_routine(thread->arg, thread);
        } else {
            thread->retval = ((void*)-1);
        }
    } else {
        thread->retval = ((void*)-1);
    }

    atomic_store(&thread->finished, 1);
    futex_wake((int*)&thread->finished, 1);

    while (!atomic_load(&thread->joined) && !atomic_load(&thread->detached)) {
        usleep(1000);
    }

    if (atomic_load(&thread->detached)) {
        free_stack(thread->stack, STACK_SIZE);
    }

    atomic_store(&thread->finished, 2);
    futex_wake((int*)&thread->finished, 1);

    return 0;
}

int mythread_create(mythread_t* tid, start_routine_t routine, void* arg) {
    if (tid == NULL || routine == NULL) {
        errno = EINVAL;
        return -1;
    }

    void* stack = create_stack(STACK_SIZE);
    if (!stack) {
        return -1;
    }

    mythread_t thread = (mythread_t)(stack + STACK_SIZE - sizeof(mythread_struct_t));

    thread->start_routine = routine;
    thread->arg = arg;
    thread->retval = NULL;
    thread->stack = stack;
    atomic_store(&thread->finished, 0);
    atomic_store(&thread->joined, 0);
    atomic_store(&thread->canceled, 0);
    atomic_store(&thread->detached, 0);

    void* stack_top = (void*)thread;
    stack_top = (void*)((uintptr_t)stack_top & ~15UL);

    int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD;
    thread->mythread_id = clone(thread_wrapper, stack_top, flags, thread);

    if (thread->mythread_id == -1) {
        perror("clone failed");
        free_stack(stack, STACK_SIZE);
        return -1;
    }

    *tid = thread;
    return 0;
}

pid_t mythread_self(void) {
    return (pid_t)syscall(SYS_gettid);
}

void mythread_cancel(mythread_t tid) {
    if (tid == NULL) {
        return;
    }

    atomic_store(&tid->canceled, 1);
}

void mythread_testcancel(mythread_t thread) {
    if (thread && atomic_load(&thread->canceled)) {
        longjmp(thread->exit, 1);
    }
}

int mythread_join(mythread_t tid, void** retval) {
    if (tid == NULL) {
        errno = EINVAL;
        return -1;
    }

    int expected = 0;
    if (!atomic_compare_exchange_strong(&tid->joined, &expected, 1)) {
        errno = EINVAL;
        return -1;
    }

    if (atomic_load(&tid->detached)) {
        errno = EINVAL;
        return -1;
    }

    int state;
    while ((state = atomic_load(&tid->finished)) < 1) {
        futex_wait((int*)&tid->finished, 0);
    }

    if (retval) {
        *retval = tid->retval;
    }

    while ((state = atomic_load(&tid->finished)) < 2) {
        futex_wait((int*)&tid->finished, 1);
    }

    free_stack(tid->stack, STACK_SIZE);

    return 0;
}

int mythread_detach(mythread_t tid) {
    if (tid == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (atomic_load(&tid->joined)) {
        errno = EINVAL;
        return -1;
    }

    atomic_store(&tid->detached, 1);
    return 0;
}