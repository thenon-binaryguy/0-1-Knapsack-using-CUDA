#include<stdio.h>
#include<omp.h>
int max(int x,int y)
{
    return (x > y ? x:y);
}
int main(void)
{
    int w,n;
    printf("Serial Execution - \n-------------------------------------------------------");
    printf("\nEnter no. of questions, n = ");
    scanf("%d",&n);
    int wt[n];
    int val[n];
    printf("\nEnter maximum marks for all questions (weight array) : \n");
    for(int i=0;i<n;i++)
    {
        printf("Maximum marks for question #%d = ",i+1);
        scanf("%d",&wt[i]);
    }
    printf("\nEnter marks scored by student (value array) : \n");
    for(int i=0;i<n;i++)
    {
        printf("Marks scored by student in question #%d = ",i+1);
        scanf("%d",&val[i]);
    }
    printf("\nEnter total(maximum) marks of the paper, w = ");
    scanf("%d",&w);
    int mat[n+1][w+1];
    double beg = omp_get_wtime();
    //1st row all zero
    for(int i=0;i<w+1;i++)
    {
        mat[0][i]=0;
    }
    //1st column all zero
    for(int i=0;i<n+1;i++)
    {
        mat[i][0]=0;
    }
    // filling up the table
    int a,b;
    for(int i=1;i<n+1;i++)
    {
        for(int j=1;j<w+1;j++)
        {
            if(wt[i-1]>j)
            {
                mat[i][j] = mat[i-1][j];
            }
            else
            {
                a = mat[i-1][j];
                b = val[i-1] + mat[i-1][j - wt[i-1]];
                mat[i][j] = max(a,b);
            }
        }
    }
    printf("-------------------------------------------------------\nResultant table - \n");
    for(int i=0;i<n+1;i++)
    {
        for(int j=0;j<w+1;j++)
        {
            printf("%d ",mat[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------------------------------------\nMaximum total marks possible to score = %d\n-------------------------------------------------------",mat[n][w]);
    double end = omp_get_wtime();
    printf("\n[Time taken for serial execution = %g]\n",(end-beg));
}
