
#include<omp.h>
#include<stdio.h>
#include <stdlib.h>
#include <time.h>



#define n 4
#define p 7
#define m 9

#define THREADS 4


float a[n][p];
float b[p][m];
float c[n][m];



int main() {
    time_t t;
    srand((unsigned) time(&t)); //Intializes random number generator

    unsigned long i,j,k;

    for(i=0; i<n; i++)
        for(k=0; k<p ; k++)
            a[i][k]= rand() %10; //genereaza 100 nr  intre 0 si 10 ( n= 100)


    for(k=0; k<p; k++)
        for(j=0; j<m ; j++)
            a[k][j]= rand() %10;


    for(i=0; i<n; i++)
        for(j=0; j<m ;j++)
            a[i][j]= rand() %10;




#pragma omp parallel shared(a,b,c) private(i,j,k) //a,c,b are shared among the threads & i,j,k are private to each threads
{
    #pragma omp for schedule (static) //static-Divide the loop into equal-sized chunks
     for(i=0; i<n; i++) {
        for(j=0; j<m ; j++) {
            for(k=0; k<p; k++) {
                c[i][j]+= a[i][k] * b[k][j];

            }
        }
     }
}
printf("\n Multipication of Matrix ...\n");

    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            printf("%.2f \t",c[i][j]);
        }
        printf("\n");
    }
    return 0;
}
