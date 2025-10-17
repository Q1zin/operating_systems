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
	pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
    pthread_t tid5;
	int err;

	printf("main [%d %d %p]: Hello from main!\n", getpid(), getppid(), (void*)pthread_self());

	err = pthread_create(&tid1, NULL, mythread, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

    err = pthread_create(&tid2, NULL, mythread, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

    err = pthread_create(&tid3, NULL, mythread, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

    err = pthread_create(&tid4, NULL, mythread, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

    err = pthread_create(&tid5, NULL, mythread, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	pthread_exit(NULL);
	// pthread_join(tid1, NULL);
    // pthread_join(tid2, NULL);
    // pthread_join(tid3, NULL);
    // pthread_join(tid4, NULL);
    // pthread_join(tid5, NULL);

	return 0;
}

