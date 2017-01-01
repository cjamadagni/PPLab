/******************************************************************************
* FILE: q1.c
* DESCRIPTION:
*   A "Hello World" Pthreads program.  Demonstrates creation and
*   termination of 5 threads.
* 
* 
******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define numOfThreads 5  // number of threads

// Function to print hello message
void printHelloMessage() {
	printf("Hello World! \n");
	pthread_exit(0);
}

// Driver function to create threads
int main() {
	 pthread_t threads[numOfThreads];
	 int errorCode;
	 long t;

	 for(t=0;t<numOfThreads;t++) {
	 	errorCode = pthread_create(&threads[t], NULL, (void *) &printHelloMessage, NULL);

	 	if(errorCode) {
	 		printf("ERROR; return code from pthread_create() is %d\n", errorCode);
	 		exit(-1);
	 	}
	 }

	 return 0;
}