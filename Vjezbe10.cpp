#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include <queue>
using namespace std;

std::queue<std::string> queue0;
std::queue<std::string> queue1;
std::queue<std::string> queue2;
std::queue<std::string> queue3;

void write(int i, std::string msg) {
	switch (i) {
	case 0: {
		queue0.push(msg);
		break;
	}
	case 1: {
		queue1.push(msg);
		break;
	}
	case 2: {
		queue2.push(msg);
		break;
	}
	case 3: {
		queue3.push(msg);
		break;
	}
	default: break;
	}
}

void read(int i) {
	switch (i) {
	case 0: {
		std::cout << queue0.front() << std::endl;
		queue0.pop();
		break;
	}
	case 1: {
		std::cout << queue1.front() << std::endl;
		queue1.pop();
		break;
	}
	case 2: {
		std::cout << queue2.front() << std::endl;
		queue2.pop();
		break;
	}
	case 3: {
		std::cout << queue3.front() << std::endl;
		queue3.pop();
		break;
	}
	default: break;
	}
}
int main()
{
	#pragma omp parallel num_threads(4)
	{
		for (int i = 0; i < 4; i++) {
			if (i != omp_get_thread_num())
			{
				#pragma omp critical
				{
					std::string msg = "thread" + to_string(omp_get_thread_num());
					write(i, msg);
				}
			}
		}
	}

	while (queue0.size() > 0) {
		read(0);
	}
	while (queue1.size() > 0) {
		read(1);
	}
	while (queue2.size() > 0) {
		read(2);
	}
	while (queue3.size() > 0) {
		read(3);
	}
}
