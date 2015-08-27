#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 16
//#define VERBOSE

int main (int argc, char ** argv)
{

#ifdef VERBOSE
	printf ("Assignment C:  Sequential Algorithm! \n" );
#endif
	int A[SIZE] = {	14, 13, 5, 16,
			11, 10, 9, 12,
			0, 8, 7, 15,
			4, 3, 2, 1};

	int distance[SIZE];
	int i,k;

	struct timeval startt, endt, result;
	result.tv_sec = 0;
	result.tv_usec= 0;

//
gettimeofday (&startt, NULL);
//

	for (i = 0; i<SIZE; i++) distance[i]=0;

	for (i = 0; i<SIZE; i++)
	{
		k = i;

		while (1)
		{ 
			if (A[k] == 0) break;
			else k = A[k]-1;
			distance[i]++;
		} 
	}

//
gettimeofday (&endt, NULL);
//
#ifdef VERBOSE
	for (i = 0; i<SIZE; i++ )
	{
		printf("%d ", distance[i]);
	}
	printf("\n");
#endif

	result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
	printf("%ld.%06ld \n", result.tv_usec/1000000, result.tv_usec%1000000);

   return 0 ;
}
