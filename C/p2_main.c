#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 		16
#define NUM_THREADS     16
//#define VERBOSE

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
   
	if (A[tid] != 0) distance[tid] = 1; //init distances
	else 			 distance[tid] = 0; //root node	
	A[tid] = A[tid]-1; //indicies correction
	
    while (A[tid] != -1)
		{
			distance[tid] = distance[tid] + distance[A[tid]];
			A[tid] = A[A[tid]];
			pthread_barrier_wait(&barrier);
		}
	pthread_exit(NULL);
}

int main (int argc, char ** argv)
{
#ifdef VERBOSE
	printf ("Assignment C:  pthread Algorithm! \n" );
#endif	
	int i;
	struct timeval startt, endt, result;
  	result.tv_sec = 0;
  	result.tv_usec= 0;

	pthread_barrier_init (&barrier, NULL, NUM_THREADS);
//
gettimeofday (&startt, NULL);
//
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
//
gettimeofday (&endt, NULL);
//

#ifdef VERBOSE
	//Print all results
	for (i = 0; i<SIZE; i++ )
	{
		printf("%d ", distance[i]);
	}
	printf("\n");
#endif

	result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
	printf("%ld", result.tv_usec);

	pthread_barrier_destroy(& barrier);
	pthread_exit(NULL);
	return 0 ;
}
