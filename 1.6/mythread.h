#ifndef MYTHREAD_H
#define MYTHREAD_H

#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <setjmp.h>
#include <stdatomic.h>

#define PAGE 4096
#define STACK_SIZE (PAGE * 8)

typedef struct _mythread mythread_struct_t;
typedef mythread_struct_t* mythread_t;
typedef void *(*start_routine_t)(void*, mythread_t);

struct  _mythread {
    int mythread_id;
    start_routine_t start_routine;
    void* arg;
    void* retval;
    void* stack;
    _Atomic int finished;
    _Atomic int joined;
    _Atomic int canceled;
    _Atomic int detached;
    jmp_buf exit;
};

int mythread_create(mythread_t* tid, start_routine_t routine, void* arg);
pid_t mythread_self(void);
int mythread_join(mythread_t tid, void **retval);
void mythread_cancel(mythread_t tid);
void mythread_testcancel(mythread_t thread);
int mythread_detach(mythread_t tid);

#endif