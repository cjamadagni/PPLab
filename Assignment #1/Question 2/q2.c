/******************************************************************************
* FILE: q2.c
* ASSIGNMENT: 1
* QUESTION: 2
* DESCRIPTION: A program to create 5 threads, and display a "Hello World"
*              message. Demonstrates passing of a threadID to each thread.  
*   
* 
* AUTHORS: Chirag Jamadagni (13CO117) & Abhijith Anilkumar (13CO102)  
******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define numOfThreads 5

void printHelloMessage(void* threadID) {
	long tid;
	tid = (long) threadID;
	printf(" Thread %ld : Hello World! \n", tid);
	pthread_exit(0);
}

int main() {
	 pthread_t threads[numOfThreads];
	 int errorCode;
	 long t;

	 for(t=0;t<numOfThreads;t++) {
	 	errorCode = pthread_create(&threads[t], NULL, (void *) printHelloMessage, (void *) t);

	 	if(errorCode) {
	 		printf("ERROR; return code from pthread_create() is %d\n", errorCode);
	 		exit(-1);
	 	}
	 }

	 return 0;
}