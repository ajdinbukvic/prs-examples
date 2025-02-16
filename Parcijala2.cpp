#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>

int main()
{
	// Zadatak 1

	#pragma omp parallel num_threads(4)
	{
		int size = omp_get_num_threads();
		int id = omp_get_thread_num();
		std::cout << "Nit: " << id << "od ukupno: " << size << std::endl;
	}

	// Zadatak 2

	const int SIZE = 100;
	int* niz_br = new int[SIZE];
	for (int i = 0; i < SIZE; i++) {
		niz_br[i] = i;
	}

	#pragma omp parallel for schedule(static,5)
	for (int i = 0; i < SIZE; i++)
	{
		int id = omp_get_thread_num();
		std::cout << "Nit: " << id << " broj: " << niz_br[i] << std::endl;
	}

	#pragma omp parallel for schedule(dynamic,10)
	for (int i = 0; i < SIZE; i++)
	{
		int id = omp_get_thread_num();
		std::cout << "Nit: " << id << " broj: " << niz_br[i] << std::endl;
	}

	// OBJASNJENJE:
	// Kod schedule(static,5) svakoj niti se dodjeljuju blokovi od po 5 fiksnih elemenata
	// nit 0: 0,1,2,3,4
	// nit 1: 5,6,7,8,9
	// ...
	// kada se dodje do niti 7 (jer ih ima ukupno 8 to je 5x8 = 40)
	// onda se ponovo rasporedjuju po 5 brojeva za svaku nit 
	// ponovo se krece od nulte sve dok se ne obradi cijeli niz
	// Kod schedule(dynamic,10) svaka nit u pocetku dobije blok od 10 brojeva za obradu
	// Razlika je u tome sto se ne zna koja ce nit obraditi koji blok
	// Kada svaka nit obradi po 10 brojeva to je ukupno 8x10 = 80
	// Preostala 2 bloka ce biti dodijeljena nitima koje prve zavrse sa obradom

	// Zadatak 3

	const int ARRAY_SIZE = 100;
	int* niz = new int[ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++) {
		niz[i] = i + 1;
	}

	int sum = 0;
	#pragma omp parallel for reduction(+:sum)
	for (int i = 0; i < ARRAY_SIZE; i++) {
		sum += niz[i];
	}
	// Sum: 5050
	std::cout << "Sum: " << sum << std::endl;

	// Zadatak 4

	int broj = 0;

	#pragma omp parallel
	{
		for (int i = 0; i < 10; i++)
		{
			#pragma omp critical 
			{
				broj++;
			}
		}
	}

	std::cout << "Broj omp critical: " << broj << std::endl;

	broj = 0;

	#pragma omp parallel
	{
		for (int i = 0; i < 10; i++)
		{
			#pragma omp atomic
			broj++;
		}
	}

	std::cout << "Broj omp atomic: " << broj << std::endl;

	broj = 0;

	omp_lock_t lock;
	omp_init_lock(&lock);
	#pragma omp parallel
	{
		for (int i = 0; i < 10; i++)
		{
			omp_set_lock(&lock);
			broj++;
			omp_unset_lock(&lock);
		}
	}
	omp_destroy_lock(&lock);
	std::cout << "Broj lock: " << broj << std::endl;

	// Zadatak 5
	
	int array1[] = {0,0,0,0};

	omp_lock_t lock2;
	omp_init_lock(&lock2);
	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		for (int i = 0; i < 4; i++)
		{
			omp_set_lock(&lock2);
			array1[i] += id;
			omp_unset_lock(&lock2);
		}
	}
	omp_destroy_lock(&lock2);

	std::cout << "Konacni niz: ";
	for (int i = 0; i < 4; i++) {
		std::cout << array1[i] << " , ";
	}
	std::cout << std::endl;

	//Koristenje omp_lock_t jer se radi o slucaju "azuriranje kompleksnog stanja"

	int array2[] = { 0,0,0,0,0,0,0,0 };
	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		array2[id]++;
	}

	std::cout << "Konacni niz: ";
	for (int i = 0; i < 8; i++) {
		std::cout << array2[i] << " , ";
	}
	std::cout << std::endl;

	//Bez kriticke sekcije jer se pristupa nizu preko indeksa koji je zapravo ID niti

	// Zadatak 6
	
	const int N = 100;
	int* is_prime = new int[N];
	for (int i = 0; i < N; i++) {
		is_prime[i] = 1;
	}
	int korijen = sqrt(N); //zaokruzuje korijen na cijeli broj
	#pragma omp parallel for
	for (int i = 2; i <= korijen; i++) 
	{
		for (int j = i*i; j <= N; j += i) 
		{
			#pragma omp critical
			{
				is_prime[j-1] = 0;
			}
		}
	}

	for (int i = 0; i < N; i++) {
		std::string status = is_prime[i] == 1 ? "prost" : "slozen";
		std::cout << "Broj: " << i+1 << " " << status << std::endl;
	}
}