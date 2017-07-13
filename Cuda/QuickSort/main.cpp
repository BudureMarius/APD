#include <io.h>
#include <malloc.h>
#include <cuda.h>
#include <stdlib.h>
#include <stdio.h>
#include <cuda_runtime.h>

#define n 10

__global__ void quickSort(int arr[], int start, int end);

int main(){

	int index;
	int size;
	size = n * sizeof(int);

	int* h_arr = (int*)malloc(size);

	int* d_arr;
	cudaMalloc(&d_arr, size);

	for(index=0;index<n;index++){
		printf("enter the %d number from arr",index);
		scanf("%d",&h_arr[index]);
	}
	
	cudaMemcpy(d_arr, h_arr, size, cudaMemcpyHostToDevice);

	quickSort <<< 1, n >>>(d_arr, 0, n-1);

	cudaMemcpy(h_arr, d_arr, size, cudaMemcpyDeviceToHost);

	cudaFree(d_arr);

}


 __global__ void quickSort(int arr[], int start, int end) {

	if (start >= end || start < 0 || end >= n) {
				return;
			}
	if (end - start < n) {
		for (int i = start; i <= end; i++) {
			int tmp = arr[i];
			int j = i;
			for (; j > 0 && tmp < arr[j - 1]; j--)
				arr[j]= arr[j - 1];
				arr[j] = tmp;

			}
			return;
		}
		int pivotIdx = (end + start) / 2;
		int pivot = arr[pivotIdx];
		int i = start;
		int j = end;

		while (i < j) {
			while (arr[i] < pivot)
				i++;
				while (arr[j] > pivot)
					j--;
				if (i >= j)
					break;
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;

			}
		quickSort<<< 1, n >>>(arr, start, i - 1);

		quickSort<<< 1, n >>>(arr, i + 1, end);
	}
}
