#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE_A 8
#define SIZE_B 32

int main (int argc, char ** argv)
{
   printf ("Assignment B:  openMP Algorithm! \n" );

	struct timeval startt, endt, result;
	result.tv_sec = 0;
	result.tv_usec= 0;

int A[SIZE_A] = {28, 36, 40, 61, 68, 71, 123, 149};

int B[SIZE_B] = {2,  5,   18, 21,  24, 29,  31,  33, 
         		34,  35,  47, 48,  49, 50,  52,  62,
        		66,  70,  73, 80,  88, 89,  114, 124,
         		125, 131, 143,144, 145,148, 155, 159};

int C[SIZE_A + SIZE_B];
int i,k;
int rank[SIZE_A+SIZE_B];
int sorted[SIZE_A+SIZE_B];


//
gettimeofday (&startt, NULL);
//

for (i = 0; i<SIZE_A; i++)  C[i] = A[i];
for (i = 0; i<SIZE_B; i++)  C[i+SIZE_A] = B[i]; 
for (i = 0; i<SIZE_A+SIZE_B; i++) rank[i]=0;

#pragma omp parallel shared(C) private(i,k)
{
	#pragma omp for
	for (i = 0; i<SIZE_A+SIZE_B; i++)
	{
		for (k = 0; k<SIZE_A+SIZE_B; k++ )
			{
	  			if (C[k]<=C[i]) rank[i]++;
			}	
	}
}

#pragma omp for
for (i = 0; i<SIZE_A+SIZE_B; i++ )
{
  sorted[rank[i]-1] = C[i];
}

//
gettimeofday (&endt, NULL);
//

for (i = 0; i<SIZE_A+SIZE_B; i++ )
{
	printf("%d ", sorted[i]);
}
printf("\n");
 
	result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
	printf(" %ld.%06ld | ", result.tv_usec/1000000, result.tv_usec%1000000);

   return 0 ;
}
