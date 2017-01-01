/******************************************************************************
* FILE: q7.c
* ASSIGNMENT: 1
* QUESTION: 7
* DESCRIPTION: Program to generate two matrices and calculate the time taken for
*              performing the matrix multiplication. The computation of product
*              is split equally amongst the number of threads availvable.
*
* INPUT: - X: Randomly generated matrix
*        - Y: Randomly generated matrix
*
* Output: Execution time and speed-up value
*
*
* 
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define matlen 250

// Global variables
int numOfThreads; // number of threads
long length;
double x[matlen][matlen]; // matrices
double y[matlen][matlen];
double p[matlen][matlen];
clock_t begin, stop; // clock variables
double cpuTimeUsed;

// Function to multiply the matrices
void *matMultiply(void* arg) {

    // Defining local variables
   	int i, j, k, start, end;
   	long offset;
    double sum=0;
   	offset = (long) arg;

   	start = offset * length;
   	end = start + length;

   	// Performing DAXPY computation
   	for(i=start; i<end; i++) {
      for(j=0; j<matlen; j++){
        for (k=0; k<matlen; k++) {
          sum += x[i][k]*y[k][j];
   	    }
        p[i][j] = sum;
        sum = 0;
      }
    }

   	printf("\n\nThread %ld computed sum between rows %d and %d",offset, start, end);

   	pthread_exit((void*) 0);
}

void matrixGen(double x[][matlen], int m) {

   int i, j;
   for (i = 0; i < m; i++)
      for (j = 0; j < m; j++)
         x[i][j] = random()/((double) RAND_MAX);
}

// Driver program for thread creation and displaying final result
int main(int argc, char* argv[]) {

   long i;
   numOfThreads = atoi(argv[1]);
   pthread_t threads[numOfThreads];

   length = matlen/numOfThreads;

   printf("\nIteration %d\n\nNumber of Threads : %d", numOfThreads, numOfThreads);

   srandom(1);
   matrixGen(x, matlen);
   printf("Generated Matrix 1 of size %dX%d\n", matlen, matlen);
   matrixGen(y, matlen);
   printf("Generated Matrix 2 of size %dX%d\n", matlen, matlen);


   begin = clock();
   // Creating threads to perform the dot product calculation
   for (i=0; i<numOfThreads; i++)
      pthread_create(&threads[i], NULL, matMultiply, (void*)i);

   // Waiting for other threads to finish execution
   for (i=0; i<numOfThreads; i++)
      pthread_join(threads[i], NULL);
   stop = clock();

   cpuTimeUsed = ((double) (stop - begin)) / CLOCKS_PER_SEC;

 	 printf("\n\nTime taken : %lf\n", cpuTimeUsed);
   printf("\nSpeed-Up = %lf\n", cpuTimeUsed/0.0544);
   printf("\n-----------------------------------------------------------------------------\n");

   return 0;
}
