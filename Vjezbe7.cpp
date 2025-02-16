#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>

int main()
{
    /*auto clock_start = std::chrono::system_clock::now();
    int k = 1000000000;
    float sum = 0;
    for (int i = 0; i < k; i++) {
        sum += (pow(-1, i) / (2 * i + 1));
    }
    float res = sum * 4;
    std::cout << res << std::endl;
    auto clock_now = std::chrono::system_clock::now();
    float currentTime = float(std::chrono::duration_cast <std::chrono::milliseconds> (clock_now - clock_start).count());
    std::cout << "Elapsed Time: " << currentTime << " MS \n";*/
    // ---------------

    /*auto clock_start = std::chrono::system_clock::now();
    int k = 1000000000;
    float sum = 0;
    #pragma omp parallel for num_threads(200)
    for (int i = 0; i < k; i++) {
        float t = i % 2 == 0 ? 1.0 : -1.0;
        sum += t / (2 * i + 1);
    }
    float res = sum * 4;
    std::cout << res << std::endl;
    auto clock_now = std::chrono::system_clock::now();
    float currentTime = float(std::chrono::duration_cast <std::chrono::milliseconds> (clock_now - clock_start).count());
    std::cout << "Elapsed Time: " << currentTime << " MS \n";*/
    //// ---------------

    /*auto clock_start = std::chrono::system_clock::now();
    int k = 1000000000;
    float sum = 0;
    #pragma omp parallel for num_threads(200)
    for (int i = 0; i < k; i++) {
    #pragma omp critical
        {
            float t = i % 2 == 0 ? 1.0 : -1.0;
            sum += t / (2 * i + 1);
        }
    }
    float res = sum * 4;
    std::cout << res << std::endl;
    auto clock_now = std::chrono::system_clock::now();
    float currentTime = float(std::chrono::duration_cast <std::chrono::milliseconds> (clock_now - clock_start).count());
    std::cout << "Elapsed Time: " << currentTime << " MS \n";*/

    //// ---------------
    auto clock_start = std::chrono::system_clock::now();
    int k = 1000000000;
    float sum = 0;
    #pragma omp parallel for num_threads(200) reduction(+:sum)
    for (int i = 0; i < k; i++) {
        float t = i % 2 == 0 ? 1.0 : -1.0;
        sum += t / (2 * i + 1);
    }
    float res = sum * 4;
    std::cout << res << std::endl;
    auto clock_now = std::chrono::system_clock::now();
    float currentTime = float(std::chrono::duration_cast <std::chrono::milliseconds> (clock_now - clock_start).count());
    std::cout << "Elapsed Time: " << currentTime << " MS \n";


}
