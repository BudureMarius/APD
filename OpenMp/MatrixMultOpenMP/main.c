#include <omp.h>
#include<stdio.h>

/*#define n s
#define p s
#define m s


float a[n][p];
float b[p][m];
float c[n][m];



int main() {

    unsigned long i,j,k;

    for(i=0; i<n; i++)
        for(k=0; k<p ; k++)
            a[i][k]= rand();


    for(k=0; k<p; k++)
        for(j=0; j<m ; j++)
            a[k][j]= rand();


    for(i=0; i<n; i++)
        for(j=0; j<m ; j++)
            a[i][j]= rand();




#pragma omp parallel shared(a,b,c) private(i,j,k) //a,c,b are shared among the threads & i,j,k are private to each threads
{
    #pragma omp schedule (static)
     for(i=0; i<n; i++) {
        for(j=0; j<m ; j++) {
            for(k=0; k<p; k++) {
                c[i][j]+= a[i][k] * b[k][j];

            }
        }
     }

    return 0;
}
*/



float a[1000][1000];
float b[1000][1000];
float c[1000][1000];

int main()
{
    int i,j,k;
    // Initialize buffers.
    for ( i = 0; i < 1000; ++i) {
        for ( j = 0; j < 1000; ++j) {
            a[i][j] = (float)i + j;
            b[i][j] = (float)i - j;
            c[i][j] = 0.0f;
        }
    }

    // Compute matrix multiplication.
    // C <- C + A x B
    #pragma omp parallel for default(none) shared(a,b,c)
    for ( i = 0; i < 1000; ++i) {
        for (j = 0; j < 1000; ++j) {
            for (k = 0; k < 1000; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return 0;
}
