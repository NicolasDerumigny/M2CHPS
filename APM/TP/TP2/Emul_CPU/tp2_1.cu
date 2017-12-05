#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda.h>


#define SIZE 10240
#define NBBLOCKS 1


/* B INITIALIZER */
void init_b(int * b)
{
	int i, j;
	int * check = malloc(sizeof(int)*SIZE);
	memset(b, -1, SIZE*sizeof(int));
	memset(check, 0, SIZE*sizeof(int));

	for(i=0; i<SIZE*2; i++)
	{
		int value = (i*i)%SIZE;
		if(0 == check[value])
		{	
			b[i] = value;
			check[value]=1;
		}
	}	
	for(i=0; i<SIZE; i++)
	{
		if(0 == check[i])
		{
		for(j=0; j<SIZE && (0 == check[i]); j++)
			{
				if(-1 == b[j])
				{ 
					b[j] = i;
					check[i] = 1;
				}
			}
		}
	}
	free(check);
}




/* CUDA FUNCTION */
__global__ mykernel(int * a, int * b, int N)
{
//	int i;
//	for(i=0; i<SIZE; i++)
//	{
//		a[b[i]] = i;
//	}

}


int main(int argc, char * argv[])
{

	int * a = malloc(sizeof(int)*SIZE);
	int * b = malloc(sizeof(int)*SIZE);

	init_b(b);



/*  INSERT CUDA ALLOCATION AND COPY HERE */
	

/* FILL IN MYKERNEL ARGS*/

	dim3 nBlocks;
	dim3 nThperBlock;

	nBlock.x = 1;
	
	nThperBlock = SIZE / 1;

	mykernel<<< nBlock , nThperBlock >>>(d_a, d_b, SIZE);

	
/* INSERT CUDA COPY HERE */





	int correct = 1;
	
	for(i=0; i<SIZE; i++)
	{
		if(a[i] != i) 
		{
			correct = 0;
		} 
	}	


	if(0 == correct)
	{
		printf("ERROR!!!!! \n");
	}
	else
	{
		printf("SUCCESS! \n");
	}


	return 1;
}
