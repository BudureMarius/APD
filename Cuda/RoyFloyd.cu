#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>

#define INF 1000000

using namespace std;

__global__ void RoyFloyd(int* matrix, int k, int N)
{
	int i = blockDim.y * blockIdx.y + threadIdx.y;
	int j = blockDim.x * blockIdx.x + threadIdx.x;

	if (matrix[i*N + k] + matrix[k*N + j] < matrix[i*N + j])
		matrix[i*N + j] = matrix[i*N + k] + matrix[k*N + j];
}

int main()
{
	int n;
	int thread_per_block = 1024;

	ifstream input("input.txt");
	ofstream output("output.txt");

	input >> n;

	int num_block = ceil((float)(n*n / (thread_per_block)));

	int *hostMatrix = (int*)malloc(sizeof(int) * n * n);
	int *deviceMatrix;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			input >> hostMatrix[i + j];
			if (hostMatrix[i + j] == 0)
				hostMatrix[i + j] = INF;
		}

	cudaMalloc((void**)&deviceMatrix, n * n * sizeof(int));
	cudaMemcpy(deviceMatrix, hostMatrix, n * n * sizeof(int), cudaMemcpyHostToDevice);
	
	clock_t t;
	t = clock();

	for (int k = 0; k < n; ++k)
		RoyFloyd <<< num_block, thread_per_block >>>(deviceMatrix, k, n);

	t = clock() - t;
	cout << "Time : " << ((double)t) / CLOCKS_PER_SEC << endl;
	
	cudaMemcpy(hostMatrix, deviceMatrix, n * n * sizeof(int), cudaMemcpyDeviceToHost);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			if (i == j || hostMatrix[i + j] == INF)
				output << "0 ";
			else
				output << hostMatrix[i + j] << " ";
		output << endl;
	}

	free(hostMatrix);
	cudaFree(deviceMatrix);
}