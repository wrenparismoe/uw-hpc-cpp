#include "strassen_mm.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <random>
#include <fstream>
#include <chrono>

template <typename T>
void printMatrix (const std::vector<std::vector<T>> &A) {
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[i].size(); j++) {
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
    std::uniform_real_distribution<double> dist(0,10); // range [0,1] //TODO: CHANGE BACK TO T
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = dist(gen);
            // TODO: REMOVE
            A[i][j] = int(dist(gen));
        }
    }
    return A;
}
template std::vector<std::vector<int>> generateRandomMatrix<int>(int m, int n);
template std::vector<std::vector<float>> generateRandomMatrix<float>(int m, int n);
template std::vector<std::vector<double>> generateRandomMatrix<double>(int m, int n);


template <typename T>
std::vector<std::vector<T>> matrixMult(const std::vector<std::vector<T>> &A,
                                       const std::vector<std::vector<T>> &B) {
    int rows_A = A.size();
    int cols_A = A[0].size();
    int rows_B = B.size();
    int cols_B = B[0].size();

    std::vector<std::vector<T>> result(rows_A, std::vector<T>(cols_B));

    for (int i = 0; i < rows_A; i++) {
        for (int j = 0; j < cols_B; j++) {
            T sum = 0;
            for (int k = 0; k < cols_A; k++) {
                sum += A[i][k] * B[k][j];
            }
            result[i][j] = sum;
        }
    }
    return result;
}

template <typename T>
void checkEquality(const std::vector<std::vector<T>> &C,
                                       const std::vector<std::vector<T>> &C_true) {
    int m = C.size();
    int n = C[0].size();
    bool equal = true;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (!(C[i][j] == C_true[i][j])) {
                equal = false;
                break;
            }
        }
    }
    if (equal) {
        std::cout << "(n=" << n << ") Matrices are equal" << std::endl;
    } else {
        std::cout << "(n=" << n << ") Matrices are NOT equal" << std::endl;
    }
}


int main() {
	const int ntrial = 8;
    int n_min = 2;
    int n_max = 1024; // 1024;

    long double nano_to_secondL = 1.e-9L;

    std::ofstream performanceCSV("performance.csv");
    performanceCSV << "n,time,FLOPs" << std::endl;

    for (int n=n_min; n <= n_max; n*=2) {
        int m = n;
        int p = n;

        // Calculate the number of flops
        long double mm_flop_count_per_trial = static_cast<long double>((m * n * (2*p - 1)));

        // Initialize C
        std::vector<std::vector<double>> C(m, std::vector<double>(n));

        // Generate random A, B
        std::vector<std::vector<double>> A = generateRandomMatrix<double>(m, p);
        std::vector<std::vector<double>> B = generateRandomMatrix<double>(p, n);

        // Time strassen_mm function
        auto start = std::chrono::high_resolution_clock::now();
        for (int j=0; j < ntrial; j++) {
            C = strassen_mm(A, B);
        }

        // Uncomment for debugging - Compute matrix mult with standard process for comparison
        // std::vector<std::vector<double>> C_true = matrixMult(A, B);
        // std::cout << "True C: " << std::endl;
        // printMatrix(C_true);
        // checkEquality(C, C_true);

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
