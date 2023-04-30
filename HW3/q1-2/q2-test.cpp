#include "hw3_p1_header.hpp"
#include <chrono>
#include <random>
#include <iostream>
#include <vector>
#include <fstream>

// test perforamnce for {kij} and {jki} loops over ntrial>=3 from n=2 to n=512

static std::random_device rd;
static std::mt19937 gen(rd());

// Function that returns a randomly generated scalar
template <typename T>
T generateRandomScalar() {
    std::uniform_real_distribution<T> dist(0,1.0); // range [0,1]
    return dist(gen);
}
template float generateRandomScalar<float>();
template double generateRandomScalar<double>();

// Function that returns a randomly generated vector
template <typename T>
std::vector<T> generateRandomMatrix (int m, int n) {
    std::vector<T> x(m*n);
    std::uniform_real_distribution<T> dist(0,1.0); // range [0,1]
    for (int i = 0; i < n; ++i) {
        x[i] = dist(gen);
    }
    return x;
}
template std::vector<float> generateRandomMatrix<float>(int m, int n);
template std::vector<double> generateRandomMatrix<double>(int m, int n);


// Main test function
int main() {
    const int ntrial = 6;
    int n_min = 2;
    int n_max = 512; // 1024;

    long double nano_to_secondL = 1.e-9L;

    std::ofstream kij_performanceCSV("kij-performance.csv");
    kij_performanceCSV << "n,time,FLOPs" << std::endl;
    std::ofstream jki_performanceCSV("jki-performance.csv");
    jki_performanceCSV << "n,time,FLOPs" << std::endl;

    // Generate random a, b
    double a = generateRandomScalar<double>();
    double b = generateRandomScalar<double>();

    for (int n=n_min; n <= n_max; n++) {
        int m = n;
        int p = n;

        // Calculate the number of doubleing point operations
        long double gemm_flop_count_per_trial = static_cast<long double>((m * n * (2*p + 1)));

        // Generate random A, B, C
        std::vector<double> A = generateRandomMatrix<double>(m, p);
        std::vector<double> B = generateRandomMatrix<double>(p, n);
        std::vector<double> C = generateRandomMatrix<double>(m, n);

        // Time mm_kij function
        auto start = std::chrono::high_resolution_clock::now();
        for (int j=0; j < ntrial; j++) {
            mm_kij(a, A, B, b, C, m, p, n);
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        long double durationL = static_cast<long double>(duration.count());
        // std::cout << "Elapsed time of gemm runs (n=" << n << "): " << durationL << " nanoseconds" << std::endl;
        long double flops_per_trial = gemm_flop_count_per_trial / (durationL * nano_to_secondL);
        long double flops = flops_per_trial * ntrial;
        std::cout << "(n=" << n << ") kij performance (MFLOPs): " << flops / (1.e6L) << std::endl;
        kij_performanceCSV << n << "," << durationL << "," << flops << std::endl;
    

        // Time mm_jki function
        start = std::chrono::high_resolution_clock::now();
        for (int j=0; j < ntrial; j++) {
            mm_jki(a, A, B, b, C, m, p, n);
        }
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        durationL = static_cast<long double>(duration.count());
        // std::cout << "Elapsed time of gemm runs (n=" << n << "): " << durationL << " nanoseconds" << std::endl;
        flops_per_trial = gemm_flop_count_per_trial / (durationL * nano_to_secondL);
        flops = flops_per_trial * ntrial;
        std::cout << "(n=" << n << ") jki performance (MFLOPs): " << flops / (1.e6L) << std::endl;
        jki_performanceCSV << n << "," << durationL << "," << flops << std::endl;

    }

    // Close the CSV file
    kij_performanceCSV.close();
    jki_performanceCSV.close();

    return 0;
}