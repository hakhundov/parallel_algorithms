#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define SIZE 32
#define NUM_THREADS 32

#define min(a, b) (((a) < (b)) ? (a) : (b))

pthread_barrier_t   barrier; // barrier synchronization object

int A[SIZE] = {58,  89,   32, 73,  131, 156, 30,  29, 
         141, 37,  133, 151, 88,  53,  122, 126,
         131, 49,  130, 115, 16,  83,  40,  145,
         10,  112, 20,  147, 14,  104, 111, 92};

int B[6][32]; // LOG2(SIZE) + 1 
int C[6][32]; // LOG2(SIZE) + 1 
int S[6][32]; // LOG2(SIZE) + 1 


int h; //global for STEP2&3
void execute(int num_threads, void * (*func)(void *)) {
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	for(t=0; t<num_threads; t++)
	{
		rc = pthread_create(&threads[t], NULL, func, (void *)t);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	//Join all threads
	for(t=0; t<num_threads; t++)
	{
		rc = pthread_join(threads[t], NULL);
		if (rc)
		{
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
      	}
	}
}

//initialize B[0,for all] = A[for all]
void *init(void *threadid)
{
	long tid;
	tid = (long)threadid;
    B[0][tid] = A[tid];
	pthread_exit(NULL);
}

void *R_init(void *threadid)
{
	long tid;
	tid = (long)threadid;
    B[0][SIZE-tid-1] = A[tid];
	pthread_exit(NULL);
}

//balanced tree method
void *minimum(void *threadid)
{
	long tid;
	tid = (long)threadid;
    B[h][tid] = min (  B[h-1][2*tid], B[h-1][2*tid+1]  );
	pthread_barrier_wait(&barrier);
	pthread_exit(NULL);
}

void *combine(void *threadid)
{
	int i;
	long tid;
	tid = (long)threadid;
   
	if (tid == 0)
	{
		C[h][0] = B[h][0];
	}
	else if (tid%2 == 0)
	{
		C[h][tid] = min( C[h+1][(tid-1)/2], B[h][tid]  );
	}
	else
	{
		C[h][tid] = C[h+1][tid/2];
	}

	pthread_barrier_wait(&barrier);
	pthread_exit(NULL);
}


int main (int argc, char ** argv)
{
	int i;
	int n_threads;

	pthread_t threads[NUM_THREADS];
	printf ("Assignment A:  pthread Algorithm! \n" );

//STEP 1
	execute(NUM_THREADS, init);
	
//STEP2
	for (h=1; h<=5; h++)   // REPLACE WITH LOG2 here!!!
	{
		n_threads = SIZE/(int)pow(2,h);
		pthread_barrier_init (&barrier, NULL, n_threads);
		execute(n_threads,minimum);
	}

//STEP3
	for (h=5; h>=0; h--)   // REPLACE WITH LOG2 here!!!
	{
		n_threads = SIZE/(int)pow(2,h);
		pthread_barrier_init (&barrier, NULL, n_threads);
		execute(n_threads,combine);
	}

//Print all results
	printf("prefix minimum is: \n");
	for (i = 0; i<SIZE; i++ )
	{
		printf("%d ", C[0][i]);
	}
	printf("\n");




//DO THE PREFIX NOW, by rotating

//STEP 1
	execute(NUM_THREADS, R_init); //<--- REVERSE INITIALIZATION
	
//STEP2
	for (h=1; h<=5; h++)   // REPLACE WITH LOG2 here!!!
	{
		n_threads = SIZE/(int)pow(2,h);
		pthread_barrier_init (&barrier, NULL, n_threads);
		execute(n_threads,minimum);
	}

//STEP3
	for (h=5; h>=0; h--)   // REPLACE WITH LOG2 here!!!
	{
		n_threads = SIZE/(int)pow(2,h);
		pthread_barrier_init (&barrier, NULL, n_threads);
		execute(n_threads,combine);
	}


//Print all results
	printf("suffix minimum  is: \n");
	for (i = 0; i<SIZE; i++ )
	{
		printf("%d ", C[0][SIZE-i-1]);
	}
	printf("\n");



 

	pthread_barrier_destroy(& barrier);
	pthread_exit(NULL);
	return 0 ;
}
