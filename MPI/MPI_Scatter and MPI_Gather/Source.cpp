# include <iostream>
#include <algorithm>
#include "mpi.h"

#define LIMIT 24

using namespace std;

int main(int argc, char *argv[])
{
	const int searchedNumber = 4;

	int error;
	int numberOfProcesses;

	int elements[LIMIT];
	int resultIndexes[LIMIT];

	error = MPI_Init(&argc, &argv);
	if (error != MPI_SUCCESS)
	{
		cout << "Error starting MPI program. Terminating.\n";
		MPI_Abort(MPI_COMM_WORLD, error);
	}

	int processRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

	if (processRank == 0)
	{
		for (int index = 0; index < LIMIT; ++index)
		{
			elements[index] = rand() % 10;
			cout << elements[index] << " ";
		}
		cout << endl << endl;
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

	int searchedNumberIndexes[LIMIT];
	int receiveElements[LIMIT];

	for( int index = 0; index < LIMIT; ++index )
	{
		searchedNumberIndexes[index] = -1;
		receiveElements[index] = -1;
		resultIndexes[index] = -1;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Scatter(elements, LIMIT / numberOfProcesses, MPI_INT, receiveElements, LIMIT / numberOfProcesses, MPI_INT, 0, MPI_COMM_WORLD);

	int count = 0;
	for( int index = 0; index < LIMIT / numberOfProcesses; ++index)
		if (receiveElements[index] == searchedNumber)
			searchedNumberIndexes[count++] = index + (LIMIT / numberOfProcesses) * processRank;

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(searchedNumberIndexes, count, MPI_INT, resultIndexes, count, MPI_INT, 0, MPI_COMM_WORLD);

	if (processRank == 0)
	{
		for (int index = 0; index < LIMIT; ++index)
			if (resultIndexes[index] >= 0)
				cout << resultIndexes[index] << " ";
	}
		
	MPI_Finalize();

	return 0;
}
