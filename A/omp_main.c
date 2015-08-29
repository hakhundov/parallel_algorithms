#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

#define SIZE 32
#define NUM_THREADS 1
#define NT 32 
//#define VERBOSE

#define min(a, b) (((a) < (b)) ? (a) : (b))


int A[SIZE] = {58,  89,   32, 73,  131, 156, 30,  29, 
         141, 37,  133, 151, 88,  53,  122, 126,
         131, 49,  130, 115, 16,  83,  40,  145,
         10,  112, 20,  147, 14,  104, 111, 92};

int B[6][32]; // LOG2(SIZE) + 1 
int C[6][32]; // LOG2(SIZE) + 1 
int S[6][32]; // LOG2(SIZE) + 1 


int h; //global for STEP2&3
void execute(int num_threads, void * (*func)(void *)) {
	int rc;
	long t;
	
	#pragma omp parallel shared(A,B,C) private(t)
	{
		#pragma omp parallel for
		for(t=0; t<num_threads; t++)
		{
			func((void *)t);
		}
	}
}

//initialize B[0,for all] = A[for all]
void *init(void *threadid)
{
	long tid;
	tid = (long)threadid;
    B[0][tid] = A[tid];
}

void *R_init(void *threadid)
{
	long tid;
	tid = (long)threadid;
    B[0][SIZE-tid-1] = A[tid];
}

//balanced tree method
void *minimum(void *threadid)
{
	long tid;
	tid = (long)threadid;
    B[h][tid] = min (  B[h-1][2*tid], B[h-1][2*tid+1]  );
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
}


int main (int argc, char ** argv)
{
	int i;
	int n_threads;
#ifdef VERBOSE
	printf ("Assignment A:  openMP Algorithm! \n" );
#endif
struct timeval startt, endt, result1, result2;
result1.tv_sec = 0;
result1.tv_usec= 0;
result2.tv_sec = 0;
result2.tv_usec= 0;

omp_set_num_threads(NUM_THREADS);

//
gettimeofday(&startt, NULL);
//

//STEP 1
	execute(NT, init);
	
//STEP2
	for (h=1; h<=log2(SIZE); h++) 
	{
		n_threads = SIZE/(int)pow(2,h);
		execute(n_threads,minimum);
	}

//STEP3
	for (h=log2(SIZE); h>=0; h--)
	{
		n_threads = SIZE/(int)pow(2,h);
		execute(n_threads,combine);
	}


//
gettimeofday(&endt, NULL);
//
result1.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);

//Print all results
#ifdef VERBOSE
	printf("prefix minimum is: \n");
	for (i = 0; i<SIZE; i++ )
	{
		printf("%d ", C[0][i]);
	}
	printf("\n");
#endif


//
gettimeofday(&startt, NULL);
//
//DO THE PREFIX NOW, by rotating

//STEP 1
	execute(NT, R_init); //<--- REVERSE INITIALIZATION
	
//STEP2
	for (h=1; h<=log2(SIZE); h++)
	{
		n_threads = SIZE/(int)pow(2,h);
		execute(n_threads,minimum);
	}

//STEP3
	for (h=log2(SIZE); h>=0; h--)
	{
		n_threads = SIZE/(int)pow(2,h);
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
//printf("%ld.%06ld \n", result1.tv_usec/1000000, result1.tv_usec%1000000);
printf("%ld", result1.tv_usec);
	return 0 ;
}
