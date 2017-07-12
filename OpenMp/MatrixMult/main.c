#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define Arow 8
#define Acol 6
#define Brow 6
#define Bcol 10
#define PTHREADS 4

/* Definire matrice*/
typedef struct Matrix {
    int r, c;
    int *contents;
    int **rows;
}Matrix;
    int rows_per_proc;
    Matrix A, B, C;
    pthread_t tid[PTHREADS];

/* Definire functii */
    Matrix createMatrix(int rows, int cols);
    void *MatrixMult(void* tid);
    void printMatrices(Matrix A, Matrix B, Matrix C);

void main() {
    int rw, cl, i, err;

/* Creare si initializare trei matrici. Primele doua vor fi inmultite, iar a treia va retine rezultatul */
    A = createMatrix(Arow, Acol);
    B = createMatrix(Brow, Bcol);
    C = createMatrix(Arow, Bcol);

/* Se completeaza matricile A si B cu nr intregi random intre 0 si 100*/
    srand(time(NULL));

    for (rw = 0; rw < Arow; rw++){
        for (cl = 0; cl < Acol; cl++){
            A.rows[rw][cl] = rand() % 26;
        }
    }

    for (rw = 0; rw < Brow; rw++){
        for (cl = 0; cl < Bcol; cl++){
            B.rows[rw][cl] = rand() % 26;
        }
    }

/* Liniile din C vor fi calculate de fiecare thread. */
    rows_per_proc = Arow / PTHREADS;

/* Initializare si incepere(start) pthreads */
    for(i = 0; i < PTHREADS; i++){
        err = pthread_create(&tid[i], NULL, MatrixMult, (void *) i);
        if(err == 0) printf("Thread %d started successfully!\n", i);
        else printf("Thread %d did not start successfully.\n", i);
    }

    for(i = 0; i < PTHREADS; i++){
        err = pthread_join(tid[i], NULL); //asteptand sa se alature threadul i fara statut//Funcția pthread_join () suspendă executarea threadului de asteptare pana cand target thread se termina, cu excepția cazului în target thread a terminat deja
        //Daca va avea succes, funcția pthread_join () va returna 0
        if(err == 0) printf("Thread %d joined successfully!\n", i);
        else printf("Thread %d did not join successfully.\n", i);
    }

    printMatrices(A, B, C);
    printf("Press Enter to Exit: ");
    scanf("Exit");
}

Matrix createMatrix(int rows, int cols){
    int i;
    Matrix mtx;
    mtx.r = rows;
    mtx.c = cols;

/* Alocare memorie pentru mtx */
    mtx.contents = (int *) malloc(sizeof(int) * rows * cols);
    mtx.rows = (int **) malloc(sizeof(int *) * rows);

/*  Gasire adresa de memorie de pornire  pentru fiecare linie a matricei */
    for (i = 0; i < rows; i++){
        mtx.rows[i] = mtx.contents + i*cols;
    }

    return mtx;
}

void *MatrixMult(void* tid){
    int i, j, k, total;
    int id = (int)tid;
    int row_start = id*rows_per_proc;

    if(id < PTHREADS - 1){
        for (i = row_start; i < row_start + rows_per_proc; i++){
            printf("Thread %d cu linia %d\n",id,i);
            for (j = 0; j < B.c; j++){
                total = 0;
                for (k = 0; k < A.c; k++){
                    total += A.rows[i][k] * B.rows[k][j];
                }
                C.rows[i][j] = total;
            }
        }
        }else{ //ultimul thread primeste toate liniile ramase
            for (i = row_start; i < row_start + (A.r - ((PTHREADS-1)*rows_per_proc)); i++){
                printf("Thread %d cu linia %d\n",id,i);
                for (j = 0; j < B.c; j++){
                    total = 0;
                    for (k = 0; k < A.c; k++){
                        total += A.rows[i][k] * B.rows[k][j];
                    }
                    C.rows[i][j] = total;
                }
            }
        }
}

void printMatrices(Matrix A, Matrix B, Matrix C){
    int i, j;

/* Print Matrix A */
    printf("Matrix A:\n");
    for (i = 0; i < A.r; i++){
        for (j = 0; j < A.c; j++){
            printf("%6d ", A.rows[i][j]);
        }
        printf("\n\n");
    }

/* Print Matrix B */
    printf("Matrix B:\n");
    for (i = 0; i < B.r; i++){
        for (j = 0; j < B.c; j++){
            printf("%6d ", B.rows[i][j]);
        }
        printf("\n\n");
    }

/* Print Matrix C = A*B */
    printf("Matrix C (A * B):\n");
    for (i = 0; i < C.r; i++){
        for (j = 0; j < C.c; j++){
            printf("%6d ", C.rows[i][j]);
        }
        printf("\n\n");
        }
    }





