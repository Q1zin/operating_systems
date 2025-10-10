#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void *mythread(void *arg) {
	printf("mythread [%d %d %p]: Hello from mythread!\n", getpid(), getppid(), (void*)pthread_self());
	return NULL;
}

int main() {
	pthread_t tid;
	int err;

	printf("main [%d %d %p]: Hello from main!\n", getpid(), getppid(), (void*)pthread_self());

	err = pthread_create(&tid, NULL, mythread, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	pthread_exit(NULL);
	// pthread_join(tid, NULL);

	return 0;
}

