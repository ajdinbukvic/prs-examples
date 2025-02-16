#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);

	int rank, ranksCount;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm_size(MPI_COMM_WORLD, &ranksCount);

	int unos = 0;

	if (rank == 0) {
		std::cout << "Unesite redni broj zadatka: ";
		std::cin >> unos;
	}

	MPI_Bcast(&unos, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (unos == 1) {
		std::cout << "Hello from: " << rank << " proces" << std::endl;
	}
	if (unos == 2) {
		if (rank == 0) {
			MPI_Send(&unos, sizeof(int), MPI_INT, 1, 0, MPI_COMM_WORLD);
		}
		if (rank == 1) {
			int unosRecv;
			MPI_Recv(&unosRecv, sizeof(int), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			std::cout << "Primljena vrijednost na ranku 1: " << unosRecv << std::endl;
		}
	}
	if (unos == 3) {
		MPI_Bcast(&unos, 1, MPI_INT, 0, MPI_COMM_WORLD);
		std::cout << "Broj koji je primio " << rank << " je: " << unos << std::endl;
	}
	if (unos == 4) {
		MPI_Bcast(&unos, 1, MPI_INT, 0, MPI_COMM_WORLD);
		int kvadrat = unos * unos;
		int sumaKvadrata;
		MPI_Reduce(&kvadrat, &sumaKvadrata, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		if (rank == 0) {
			std::cout << "Suma kvadrata: " << sumaKvadrata << std::endl;
		}
	}
	if (unos == 5) {
		int* niz = new int[5];
		for (int i = 0; i < 5; i++) {
			niz[i] = i + rank;
		}
		int brojElemenata = 5 * ranksCount;
		int* nizoviProcesa = new int[brojElemenata] {};
		MPI_Gather(niz, 5, MPI_INT, nizoviProcesa, 5, MPI_INT, 0, MPI_COMM_WORLD);
		if (rank == 0) {
			int suma = 0;
			for (int i = 0; i < brojElemenata; i++) {
				suma += nizoviProcesa[i];
			}
			float prosjek = static_cast<float>(suma) / brojElemenata;
			std::cout << "Prosjek iznosi: " << prosjek << std::endl;
		}
	}
	if (unos == 6) {
		int cijeliBroj = 10;
		double realanBroj = 8.2;
		char nizKaraktera[10] = {'a','b','c','d','e','f','g','h','i','j'};

		int cijeliBrojPackSize;
		int realanBrojPackSize;
		int nizKarakteraPackSize;

		MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &cijeliBrojPackSize);
		MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &realanBrojPackSize);
		MPI_Pack_size(10, MPI_CHAR, MPI_COMM_WORLD, &nizKarakteraPackSize);

		int bufferSize = cijeliBrojPackSize + realanBrojPackSize + nizKarakteraPackSize;

		if (rank == 0) {
			char* buffer = new char[bufferSize] {};
			int position = 0;

			MPI_Pack(&cijeliBroj, 1, MPI_INT, buffer, bufferSize, &position, MPI_COMM_WORLD);
			MPI_Pack(&realanBroj, 1, MPI_DOUBLE, buffer, bufferSize, &position, MPI_COMM_WORLD);
			MPI_Pack(&nizKaraktera, 10, MPI_CHAR, buffer, bufferSize, &position, MPI_COMM_WORLD);

			MPI_Send(buffer, bufferSize, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
		}
		if (rank == 1) {
			char* buffer = new char[bufferSize] {};

			int primljeniCijeliBroj;
			double primljeniRealniBroj;
			char primljeniNizKaraktera[10] {};

			MPI_Recv(buffer, bufferSize, MPI_PACKED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			int position = 0;

			MPI_Unpack(buffer, bufferSize, &position, &primljeniCijeliBroj, 1, MPI_INT, MPI_COMM_WORLD);
			MPI_Unpack(buffer, bufferSize, &position, &primljeniRealniBroj, 1, MPI_DOUBLE, MPI_COMM_WORLD);
			MPI_Unpack(buffer, bufferSize, &position, &primljeniNizKaraktera, 10, MPI_CHAR, MPI_COMM_WORLD);

			std::cout << "Primljeni cijeli broj: " << primljeniCijeliBroj << std::endl;;
			std::cout << "Primljeni realni broj: " << primljeniRealniBroj << std::endl;
			std::cout << "Primljeni niz karaktera: ";

			for (int i = 0; i < 10; i++) {
				std::cout << primljeniNizKaraktera[i] << ",";
			}
		}
	}

	MPI_Finalize();

	return 0;

}