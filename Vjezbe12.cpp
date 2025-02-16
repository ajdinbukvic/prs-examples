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
    // 1

    /*const int NUM_THREADS = 40;
    const int NUM_ITERATIONS = 1000000;
    int count = 0;

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            #pragma omp atomic
            count++;
        }
    }

    std::cout << "Vrijednost brojaca: " << count << std::endl;*/

    // 2

    /*std::srand(std::time(nullptr));
    const int THREAD_COUNT[] = { 4, 7, 20, 100 };
    const int THREAD_ITERATIONS = sizeof(THREAD_COUNT) / sizeof(THREAD_COUNT[0]);
    const int ARRAY_SIZE = 20000;
    int* array = new int[ARRAY_SIZE];

    for (int i = 0; i < ARRAY_SIZE; i++) 
    {
        array[i] = std::rand() % 1000 + 1;
    }

    for (int i = 0; i < THREAD_ITERATIONS; i++) {
        int numThreads = THREAD_COUNT[i];
        std::vector<std::string> data;
        #pragma omp parallel num_threads(numThreads)
        {
            int threadId = omp_get_thread_num();
            int size = ARRAY_SIZE / numThreads;
            int start = threadId * size;
            int end = (threadId == numThreads - 1) ? ARRAY_SIZE : start + size;

            float sum = 0;
            for (int j = start; j < end; j++) {
                sum += array[j];
            }
            float average = sum / (end - start);

            #pragma omp critical
            {
                data.push_back("Thread " + std::to_string(threadId) +
                    ", iteracija " + std::to_string(numThreads) +
                    ", rezultat: " + std::to_string(average));
            }

            std::ofstream file("data_" + std::to_string(numThreads) + ".txt");
            for (const auto& line : data) {
                file << line << std::endl;
            }
        }
    }

    std::cout << "Podaci su spremljeni u fajlove." << std::endl;*/

    // 3

    /*std::srand(std::time(nullptr));
    const int ARRAY_SIZE = 20000;
    int* array = new int[ARRAY_SIZE];

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = std::rand() % 1000 + 1;
    }

    const int THREADS_COUNT = 10;
    int* results = new int[THREADS_COUNT];

    omp_lock_t lock;
    omp_init_lock(&lock);

    #pragma omp parallel num_threads(THREADS_COUNT)
    {
        int threadId = omp_get_thread_num();
        int size = ARRAY_SIZE / THREADS_COUNT;
        int start = threadId * size;
        int end = (threadId == THREADS_COUNT - 1) ? ARRAY_SIZE : start + size;

        float sum = 0;
        for (int i = start; i < end; i++) {
            sum += array[i];
        }
        float average = sum / (end - start);
      
        omp_set_lock(&lock);
        results[threadId] = average;
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);

    for (int i = 0; i < THREADS_COUNT; i++) {
        std::cout << "Thread " << i << " rezultat: " << results[i] << std::endl;
    }*/

}