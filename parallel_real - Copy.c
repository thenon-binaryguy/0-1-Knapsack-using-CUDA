#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <malloc.h>
#include <time.h>
typedef struct {
    int x;
} __attribute__((aligned(64))) elem;
int MFKnapsack(int, int); //Function returning the optimal value for a given n and W
int max(int, int);
int n;
int* weights;
int* values;
int W;
elem **F;
int main()
{
    clock_t start, end;

    printf("enter number of Questions in the Exam paper :");
	scanf("%d", &n);
	printf("---------------------------------------------------");
	weights = (int*)malloc((n+1)*sizeof(int));
	values = (int*)malloc((n+1)*sizeof(int));
	int i, j;
	//Read weights and corresponding values for 'n' items
	printf("\nenter marks (maximum) for all %d questions\n \n",n);
	for(i=1;i<=n;i++)
	{
	    printf("enter marks (maximum) for question number %d :",i);
		scanf("%d", &weights[i]);
	}
	printf("---------------------------------------------------");
	printf("\nenter marks scored by the student for all %d questions \n",n);
	for(i=1;i<=n;i++)
	{
	    printf("enter marks scored by student in question number %d :",i);
		scanf("%d", &values[i]);
	}
    printf("---------------------------------------------------");
	//Read Max. Weight Capacity of Knapsack
	printf("\nenter the total(maximum) marks of th paper : ");
	scanf("%d", &W);
	//Allocating memory for the Memory Function
	F = (elem**)malloc((n+1)*sizeof(elem*));
	//64 => boundary width to the next element
	for(i=0;i<=n;i++)
		F[i] = (elem*) _aligned_malloc(64,(W+1)*sizeof(elem));
	//Zeroing out the first row
	for(i=0;i<(W+1);i++)
		(*(F) + i ) -> x = 0;
	//Zeroing out the first column
	for(i=0;i<(n+1);i++)
		(*(F+i)) -> x = 0;
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=W;j++)
			(*(F + i) + j) -> x = -1;
	}
    start = clock();
	int res;
	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			printf("\n1\n");
			res = MFKnapsack(n, W); //Optimal value for items with given weights and values
		}
	}
    end = clock();
    double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
    printf("\n2\n");
	printf("Total marks of the student= %d\n", res);
	printf("Time taken = %1f s",(double)(time_taken));
	free(weights);
	free(values);
	return 0;
}
int MFKnapsack(int i, int j)
{
	int value;
	if( ((*(F + i) + j)->x) < 0)
	{
		if(j < weights[i])  //If weight of item is more than current capacity
			value = MFKnapsack(i-1, j); //Value of previous item
		else
		{
			int a;
			int b;
				#pragma omp parallel sections
				{
	//Two sections that can run independently and simultaneously
					#pragma omp  section
					{
						a = MFKnapsack(i-1, j);
					}
					#pragma omp  section
					{
						b =  (values[i] + MFKnapsack(i-1, j - weights[i]));
					}
				}
				value = max(a,b);
				printf("\n3\n");
		}
		(*(F + i) + j)->x = value;
	}
	return (*(F + i) + j)->x;
}
int max(int a, int b)
{
	return (a>b?a:b);
}

