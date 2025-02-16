#include <mpi.h>

#include <iostream>

#include <cstring>

#include <chrono>

const int ARRAY_SIZE = 12;

int main(int argc, char* argv[]) {


	int lSum = 0;

	MPI_Init(&argc, &argv);

	int rank, ranksCount;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm_size(MPI_COMM_WORLD, &ranksCount);

	int arraySize = ARRAY_SIZE / ranksCount;

	int* array = new int[ARRAY_SIZE];

	int* array2 = new int[arraySize];

	/*int start = rank * arraySize;
	int end = (rank * arraySize) + arraySize;*/

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		array[i] = i;
	}

	MPI_Scatter(array, arraySize, MPI_INT, array2, arraySize, MPI_INT, 0, MPI_COMM_WORLD);

	int gSum = 0;
	int sum = 0;
	for (int i = 0; i < arraySize; i++) {
		sum += array2[i];
		std::cout << "Rank: " << rank << " " << "Element: " << array2[i] << std::endl;
	}

	MPI_Reduce(&sum, &gSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		std::cout << "Sum: " << gSum;
	}
	/*for (int i = start; i < end; i++) {
		lSum += array[i];
	}*/


	//int sum = 0;

	//MPI_Reduce(array, array2, arraySize, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	//if (rank == 0) {
	//	//int sum = 0;
	//	for (int i = 0; i < arraySize; i++) {
	//		sum += array2[i];
	//	}
	//	std::cout << "Result: " << sum << std::endl;
	//}

	//MPI_Bcast(&sum, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//std::cout << "Sum: " << sum;

	MPI_Finalize();

	return 0;

}