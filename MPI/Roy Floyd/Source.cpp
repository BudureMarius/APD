#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

const int INFINITY = 10000000;

int*aux;

void Read_matrix(int receiveMatrix[], int size, int rank, int processes, MPI_Comm comm)
{
	int matrix[6][6] = {
		{ 0, 3, 5, 1, INFINITY, INFINITY },
		{ 3, 0, INFINITY, INFINITY, 9, INFINITY },
		{ 5, INFINITY, 0, 7, 7, 1 },
		{ 1, INFINITY, 7, 0, INFINITY, 4 },
		{ INFINITY, 9, 7, INFINITY, 0, INFINITY },
		{ INFINITY, INFINITY, 1, 4, INFINITY, 0 }
	};

	int i, j;
	int* sendMatrix = NULL;


	if (rank == 0)
	{
		sendMatrix = (int *)malloc(size * size * sizeof(int));
		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++)
				sendMatrix[i * size + j] = matrix[i][j];
	}

	//
	MPI_Scatter(sendMatrix, size * size / processes, MPI_INT, receiveMatrix, size * size / processes, MPI_INT, 0, comm);
}

void Print_matrix(int sendMatrix[], int n, int rank, int processes, MPI_Comm comm)
{
	int i, j;
	int* receiveMatrix = NULL;
	

	if (rank == 0)
	{
		receiveMatrix = (int *)malloc(n*n * sizeof(int));
		MPI_Gather(sendMatrix, n*n / processes, MPI_INT, receiveMatrix, n*n / processes, MPI_INT, 0, comm);
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
				printf("%d ", receiveMatrix[i*n + j]);
			printf("\n");
		}
		free(receiveMatrix);
	}
	else
	{
		MPI_Gather(sendMatrix, n*n / processes, MPI_INT, receiveMatrix, n*n / processes, MPI_INT, 0, comm);
	}
}

void MakeCopy(int matrix[], int n, int processes, int row[], int k)
{
	for (int j = 0; j < n; j++)
		row[j] = matrix[(k % (n / processes))*n + j];
}

void Floyd(int matrix[], int size, int rank, int processes)
{
	int krank;

	aux = (int *)malloc(size * sizeof(int));

	for (int k = 0; k < size; k++)
	{
		krank = (size * k + 1) / (( size * size) / processes);

		if (rank == krank)
			MakeCopy(matrix, size, processes, aux, k);
		
		MPI_Bcast(aux, size, MPI_INT, krank, MPI_COMM_WORLD);

		for (int i = 0; i < size / processes; i++)
			for (int j = 0; j < size; j++)
				if (matrix[i*size + k] + aux[j] < matrix[i*size + j])
					matrix[i*size + j] = matrix[i*size + k] + aux[j];
	}
	free(aux);
}

int main(int argc, char* argv[])
{
	int* matrix;
	int size;
	int processes;
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	size = 6;

	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	matrix = (int *)malloc(size * size / processes * sizeof(int));

	Read_matrix(matrix, size, rank, processes, MPI_COMM_WORLD);
	

	clock_t start, stop;
	start = clock();

	Floyd(matrix, size, rank, processes);


	Print_matrix(matrix, size, rank, processes, MPI_COMM_WORLD);

	free(matrix);


	stop = clock();
	printf("\nAll cost time is: %d", (stop - start) * 1000 / CLOCKS_PER_SEC);
	printf("ms\n");

	MPI_Finalize();

	return 0;
}