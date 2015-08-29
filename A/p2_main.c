#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

#define SIZE 32
#define NUM_THREADS 2 
//#define VERBOSE

#define min(a, b) (((a) < (b)) ? (a) : (b))

pthread_barrier_t   barrier; // barrier synchronization object


int A[SIZE] = {58,  89,   32, 73,  131, 156, 30,  29, 
         141, 37,  133, 151, 88,  53,  122, 126,
         131, 49,  130, 115, 16,  83,  40,  145,
         10,  112, 20,  147, 14,  104, 111, 92};


int B[6][32]; // LOG2(SIZE) + 1 
int C[6][32]; // LOG2(SIZE) + 1 
int S[6][32]; // LOG2(SIZE) + 1 


/*
//experimenting with larger sizes

#undef SIZE
#define SIZE 8192

int A[SIZE];
int B[14][8192]; // LOG2(SIZE) + 1 
int C[14][8192]; // LOG2(SIZE) + 1 
int S[14][8192]; // LOG2(SIZE) + 1 
*/

int n_threads;
int chunk;
int h; //global for STEP2&3
void execute(int num_threads, void * (*func)(void *)) {
	pthread_t threads[SIZE];
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
   	int i;
	for (i=0; i<chunk; i++)
	{	 
		B[0][tid*chunk+i] = A[tid*chunk+i];
	//	printf("tid is %d, i is %d, index is %d\n",tid, i, tid*chunk+i);
	}
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
	int i;
	tid = (long)threadid;

	if (n_threads<=NUM_THREADS) {
		B[h][tid] = min (  B[h-1][2*tid], B[h-1][2*tid+1]  );
	}
	else {
		for (i=0; i<chunk; i++)
		{	
 			B[h][tid*chunk+i] = min (  B[h-1][2*(tid*chunk+i)], B[h-1][2*(tid*chunk+i)+1]  );
		}
	}
    //printf("h is %d   tid %d\n",h, tid);
	pthread_barrier_wait(&barrier);
	pthread_exit(NULL);
}

void *combine(void *threadid)
{
	int i;
	long tid;
	tid = (long)threadid;
   
	if (n_threads<=NUM_THREADS) {
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
	}
	else {
		for (i=0; i<chunk; i++)
		{
			if (tid*chunk+i == 0)
			{
				C[h][0] = B[h][0];
			}
			else if ((tid*chunk+i)%2 == 0)
			{
				C[h][(tid*chunk+i)] = min( C[h+1][((tid*chunk+i)-1)/2], B[h][(tid*chunk+i)]  );
			}
			else
			{
				C[h][tid*chunk+i] = C[h+1][(tid*chunk+i)/2];
			}
		}
	}
	pthread_barrier_wait(&barrier);
	pthread_exit(NULL);
}

int main (int argc, char ** argv)
{
#ifdef VERBOSE
		printf ("Assignment A:  pthread Algorithm! \n" );
#endif

	int i;

chunk = SIZE/NUM_THREADS;

/*
srand(42);

for (i=0; i<8192; i++)
{
	A[i] = rand()%1000+20;
}
*/

	struct timeval startt, endt, result1, result2;
	result1.tv_sec = 0;
	result1.tv_usec= 0;
	result2.tv_sec = 0;
	result2.tv_usec= 0;

	pthread_t threads[NUM_THREADS];

//
gettimeofday(&startt, NULL);
//

//STEP 1
	execute(NUM_THREADS, init);

//STEP2
	for (h=1; h<=log2(SIZE); h++)   // REPLACE WITH LOG2 here!!!
	{
		n_threads = SIZE/(int)pow(2,h);
		pthread_barrier_init (&barrier, NULL, NUM_THREADS);
		execute(NUM_THREADS, minimum); //changed here
#ifdef DEBUG
for (i = 0 ; i< n_threads; i++)
printf("%d ", B[h][i]);
printf("\n");
#endif
	}

//STEP3
	for (h=log2(SIZE); h>=0; h--)   // REPLACE WITH LOG2 here!!!
	{
		n_threads = SIZE/(int)pow(2,h);
		pthread_barrier_init (&barrier, NULL, NUM_THREADS);
		execute(NUM_THREADS,combine);
#ifdef DEBUG
for (i = 0 ; i< n_threads; i++)
{
printf("%d ", C[h][i]);
}
printf("\n");
#endif
	}

#ifdef VERBOSE
//Print all results
	printf("prefix minimum is: \n");
	for (i = 0; i<SIZE; i++ )
	{
		printf("%d ", C[0][i]);
	}
	printf("\n");
#endif

//
gettimeofday(&endt, NULL);
//
result1.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);

return 0; ////////////////

//
gettimeofday(&startt, NULL);
//
//DO THE PREFIX NOW, by rotating

//STEP 1
	execute(32, R_init); //<--- REVERSE INITIALIZATION
	
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
        
        printf("**** h is %d \n", h);
		n_threads = SIZE/(int)pow(2,h);
		pthread_barrier_init (&barrier, NULL, n_threads);
		execute(n_threads,combine);
	}

//
gettimeofday(&endt, NULL);
//
result2.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);

#ifdef VERBOSE
//Print all results
	printf("suffix minimum  is: \n");
	for (i = 0; i<SIZE; i++ )
	{
		printf("%d ", C[0][SIZE-i-1]);
	}
	printf("\n");
#endif

//add the two different timings to get the whole
result1.tv_sec  = result1.tv_sec  + result2.tv_sec;
result1.tv_usec = result1.tv_usec + result2.tv_usec;
//printf("%ld.%07ld \n", result1.tv_usec/1000000, result1.tv_usec%1000000);
 printf("%ld", result1.tv_usec);


	pthread_barrier_destroy(& barrier);
	pthread_exit(NULL);
	return 0 ;
}
