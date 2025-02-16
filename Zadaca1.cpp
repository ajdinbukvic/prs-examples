#include <mpi.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// STRUKTURE

struct Car {
	int id;
	char mark[50];
	char model[50];
	char generationName[50];
	int year;
	int mileage;
	int volEngine;
	char fuel[50];
	char city[50];
	char province[50];
	int price;
};

struct CarYearAvgPrice {
	int year;
	float avgPrice;
};

const int NUM_ROWS = 600000;
const int START_YEAR = 1990; // NAJMANJA GODINA U CSV FAJLU JE 1995
const int END_YEAR = 2024;
const int NUM_YEARS = END_YEAR - START_YEAR + 1;

// FUNKCIJE

void loadData(const std::string& fileName, Car* cars);
MPI_Datatype createCustomCarType();
float calculateAvgPricePerRank(Car* carsSlice, int NUM_CARS_PER_RANK);
MPI_Datatype createCustomCarYearAvgPriceType();
void calculateYearAvgPricePerRank(Car* carsSlice, CarYearAvgPrice* carYearAvgPricePerRank, unsigned long* totalPrices, int* counts, int NUM_CARS_PER_RANK);
void calculateTotalYearAvgPrice(CarYearAvgPrice* finalResults, CarYearAvgPrice* totalCarYearAvgPrice, int ranksCount);
void displayMenu();
void useMenu(CarYearAvgPrice* finalResults, float totalAvgPrice, int ranksCount);

