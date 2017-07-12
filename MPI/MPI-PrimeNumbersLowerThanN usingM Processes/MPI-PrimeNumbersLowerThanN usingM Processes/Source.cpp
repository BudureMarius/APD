# include <iostream>
#include "mpi.h"

using namespace std;

void PrintAllPrimeNumbers(int, int);

int main(int argc, char *argv[])
{
	int LIMIT = 100;
	int numberOfProcesses;
	int processRank;
	int error;
	int length;

	error = MPI_Init(&argc, &argv);
	if (error != MPI_SUCCESS) {
		cout << "Error starting MPI program. Terminating.\n";
		MPI_Abort(MPI_COMM_WORLD, error);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

	MPI_Bcast(&LIMIT, 1, MPI_INT, 0, MPI_COMM_WORLD);

	length = LIMIT / numberOfProcesses;

	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
	if (processRank != numberOfProcesses - 1)
		PrintAllPrimeNumbers(length * processRank, length * processRank + length);
	else
		PrintAllPrimeNumbers(length * processRank, LIMIT);

	MPI_Finalize();
}

void PrintAllPrimeNumbers(int start, int stop)
{
	for (int number = (start < 2 ? 2 : start); number < stop; ++number)
	{
		bool isPrime = true;
		for (int div = 2; div <= number / 2; ++div)
			if (number % div == 0)
			{
				isPrime = false;
				break;
			}
		if (isPrime)
		{
			int processRank;
			MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
			cout << "Number : " << number << " -> Process : " << processRank << endl;
		}
	}
}
