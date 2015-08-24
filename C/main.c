#include <stdio.h>
#include <stdlib.h>

#define SIZE 16

int main (int argc, char ** argv)
{
   printf ("Assignment C:  Sequential Algorithm! \n" );



int A[SIZE] = {14, 13, 5, 16,
			   11, 10, 9, 12,
			   0, 8, 7, 15,
			   4, 3, 2, 1};

int distance[SIZE];
int i,k;


for (i = 0; i<SIZE; i++)
	distance[i]=0;

for (i = 0; i<SIZE; i++)
{
//printf ("\n === i is: %d \n", i );
   
k = i;

	while (1)
     { 
        //printf ("A[k-1]: %d", A[k-1]);
		if (A[k] == 0) break;
        else k = A[k]-1;
		distance[i]++;
     } 
}



 


for (i = 0; i<SIZE; i++ )
{
	printf("%d ", distance[i]);
}
printf("\n");
 


   return 0 ;
}
