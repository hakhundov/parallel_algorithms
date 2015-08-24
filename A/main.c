#include <stdio.h>
#include <stdlib.h>

#define SIZE 32

int main (int argc, char ** argv)
{
   printf ("Assignment A:  Sequential Algorithm! \n" );

int A[SIZE] = {58,  89,   32, 73,  131, 156, 30,  29, 
         141, 37,  133, 151, 88,  53,  122, 126,
         131, 49,  130, 115, 16,  83,  40,  145,
         10,  112, 20,  147, 14,  104, 111, 92};
int i;
int prefix_minima[SIZE];
int suffix_minima[SIZE];


prefix_minima[0] = A[0];
 
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
for (i = 1; i<SIZE+1; i++ )
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
 


   return 0 ;
}
