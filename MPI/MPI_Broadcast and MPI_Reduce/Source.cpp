# include <iostream>
#include "mpi.h"

#define LIMIT 24

using namespace std;

int main(int argc, char *argv[])
{
	const int searchedNumber = 4;
	
	int error;
	int numberOfProcesses;
	int length;
	int elements[LIMIT];
	int searchedNumberIndexes[LIMIT];
	int resultIndex;
	int maximumIndex;


	error = MPI_Init(&argc, &argv);
	if (error != MPI_SUCCESS) {
		cout << "Error starting MPI program. Terminating.\n";
		MPI_Abort(MPI_COMM_WORLD, error);
	}

	int processRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);


	if (processRank == 0)
	{
		for (int i = 0; i < LIMIT; ++i)
		{
			elements[i] = rand() % 10;
			searchedNumberIndexes[i] = -1;
			cout << elements[i] << " ";
		}
		cout << endl << endl;
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(elements, LIMIT, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
	length = LIMIT / numberOfProcesses;

	int start = length * processRank;
	int stop = start + length;
	int count = 0;
	
	for( int index = start; index < stop; ++index )
	{
		if (elements[index] == searchedNumber)
			searchedNumberIndexes[count++] = index;
	}

	maximumIndex = searchedNumberIndexes[0];
	for( int index = 0; index <= LIMIT; ++index )
	{
		if (searchedNumberIndexes[index] > maximumIndex)
			maximumIndex = searchedNumberIndexes[index];
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Reduce(&maximumIndex, &resultIndex, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (processRank == 0)
	{
		if (resultIndex < 0)
			cout << "Element not found!" << endl;
		else
			cout << "Maximum position for " << searchedNumber << " is : " << resultIndex << endl;
	}

	MPI_Finalize();
}
