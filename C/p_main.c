#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 			16
#define NUM_THREADS     16

int A[SIZE] = {14, 13, 5, 16,
			   11, 10, 9, 12,
			   0, 8, 7, 15,
			   4, 3, 2, 1};

int distance[SIZE];

pthread_barrier_t   barrier; // barrier synchronization object

void *jump(void *threadid)
{
	int i;
	long tid;
	tid = (long)threadid;
   
	for (i = 0; i < 5; i++)
	{
    	if (i==0) //first step inits
		{
			if (A[tid] != 0) distance[tid] = 1; //init distances
			else 			 distance[tid] = 0; //root node	
			A[tid] = A[tid]-1; //indicies correction
		}
    	else if (A[tid] != -1)
		{
			distance[tid] = distance[tid] + distance[A[tid]];
			A[tid] = A[A[tid]];
		}
		pthread_barrier_wait(&barrier);
	}
	pthread_exit(NULL);
}

int main (int argc, char ** argv)
{
	int i;
	printf ("Assignment C:  Sequential Algorithm! \n" );
	pthread_barrier_init (&barrier, NULL, NUM_THREADS);

//Create all threads
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	for(t=0; t<NUM_THREADS; t++)
	{
		rc = pthread_create(&threads[t], NULL, jump, (void *)t);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

//Join all threads
	for(t=0; t<NUM_THREADS; t++)
	{
		rc = pthread_join(threads[t], NULL);
		if (rc)
		{
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
      	}
	}

	//Print all results
	for (i = 0; i<SIZE; i++ )
	{
		printf("%d ", distance[i]);
	}
	printf("\n");
   
	pthread_barrier_destroy(& barrier);
	pthread_exit(NULL);
	return 0 ;
}
