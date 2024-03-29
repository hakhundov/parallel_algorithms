#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 32

//#define VERBOSE

int main (int argc, char ** argv)
{

#ifdef VERBOSE
   printf ("Assignment A:  Sequential Algorithm! \n" );
#endif

int A[SIZE] = {58,  89,   32, 73,  131, 156, 30,  29, 
         141, 37,  133, 151, 88,  53,  122, 126,
         131, 49,  130, 115, 16,  83,  40,  145,
         10,  112, 20,  147, 14,  104, 111, 92};


/*
#undef SIZE
#define SIZE 8192
*/

int i;
int prefix_minima[SIZE];
int suffix_minima[SIZE];

/*
int A[8192];


srand(42);

for (i = 0 ; i< 8192; i++)
{	
	A[i] = rand()%1000+20;
}
*/


struct timeval startt, endt, result;
result.tv_sec = 0;
result.tv_usec= 0;

prefix_minima[0] = A[0];

//
gettimeofday(&startt, NULL);
//
for (i = 1; i<SIZE; i++ )
	{
	 if (A[i] <= prefix_minima[i-1])
		{
			prefix_minima[i] = A[i];
		}
	 else
		{
			prefix_minima[i] = prefix_minima[i-1];
		}
	}


suffix_minima[SIZE-1] = A[SIZE-1];
for (i = 1; i<SIZE; i++ )
{
  if (A[SIZE-i-1] <= suffix_minima[SIZE-i])
	{
		suffix_minima[SIZE-i-1] = A[SIZE-i-1];
	}
 else
    {
		suffix_minima[SIZE-i-1] = suffix_minima[SIZE-i];
	}
}
//
gettimeofday(&endt, NULL);
//

#ifdef VERBOSE
for (i = 0; i<SIZE; i++ )
{
	printf("%d " ,prefix_minima[i]);
}
printf("\n");
 
for (i = 0; i<SIZE; i++ )
{
	printf("%d " ,suffix_minima[i]);
}
printf("\n");
#endif

result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
//printf("%ld.%07ld", result.tv_usec/1000000, result.tv_usec%1000000);

printf("%ld", result.tv_usec);



   return 0 ;
}
