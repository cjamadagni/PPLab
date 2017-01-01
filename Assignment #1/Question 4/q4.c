/******************************************************************************
* FILE: q4.c
* ASSIGNMENT: 1
* QUESTION: 4
* DESCRIPTION: Program to accept two vectors and calculate the time taken for
*              Performing the DAXPY loop. The product computation
*              is split equally amongst the number of threads availvable.
*
* INPUT: - X: Vector where each index is 1
*        - Y: Vector where each index is 3
*        - A: Scalar with value 4
*
* Output: Execution time and speed-up value
*
* NOTE: Simple input values chosen inorder to simplify output screenshot. By
*		doing so we avoid the user entering ~130,000 values and printing the
*		input vectors in case of randomly generated input.
*
*
* 
******************************************************************************/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define veclen 65536 // vector length

// Globally accessible variables and mutex
int numOfThreads; // number of threads
long length;
double *x;
double *y;
double a;
clock_t begin, stop; // clock variables
double cpuTimeUsed;

// Function to compute DAXPY
void *DAXPY (void *arg) {

	// Defining local variables
	int i, start, end;
	long offset;
	offset = (long) arg;

	start = offset * length;
	end = start + length;

	// Performing DAXPY computation
	for(i=start; i<end; i++) {
		x[i] += a*x[i] + y[i];
	}

	printf("\n\nThread %ld computed sum between indices %d and %d",offset, start, end);

	pthread_exit((void*) 0);
}

// Driver program for thread creation and displaying final result
int main(int argc, char *argv[]) {

	long i;
	void *status;
	double *p, *q;
	numOfThreads = atoi(argv[1]);
	pthread_t threads[numOfThreads];
	pthread_attr_t attr;

	// Allocating memory and initializing values of input vectors
	p = (double*) malloc(veclen*sizeof(double));
	q = (double*) malloc(veclen*sizeof(double));

	for(i=0; i<veclen; i++) {
		p[i] = 1;
		q[i] = 3;
	}

	printf("\nIteration %d\n\nNumber of Threads : %d", numOfThreads, numOfThreads);

	// Displaying input vectors
	printf("\n\nVector X consists of %d elements with value = 1", (veclen));
	printf("\nVector Y consists of %d elements with value = 3", (veclen));

  // Updating value of global structure
	x = p;
	y = q;
	a = 4;
	length = veclen/numOfThreads;

	begin = clock();
	// Creating threads to perform the dot product calculation
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for(i=0; i<numOfThreads; i++)
		pthread_create(&threads[i], &attr, DAXPY, (void *)i);
	pthread_attr_destroy(&attr);


 	// Waiting for other threads to finish execution
	for(i=0; i<numOfThreads; i++)
		pthread_join(threads[i], &status);
	stop = clock();
	cpuTimeUsed = ((double) (stop - begin)) / CLOCKS_PER_SEC;

	printf("\n\nTime taken : %lf\n", cpuTimeUsed);
	printf("\nSpeed-Up = %lf\n", cpuTimeUsed/0.000039);
	printf("\n-----------------------------------------------------------------------------\n");

 	// Memory cleanup
	free(p);
	free(q);
	pthread_exit(NULL);
}
