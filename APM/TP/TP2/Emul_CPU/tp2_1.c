#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define SIZE 102400
#define MOD 102399
#define STEP 32

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
	int value;

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


int main(int argc, char * argv[])
{

	int * a = malloc(sizeof(int)*SIZE);
	int * b = malloc(sizeof(int)*SIZE);

    init_a(a);
	init_b(b);
	

	int i;
	for(i=0; i<SIZE; i++)
	{
		a[b[i]] += b[i];
	}




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
