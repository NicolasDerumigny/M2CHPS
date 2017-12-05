#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda.h>

#define SIZE 102400
#define MOD 102399
#define STEP 128

/* ARRAY A INITIALIZER */
void init_a(int * a)
{
    int i;
    for(i=0; i<SIZE; i++)
    {
        a[i] = 1;
    }
}

/* ARRAY B INITIALIZER */
void init_b(int * b)
{
	int i, j;

	j=0;

	for(i=0; i<SIZE-1; i++)
	{
		b[j] = i;
		j = (j+STEP)%MOD;
	}	

    b[SIZE-1] = SIZE-1;
}

/* CHECKING A VALUES */
int check_a(int * a)
{
    int i;
    int correct = 1;
	for(i=0; i<SIZE; i++)
	{
		if(a[i] != (i+1)) 
		{
         
			correct = 0;
		} 
	}	

    return correct;
}


/* CUDA FUNCTION */
__global__ void mykernel(int * a, int * b, int N)
{
/* A COMPLETER */
}


int main(int argc, char * argv[])
{

	int * a = (int *)malloc(sizeof(int)*SIZE);
	int * b = (int *)malloc(sizeof(int)*SIZE);

    init_a(a);
	init_b(b);


/*  INSERT CUDA ALLOCATION AND COPY HERE */
    /* A COMPLETER */    


	dim3 nBlocks;
	dim3 nThperBlock;

	nBlocks.x = 1;
	
	nThperBlock.x = 1024;

	mykernel<<< nBlocks , nThperBlock >>>(d_a, d_b, SIZE);

	
/* INSERT CUDA COPY HERE */
    /* A COMPLETER */

	int correct = check_a(a);;
	
	if(0 == correct)
	{
		printf("\n\n ******************** \n ***/!\\ ERROR /!\\ *** \n ******************** \n\n");
	}
	else
	{
		printf("\n\n ******************** \n ***** SUCCESS! ***** \n ******************** \n\n");
	}


	return 1;
}
