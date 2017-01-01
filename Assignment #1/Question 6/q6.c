/******************************************************************************
* FILE: q6.c
* ASSIGNMENT: 1
* QUESTION: 6
* DESCRIPTION: Program to demonstrate signalling using condition variables. Two
*              increment-threads increment a shared variable and this variable 
*              reaches a threshold value, a signal is sent to the third 
*              watch-count thread which locks the shared variable. When an 
*   		   increment-thread gives a signal, the watch-count thread releases
*		       the lock and terminates along with the two increment-threads					
* 
* 
******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define numOfThreads 3  // number of threads
#define thresholdCount 10  // threshold value of the shared variable 

int count = 0; // global variable to keep track of the count
pthread_mutex_t countMutex;
pthread_cond_t countThreshold;


// Function for increment-threads to increment the value of the shared variable
void *incrementCount(void *tid) 
{
  int i;
  long threadID = (long)tid;

  while (count != thresholdCount) {
    
  	// Lock mutex and update the count
    pthread_mutex_lock(&countMutex);
    count++;
    printf ("\nIncrement-thread %ld: Count incremented to %d.", threadID, count);

    // Checking if threshold value has been reached
    if (count == thresholdCount) {
    	printf("\nIncrement-thread %ld: Threshold value reached!", threadID);

    	// signalling the watch-count thread
    	pthread_cond_signal(&countThreshold);
    	printf("\nIncrement-thread %ld: Watch-count thread has been signalled.", threadID);
    }
    
	pthread_mutex_unlock(&countMutex);
    sleep(1);  // Adding sleep so threads can alternate on the mutex lock
   
  }

  pthread_exit(NULL); 
}

// Function for the watch-count thread to receive signal and unlock mutex
void *watchCount(void *tid) 
{
  long threadID = (long)tid;

  // locking mutex and waiting for signal
  pthread_mutex_lock(&countMutex);
  
  while (count < thresholdCount) {
    pthread_cond_wait(&countThreshold, &countMutex);
    printf("\n\nWatch-count thread %ld: Condition signal received. Count= %d", threadID,count);
  }
  
  printf("\nWatch-count thread %ld: Unlocking mutex.\n", threadID);
  pthread_mutex_unlock(&countMutex);
  pthread_exit(NULL);
}



// Driver function for thread creation
int main() {
	int i;
	int t1=1, t2=2, t3=3; // Assigning thread IDs
	pthread_t threads[3];
	pthread_attr_t attr;

	// Initializing mutex and condition variable
	pthread_mutex_init(&countMutex, NULL);
	pthread_cond_init (&countThreshold, NULL);		
  
	printf("\n\nThreshold value = %d\n", thresholdCount);

	// Creating threads
	pthread_attr_init(&attr);
  	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  	pthread_create(&threads[0], &attr, watchCount, (void *)t1);
  	pthread_create(&threads[1], &attr, incrementCount, (void *)t2);
  	pthread_create(&threads[2], &attr, incrementCount, (void *)t3);

  	// Waiting for threads to complete execution
  	for (i=0; i<numOfThreads; i++)
  		pthread_join(threads[i], NULL);

  	// Displaying result
  	printf("\n\nJoined all 3 threads.\nFinal value of shared variable = %d\n\n", count);	
  
  	// Clean up
  	pthread_attr_destroy(&attr);
  	pthread_mutex_destroy(&countMutex);
  	pthread_cond_destroy(&countThreshold);
  	pthread_exit (NULL);
}



