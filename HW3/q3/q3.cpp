#include "strassen_mm.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <random>
#include <fstream>
#include <chrono>

void printMatrix (const std::vector<std::vector<double>> &A) {
    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < A[i].size(); j++)
        {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

static std::random_device rd;
static std::mt19937 gen(rd());

// Function that returns a randomly generated matrix
template <typename T>
std::vector<std::vector<T>> generateRandomMatrix (int m, int n = 0) {
    std::vector<std::vector<T>> A(m, std::vector<T>(n));
    std::uniform_real_distribution<T> dist(0,1.0); // range [0,1]
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = dist(gen);
        }
    }
    return A;
}
template std::vector<std::vector<float>> generateRandomMatrix<float>(int m, int n);
template std::vector<std::vector<double>> generateRandomMatrix<double>(int m, int n);

int main() {
	const int ntrial = 3;
    int n_min = 2;
    int n_max = 1024; // 1024;

    long double nano_to_secondL = 1.e-9L;

    std::ofstream performanceCSV("performance.csv");
    performanceCSV << "n,time,FLOPs" << std::endl;

    for (int n=n_min; n <= n_max; n+=2) {
        int m = n;
        int p = n;

        // Calculate the number of flops
        long double mm_flop_count_per_trial = static_cast<long double>((m * n * (2*p - 1)));

        // Generate random A, B
        std::vector<std::vector<float>> A = generateRandomMatrix<float>(m, p);
        std::vector<std::vector<float>> B = generateRandomMatrix<float>(p, n);

        // Time axpy function
        auto start = std::chrono::high_resolution_clock::now();
        for (int j=0; j < ntrial; j++) {
            strassen_mm(A, B);
        }
		
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        long double durationL = static_cast<long double>(duration.count());
        
        long double flops_per_trial = mm_flop_count_per_trial / (durationL * nano_to_secondL);
        long double flops = flops_per_trial * ntrial;
		
        std::cout << "(n=" << n << ")  performance (MFLOPs): " << flops / (1.e6L) << std::endl;
		
        performanceCSV << n << "," << durationL << "," << flops << std::endl;
	}
	performanceCSV.close();
	return 0;
}
