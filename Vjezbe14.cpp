#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;

const double PI = 3.14159265358979323846;


std::vector<complex<double>> calculate(std::vector<complex<double>> nums) {

int N = nums.size();
std::vector<complex<double>> output(N);
#pragma omp parallel for
for (int i = 0; i < N; i++) {
complex<double> sum (0,0);
for (int j = 0; j < N; j++) {
double x = (-2.0 * PI * j * i) / N;
complex<double> eix(cos(x), sin(x));
#pragma omp critical
{
sum += nums[j] * eix;
}
}
complex<double> num(sum.real(), abs(sum.imag()));
output[i] = num;
}
return output;
}


int main() {
std::vector<complex<double>> numbers = { 0, 0.5, 1, 1, 0.5, 0, -0.5, -1, -1, -0.5};
std::vector<complex<double>> out = calculate(numbers);
for (int i = 0; i < out.size(); i++) {
std::cout << out[i] << std::endl;
}
}