int main(int argc, char* argv[]) {

	Car* cars = nullptr;

	MPI_Init(&argc, &argv);

	int rank, ranksCount;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm_size(MPI_COMM_WORLD, &ranksCount);

	// UCITAVANJE PODATAKA

	if (rank == 0) {
		cars = new Car[NUM_ROWS];
		std::cout << "UCITAVANJE PODATAKA..." << std::endl;
		loadData("random_car_data_600k.csv", cars);
	}

	// KREIRANJE CUSTOM CAR TIPA

	MPI_Datatype MPI_CAR = createCustomCarType();

	MPI_Type_commit(&MPI_CAR);

	// SCATTER

	int NUM_CARS_PER_RANK = NUM_ROWS / ranksCount;

	Car* carsSlice = new Car[NUM_CARS_PER_RANK];

	MPI_Scatter(cars, NUM_CARS_PER_RANK, MPI_CAR, carsSlice, NUM_CARS_PER_RANK, MPI_CAR, 0, MPI_COMM_WORLD);

	// ISPIS PRVOG ID-a ZA SVAKI RANK (PO 150000 PODATAKA)

	//std::cout << "Rank: " << rank << " Prvi Car ID po ranku: " << carsSlice[0].id << std::endl;

	// RACUNANJE PROSJECNE CIJENE PO RANKU

	float totalAvgPrice = 0;

	float avgPricePerRank = calculateAvgPricePerRank(carsSlice, NUM_CARS_PER_RANK);

	// ISPIS PROSJECNE CIJENE PO RANKU

	//std::cout << "Rank: " << rank << " Prosjecna cijena po ranku: " << avgPricePerRank << std::endl;

	// REDUCE

	MPI_Reduce(&avgPricePerRank, &totalAvgPrice, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	// KREIRANJE CUSTOM CAR_YEAR_AVG_PRICE TIPA

	MPI_Datatype MPI_CAR_YEAR_AVG_PRICE = createCustomCarYearAvgPriceType();

	MPI_Type_commit(&MPI_CAR_YEAR_AVG_PRICE);

	// RACUNANJE PROSJECNE CIJENE PO GODINAMA ZA SVAKI RANK

	CarYearAvgPrice* carYearAvgPricePerRank = new CarYearAvgPrice[NUM_YEARS];

	unsigned long* totalPrices = new unsigned long[NUM_YEARS](); 
	int* counts = new int[NUM_YEARS]();

	calculateYearAvgPricePerRank(carsSlice, carYearAvgPricePerRank, totalPrices, counts, NUM_CARS_PER_RANK);

	// KREIRANJE I INICIJALIZACIJA NIZA REZULTATA ZA RANK 0

	CarYearAvgPrice* totalCarYearAvgPrice = nullptr;

	if (rank == 0) {
		totalCarYearAvgPrice = new CarYearAvgPrice[NUM_YEARS * ranksCount];
		for (int i = 0; i < NUM_YEARS; i++) {
			totalCarYearAvgPrice[i].year = START_YEAR + (i % NUM_YEARS);
			totalCarYearAvgPrice[i].avgPrice = 0.0f;
		}
	}

	// GATHER

	MPI_Gather(carYearAvgPricePerRank, NUM_YEARS, MPI_CAR_YEAR_AVG_PRICE, totalCarYearAvgPrice, NUM_YEARS, MPI_CAR_YEAR_AVG_PRICE, 0, MPI_COMM_WORLD);

	// RACUNANJE UKUPNIH PROSJECNIH CIJENA PO GODINAMA
	
	if (rank == 0) {
		CarYearAvgPrice* finalResults = new CarYearAvgPrice[NUM_YEARS];
		calculateTotalYearAvgPrice(finalResults, totalCarYearAvgPrice, ranksCount);
		useMenu(finalResults, totalAvgPrice, ranksCount);
		delete[] finalResults;
	}

	// OSLOBADJANJE MEMORIJE

	delete[] carsSlice;
	delete[] carYearAvgPricePerRank;
	delete[] totalPrices;
	delete[] counts;
	if (rank == 0) {
		delete[] cars;
		delete[] totalCarYearAvgPrice;
	}

	MPI_Type_free(&MPI_CAR);
	MPI_Type_free(&MPI_CAR_YEAR_AVG_PRICE);

	MPI_Finalize();

	return 0;

}

// FUNKCIJE

void loadData(const std::string& fileName, Car* cars) {
	std::ifstream file(fileName);
	std::string line;

	std::getline(file, line);

	for (int i = 0; i < NUM_ROWS; i++) {
		std::string temp;
		std::getline(file, line);
		std::stringstream ss(line);
		ss >> cars[i].id;
		ss.ignore(1, ',');
		std::getline(ss, temp, ',');
		strncpy_s(cars[i].mark, temp.c_str(), sizeof(cars[i].mark) - 1);
		cars[i].mark[sizeof(cars[i].mark) - 1] = '\0';
		std::getline(ss, temp, ',');
		strncpy_s(cars[i].model, temp.c_str(), sizeof(cars[i].model) - 1);
		cars[i].model[sizeof(cars[i].model) - 1] = '\0';
		std::getline(ss, temp, ',');
		strncpy_s(cars[i].generationName, temp.c_str(), sizeof(cars[i].generationName) - 1);
		cars[i].generationName[sizeof(cars[i].generationName) - 1] = '\0';
		ss >> cars[i].year;
		ss.ignore(1, ',');
		ss >> cars[i].mileage;
		ss.ignore(1, ',');
		ss >> cars[i].volEngine;
		ss.ignore(1, ',');
		std::getline(ss, temp, ',');
		strncpy_s(cars[i].fuel, temp.c_str(), sizeof(cars[i].fuel) - 1);
		cars[i].fuel[sizeof(cars[i].fuel) - 1] = '\0';
		std::getline(ss, temp, ',');
		strncpy_s(cars[i].city, temp.c_str(), sizeof(cars[i].city) - 1);
		cars[i].city[sizeof(cars[i].city) - 1] = '\0';
		std::getline(ss, temp, ',');
		strncpy_s(cars[i].province, temp.c_str(), sizeof(cars[i].province) - 1);
		cars[i].province[sizeof(cars[i].province) - 1] = '\0';
		ss >> cars[i].price;
	}

	file.close();
}

MPI_Datatype createCustomCarType() {

	MPI_Datatype MPI_CAR;

	int* blockLengths = new int[11] { 1, 50, 50, 50, 1, 1, 1, 50, 50, 50, 1 };

	int idOffest = offsetof(Car, id);
	int markOffest = offsetof(Car, mark);
	int modelOffest = offsetof(Car, model);
	int generationNameOffest = offsetof(Car, generationName);
	int yearkOffest = offsetof(Car, year);
	int mileagelOffest = offsetof(Car, mileage);
	int volEngineOffest = offsetof(Car, volEngine);
	int fuelOffest = offsetof(Car, fuel);
	int cityOffest = offsetof(Car, city);
	int provincedOffest = offsetof(Car, province);
	int priceOffest = offsetof(Car, price);

	MPI_Aint* arrayOfOffsets = new MPI_Aint[11]{ idOffest, markOffest, modelOffest, generationNameOffest, yearkOffest,
												mileagelOffest, volEngineOffest, fuelOffest, cityOffest, provincedOffest, priceOffest };

	MPI_Datatype* dataTypes = new MPI_Datatype[11]{ MPI_INT, MPI_CHAR, MPI_CHAR, MPI_CHAR, MPI_INT, MPI_INT, MPI_INT, MPI_CHAR, MPI_CHAR, MPI_CHAR, MPI_INT };

	MPI_Type_create_struct(11, blockLengths, arrayOfOffsets, dataTypes, &MPI_CAR);

	return MPI_CAR;
}

float calculateAvgPricePerRank(Car* carsSlice, int NUM_CARS_PER_RANK) {
	unsigned long long priceSumPerRank = 0;

	for (int i = 0; i < NUM_CARS_PER_RANK; i++) {
		priceSumPerRank += carsSlice[i].price;
	}
	
	float avgPricePerRank = static_cast<float>(priceSumPerRank) / NUM_CARS_PER_RANK;

	return avgPricePerRank;
}

MPI_Datatype createCustomCarYearAvgPriceType() {

	MPI_Datatype MPI_CAR_YEAR_AVG_PRICE;

	int* blockLengths = new int[2] { 1, 1 };

	int yearkOffest = offsetof(CarYearAvgPrice, year);
	int avgPriceOffest = offsetof(CarYearAvgPrice, avgPrice);

	MPI_Aint* arrayOfOffsets = new MPI_Aint[2]{ yearkOffest, avgPriceOffest };

	MPI_Datatype* dataTypes = new MPI_Datatype[2]{ MPI_INT, MPI_FLOAT };

	MPI_Type_create_struct(2, blockLengths, arrayOfOffsets, dataTypes, &MPI_CAR_YEAR_AVG_PRICE);

	return MPI_CAR_YEAR_AVG_PRICE;
}

void calculateYearAvgPricePerRank(Car* carsSlice, CarYearAvgPrice* carYearAvgPricePerRank, unsigned long* totalPrices, int* counts, int NUM_CARS_PER_RANK) {
	for (int i = 0; i < NUM_YEARS; i++) {
		carYearAvgPricePerRank[i].year = START_YEAR + i;
		carYearAvgPricePerRank[i].avgPrice = 0.0;
	}

	for (int i = 0; i < NUM_CARS_PER_RANK; i++) {
		int year = carsSlice[i].year;
		int index = year - START_YEAR;
		if (index >= 0 && index < NUM_YEARS) {
			totalPrices[index] += carsSlice[i].price;
			counts[index] += 1;
		}
	}

	for (int i = 0; i < NUM_YEARS; i++) {
		if (counts[i] > 0) {
			carYearAvgPricePerRank[i].avgPrice = static_cast<float>(totalPrices[i]) / counts[i];
		}
		else {
			carYearAvgPricePerRank[i].avgPrice = 0.0;
		}
	}
}

void calculateTotalYearAvgPrice(CarYearAvgPrice* finalResults, CarYearAvgPrice* totalCarYearAvgPrice, int ranksCount) {
	for (int i = 0; i < NUM_YEARS; i++) {
		finalResults[i].year = START_YEAR + i;
		finalResults[i].avgPrice = 0.0f;
	}

	for (int i = 0; i < NUM_YEARS; i++) {
		float total = 0.0f;
		int count = 0;

		for (int j = 0; j < ranksCount; j++) {
			int offset = j * NUM_YEARS + i;
			total += totalCarYearAvgPrice[offset].avgPrice;
			if (totalCarYearAvgPrice[offset].avgPrice > 0.0f) {
				count += 1;
			}
		}

		if (count > 0) {
			finalResults[i].avgPrice = total / count;
		}
	}
}

void displayMenu() {
	std::cout << "\nIzaberite opciju:\n";
	std::cout << "1. Prikaz prosjecne cijene\n";
	std::cout << "2. Prikaz prosjecne cijene po godinama\n";
	std::cout << "3. Kraj programa\n";
	std::cout << "Unesite opciju: ";
}

void useMenu(CarYearAvgPrice* finalResults, float totalAvgPrice, int ranksCount) {
	int option = 0;
	while (option != 3) {
		displayMenu();
		std::cin >> option;
		switch (option) {
		case 1:
			std::cout << "\nUkupna prosjecna cijena: " << totalAvgPrice / ranksCount << std::endl;
			break;
		case 2: 
		{
			std::cout << std::endl;
			for (int i = 0; i < NUM_YEARS; i++) {
				std::cout << "Godina: " << finalResults[i].year << " | Prosjecna cijena: " << finalResults[i].avgPrice << std::endl;
			}
			break;
		}
		case 3:
			std::cout << "Kraj programa..." << std::endl;
			break;
		default:
			std::cout << "Pogresna opcija. Pokusajte ponovo." << std::endl;
		}
	}
}