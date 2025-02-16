#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <cstdlib>

int main()
{
    /*const int n = 10000000;
    int* niz = new int[n];
    for (int i = 0; i < n; i++) {
        niz[i] = i;
    }
    int suma = 0;
    auto start1 = omp_get_wtime();
    #pragma omp parallel for reduction(+:suma) schedule(static, 1)
    for (int i = 0; i < n; i++) {
        suma += niz[i];
    }
    auto end1 = omp_get_wtime();
    std::cout << "Vrijeme: " << end1 - start1 << std::endl;*/

    /*const int m = 20;
    std::vector<std::vector<double>> matrica;
    auto start2 = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            matrica[i][j] = sin(i);
        }
    }
    auto end2 = omp_get_wtime();
    std::cout << "Vrijeme: " << end2 - start2 << std::endl;*/

    /*srand(time(NULL));
    int size = 1000000;
    float* brojevi = new float[size];
    for (int i = 0; i < size; i++) {
        int random = rand() % size;
        brojevi[i] = random;
    }
    auto start3 = omp_get_wtime();
    #pragma omp parallel for schedule(guided, 1)
    for (int i = 0; i < 1000; i++) {
        brojevi[i] = sqrt(brojevi[i]);
    }
    auto end3 = omp_get_wtime();
    std::cout << "Vrijeme: " << end3 - start3 << std::endl; */
}
