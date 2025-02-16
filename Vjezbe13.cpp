#include <iostream>
#include <omp.h>
#include <random>
#include <cmath>

int factorial(int n)
{
    unsigned long long f = 1;
    for (long long i = 1; i <= n; ++i) {
        f *= i;
    }
    return f;
}

int main()
{
    int M = 5;
    int n = 0;
    int thread_num = 2;

    #pragma omp parallel num_threads(thread_num)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        for (int i = 0; i < M; i++) {
            int x = dis(gen);
            if (x == 1) {
                #pragma omp atomic
                n++;
            }
        }
    }
    std::cout << "Rezultat: " << static_cast<double>(n) / static_cast<double>(M * thread_num) << std::endl;
    std::cout << "Broj glava: " << n  << std::endl;
    int glava = n;
    double res = factorial(M * thread_num) / ((factorial(glava)) * factorial((M * thread_num) - glava));
    std::cout << "Formula: " << res << std::endl;
    double y = res * pow(0.5, glava) * pow(1.0 - 0.5, (M * thread_num) - glava);
    std::cout << "Final: " << y;
}