#include<stdio.h>
#include<omp.h>
#include <time.h>
#include <malloc.h>
#include <cuda.h>
#include <cuda_runtime.h>



__global__ void Knapsack(int W, int wt[], int val[], int n,int *res)
{
    
	int i, w;
    int** K = new int*[n+1];
    //int K[n + 1][W + 1];
    for(int i = 0; i < (n+1); i++)
    {K[i] = new int[W+1];}


    for (i = 0; i <= n; i++)
    {
        for (w = 0; w <= W; w++)
        {
            if (i == 0 || w == 0)
                K[i][w] = 0;
            else if (wt[i - 1] <= w)
                K[i][w] = max(val[i - 1]
                          + K[i - 1][w - wt[i - 1]],
                          K[i - 1][w]);
            else
                K[i][w] = K[i - 1][w];
        }
    }
 	//printf("Total marks of the student= %d\n", K[n][W]);
 	int q;
 	q=K[n][W];
 	*res=q;
 	delete[] K ;
}	
int main(void)
{
    int w,n;
	int i;
    int* weights;
	int* values;
	int* d_values;
	int* d_weights;
	int *res,*r;
	clock_t start, end;
    printf("Parallel Execution Using Cuda \n-------------------------------------------------------");
    
    printf("\nEnter no. of questions, n = ");
    scanf("%d",&n);
    
    //Allocating host memory
    weights = (int*)malloc((n+1)*sizeof(int));
	values = (int*)malloc((n+1)*sizeof(int));
	r=(int*)malloc(sizeof(int));
	
	//Allocating device memory
	cudaMalloc((void**)&d_weights, sizeof(int)*(n+1));
	cudaMalloc((void**)&d_values, sizeof(int)*(n+1));
	cudaMalloc((void**)&res, sizeof(int));
	
    printf("\nEnter maximum marks for all questions (weight array) : \n");
    for(i=0;i<n;i++)
    {
        printf("Maximum marks for question #%d = ",i);
        scanf("%d",&weights[i]);
    }
    printf("\nEnter marks scored by student (value array) : \n");
    for(i=0;i<n;i++)
    {
        printf("Marks scored by student in question #%d = ",i);
        scanf("%d",&values[i]);
    }
    printf("\nEnter total(maximum) marks of the paper(Weight w), w = ");
    scanf("%d",&w);
    //double beg = omp_get_wtime();
    
    cudaMemcpy(d_weights, weights, sizeof(int)*(n+1), cudaMemcpyHostToDevice);
    cudaMemcpy(d_values, values , sizeof(int)*(n+1), cudaMemcpyHostToDevice);
    
    start = clock();
    //Executing the Kernel
	Knapsack<<<1,256>>>(w,d_weights,d_values,n,res); //Optimal valuesue for items with given weights and values
	
    cudaMemcpy(r,res,sizeof(int),cudaMemcpyDeviceToHost);
	printf("Total marks of the student= %d\n", *r);
	end = clock();
    double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
	printf("Time taken = %1f s",(double)(time_taken));
	
	cudaMemcpy(weights,d_weights, sizeof(int)*(n+1), cudaMemcpyDeviceToHost);
    cudaMemcpy( values ,d_values, sizeof(int)*(n+1),cudaMemcpyDeviceToHost);
    
	//De allocating device memory
	cudaFree(d_weights);
	cudaFree(d_values);

	
	//De allocating host memory
	free(weights);
	free(values);
	
	return 0;
}
