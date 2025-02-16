#include <iostream>
#include <mpi.h>
#include <cstring>

void reverseWord(char* word) {
    int len = strlen(word);
    for (int i = 0; i < len / 2; i++) {
        std::swap(word[i], word[len - 1 - i]);
    }
}

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const char* recenica = "prva druga treca cetvrta";
    int redniBrojRijeci = 3;

    int redniBrojPackSize;
    int recenicaPackSize;
    int recenicaSize = strlen(recenica) + 1;

    MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &redniBrojPackSize);
    MPI_Pack_size(recenicaSize, MPI_CHAR, MPI_COMM_WORLD, &recenicaPackSize);

    int bufferSize = redniBrojPackSize + recenicaPackSize;
    char* buffer = new char[bufferSize] {};
    int position = 0;

    if (rank == 0) {

        MPI_Pack(recenica, recenicaSize, MPI_CHAR, buffer, bufferSize, &position, MPI_COMM_WORLD);
        MPI_Pack(&redniBrojRijeci, 1, MPI_INT, buffer, bufferSize, &position, MPI_COMM_WORLD);
    }

    MPI_Bcast(buffer, bufferSize, MPI_PACKED, 0, MPI_COMM_WORLD);

    position = 0;
    char* recenicaRec = new char[recenicaSize] {};
    int redniBrojRec;

    MPI_Unpack(buffer, bufferSize, &position, recenicaRec, recenicaSize, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, bufferSize, &position, &redniBrojRec, 1, MPI_INT, MPI_COMM_WORLD);


    if (rank == 1 || rank == 2) {
        char* recenicaCopy = new char[recenicaSize];
        strcpy_s(recenicaCopy, recenicaSize, recenicaRec);

        char* context = nullptr;
        char* rijec = strtok_s(recenicaCopy, " ", &context);
        int index = 1;

        char* recenicaBezRijeci = new char[recenicaSize + 1];
        recenicaBezRijeci[0] = '\0';

        while (rijec != nullptr) {
            if (rank == 1 && index == redniBrojRec) {
                reverseWord(rijec);
                std::cout << "Rank 1 (obrnuta rijec sa indeksa): " << rijec << std::endl;
            }
            if (rank == 2 && index != redniBrojRec) {
                strcat_s(recenicaBezRijeci, strlen(recenicaBezRijeci) + strlen(rijec) + 2, rijec);
                strcat_s(recenicaBezRijeci, strlen(recenicaBezRijeci) + 2, " ");
            }
            rijec = strtok_s(nullptr, " ", &context);
            index++;
        }
        if (rank == 2) {
            std::cout << "Rank 2 (recenica bez rijeci sa indeksa): " << recenicaBezRijeci << std::endl;
        }
    }

    if (rank == 3) {
        std::cout << "Rank 3 (cijela recenica): " << recenicaRec << std::endl;
    }

    delete[] buffer;

    MPI_Finalize();

    return 0;
}