#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define SIZE 32
#define NUM_THREADS 32

#define DEBUG

#define min(a, b) (((a) < (b)) ? (a) : (b))

pthread_barrier_t   barrier; // barrier synchronization object

int A[SIZE] = {58,  89,   32, 73,  131, 156, 30,  29, 
         141, 37,  133, 151, 88,  53,  122, 126,
         131, 49,  130, 115, 16,  83,  40,  145,
         10,  112, 20,  147, 14,  104, 111, 92};

int B[6][32]; // LOG2(SIZE) + 1 
int C[6][32]; // LOG2(SIZE) + 1 

int h; //global for STEP2
int i;
int prefix_minima[SIZE];
int suffix_minima[SIZE];


//initialize B[0,for all] = A[for all]
void *init(void *threadid)
{
	long tid;
	tid = (long)threadid;
    B[0][tid] = A[tid];
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

#ifdef DEBUGx	
printf("thread id  %d , comparing:  %d  &  %d  \n", tid, B[h-1][2*tid], B[h-1][2*tid+1]);	
#endif
}

void *combine(void *threadid)
{
	int i;
	long tid;
	tid = (long)threadid;
   
	if (tid == 0)
	{
		C[h][1] = B[h,0];
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

	printf ("Assignment A:  pthread Algorithm! \n" );

//STEP 1////////////////////////////////////////////////////////////////////////////////
//Create init all threads
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	for(t=0; t<NUM_THREADS; t++)
	{
		rc = pthread_create(&threads[t], NULL, init, (void *)t);
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


#ifdef DEBUG
	//Print init  results
	for (i = 0; i<SIZE; i++ )
	{
		printf("%d ", B[0][i]);
	}
	printf("\n");
#endif   
	
//STEP2////////////////////////////////////////////////////////////////////////////////

for (h=1; h<=5; h++)   // REPLACE WITH LOG2 here!!!
{

#ifdef DEBUGx
printf(" h = %d \n", h);
#endif

 //Create minimizing  threads
	
	n_threads = SIZE/(int)pow(2,h);
	
	pthread_barrier_init (&barrier, NULL, n_threads);

	for(t=0; t< n_threads; t++)
	{
		rc = pthread_create(&threads[t], NULL, minimum, (void *)t);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

//Join all threads
	for(t=0;  t<n_threads; t++)
	{
		rc = pthread_join(threads[t], NULL);
		if (rc)
		{
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
      	}
	}

#ifdef DEBUG
printf("\n n_threads = %d\t\n", n_threads);
for (i = 0; i<n_threads; i++ )
{
	printf("%d ", B[h][i]);
}
printf("\n");
#endif
}


//STEP3////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


#ifdef DEBUG
//Print all results
for (i = 0; i<SIZE; i++ )
{
	printf("%d ", B[0][i]);
}
printf("\n");
#endif   

	pthread_barrier_destroy(& barrier);
	pthread_exit(NULL);
	return 0 ;
}
