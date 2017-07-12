#include <omp.h>
#include<stdio.h>

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
