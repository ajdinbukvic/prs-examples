#include <mpi.h>

#include <iostream>

#include <cstring>

#include <chrono>

const int ARRAY_SIZE = 100;

int main(int argc, char* argv[]) {

	/*auto start = std::chrono::high_resolution_clock::now();

	int sum = 0;
	int* array = new int[ARRAY_SIZE];

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		array[i] = i;
	}

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		sum += array[i];
	}
	

	std::cout << "Sum: " << sum << std::endl;

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration = end - start;

	std::cout << "Time: " << duration.count();*/

	//////////////////////////////////////////////////////////     Vjezbe 4 (1.11.2024)     //////////////////////////////////////////////

	int lSum = 0;

	MPI_Init(&argc, &argv); 

	int rank, ranksCount;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm_size(MPI_COMM_WORLD, &ranksCount);

	int arraySize = ARRAY_SIZE / ranksCount;

	int* array = new int[ARRAY_SIZE];

	int start = rank * arraySize;
	int end = (rank * arraySize) + arraySize;

	for (int i = start; i < end; i++)
	{
		array[i] = i;
	}

	int gSum = 0;

	for (int i = start; i < end; i++) {
		lSum += array[i];
	}

	/*if (rank == 0) {
		for (int i = 1; i < ranksCount; i++) {
			int processSum = 0;
			MPI_Recv(&processSum, sizeof(int), MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			gSum += processSum;
		}
		std::cout << "Global Sum: "  << gSum;
	}
	else {
		MPI_Send(&gSum, sizeof(int), MPI_INT, 0, 0, MPI_COMM_WORLD);
	}*/


	MPI_Reduce(&lSum, &gSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		std::cout << "Result: " << gSum;
	}



	MPI_Finalize();

	return 0;

}




****************************************************************
vjezbe 4 (1.11.2024.)
****************************************************************

#include <mpi.h>

#include <iostream>

#include <cstring>

#include <chrono>

const int ARRAY_SIZE = 9;

int main(int argc, char* argv[]) {


	int lSum = 0;

	MPI_Init(&argc, &argv); 

	int rank, ranksCount;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm_size(MPI_COMM_WORLD, &ranksCount);

	int arraySize = ARRAY_SIZE / ranksCount;

	int* array = new int[arraySize];

	int* array2 = new int[arraySize];

	int start = rank * arraySize;
	int end = (rank * arraySize) + arraySize;

	for (int i = start; i < end; i++)
	{
		array[i] = i;
	}

	int gSum = 0;

	/*for (int i = start; i < end; i++) {
		lSum += array[i];
	}*/
	


	MPI_Reduce(array, array2, arraySize, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		int sum = 0;
		for (int i = 0; i < arraySize; i++) {
			sum += array2[i];
		}
		std::cout << "Result: " << sum;
	}



	MPI_Finalize();

	return 0;

}