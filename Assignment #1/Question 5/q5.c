/******************************************************************************
* FILE: q5.c
* ASSIGNMENT: 1
* QUESTION: 5
* DESCRIPTION: Program to accept two vectors and calculate the dot product. Dot 
*              product computation is split equally amongst 4 threads. Each 
*              thread acquires a mutex before writing to the result variable.
* 
* INPUT: - X: Vector where each index is 1
*        - Y: Vectpr where each index is 3
*
* Output: Dot Product
*
* NOTE: Simple input values chosen inorder to simplify output screenshot. By 
*		doing so we avoid the user entering ~40,000 values and printing the
*		input vectors in case of randomly generated input. 
*            
*
* AUTHORS: Chirag Jamadagni (13CO117) & Abhijith Anilkumar (13CO102)  
******************************************************************************/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Globally accessible structure
typedef struct {
	double *x; // vector 1
	double *y; // vector 2
	double sum; // dot product
	int vectorLength; // vector length
} DOTDATA;

// Globally accessible variables and mutex
#define numOfThreads 4 // number of threads
#define veclen 10000 // vector length  
DOTDATA dotprod;
pthread_t threads[numOfThreads];
pthread_mutex_t mutex;

// Function to compute the dot product
void *dotProduct (void *arg) {

	// Defining local variables
	int i, start, end, length;
	long offset;
	double sum, *x, *y;
	offset = (long) arg;

	length = dotprod.vectorLength;
	start = offset * length;
	end = start + length;
	x = dotprod.x;
	y = dotprod.y;

	// Performing dot product computation
	sum = 0;
	for(i=start; i<end; i++) {
		sum += (x[i]*y[i]);
	}

	// Acquiring mutex before updating data in global structure
	pthread_mutex_lock(&mutex);
	dotprod.sum += sum;
	printf("\n\nThread %ld computed sum between indices %d and %d",offset, start, end);
	printf("\n\tLocal sum = %f",sum);
	printf("\n\tGlobal sum = %f",dotprod.sum);

	// Releasing mutex
	pthread_mutex_unlock(&mutex);
	pthread_exit((void*) 0);

       

}

// Driver program for thread creation and displaying final result
int main() {

	long i;
	double *a, *b; // input vectors
	void *status;
	pthread_attr_t attr;

	// Allocating memory and initializing values of input vectors
	a = (double*) malloc(numOfThreads*veclen*sizeof(double));
	b = (double*) malloc(numOfThreads*veclen*sizeof(double));

	for(i=0; i<veclen*numOfThreads; i++) {
		a[i] = 1;
		b[i] = 3;
	}

	// Displaying input vectors
	printf("\n\nVector X consists of %d elements with value = 1", (veclen*numOfThreads));
	printf("\nVector Y consists of %d elements with value = 3", (veclen*numOfThreads));

	// Updating value of global structure
	dotprod.vectorLength = veclen;
	dotprod.x = a;
	dotprod.y = b;
	dotprod.sum = 0;

	// Initializing mutex
	pthread_mutex_init(&mutex, NULL);

	// Creating threads to perform the dot product calculation
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for(i=0; i<numOfThreads; i++)
		pthread_create(&threads[i], &attr, dotProduct, (void *)i);
	pthread_attr_destroy(&attr);


 	// Waiting for other threads to finish execution
 	for(i=0; i<numOfThreads; i++)
 		pthread_join(threads[i], &status);

 	// Printing final results
 	printf("\n\nDOT PRODUCT (X.Y) = %f\n\n",dotprod.sum);

 	// Memory cleanup
 	free(a);
 	free(b);
 	pthread_mutex_destroy(&mutex);
 	pthread_exit(NULL);
 }


         


