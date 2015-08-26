#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define SIZE 			16
#define NUM_THREADS     16

int A[SIZE] = {14, 13, 5, 16,
			   11, 10, 9, 12,
			   0, 8, 7, 15,
			   4, 3, 2, 1};

int distance[SIZE];

int main (int argc, char ** argv)
{
	printf ("Assignment C:  openMP Algorithm! \n" );
	int i;
	int rc;
	long t;

	omp_set_num_threads(8);

	struct timeval startt, endt, result;
	result.tv_sec = 0;
	result.tv_usec= 0;
//
gettimeofday (&startt, NULL);
//
	for (i = 0; i < 5; i++)
	{
        #pragma omp parallel shared(A, distance, i) private(t)
        {
         #pragma omp for schedule(dynamic,4)
         for(t=0; t<NUM_THREADS; t++)
            {
                if (i==0) //first step inits
                {
                    if (A[t] != 0) distance[t] = 1; //init distances
                    else 			 distance[t] = 0; //root node	
                    A[t] = A[t]-1; //indicies correction
                }
                else if (A[t] != -1)
                {
                    distance[t] = distance[t] + distance[A[t]];
                    A[t] = A[A[t]];
                }
            }
        }
	}
//
gettimeofday (&endt, NULL);
//
	//Print all results
	for (i = 0; i<SIZE; i++ )
	{
		printf("%d ", distance[i]);
	}
	printf("\n");

	result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
	printf(" %ld.%06ld | ", result.tv_usec/1000000, result.tv_usec%1000000);

	return 0 ;
}
