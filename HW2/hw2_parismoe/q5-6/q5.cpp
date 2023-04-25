#include <chrono>
#include <random>
#include <fstream>
#include "axpy.hpp"
#include "gemv.hpp"
#include "gemm.hpp"

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
std::vector<T> generateRandomVector (int n) {
    std::vector<T> x(n);
    std::uniform_real_distribution<T> dist(0,1.0); // range [0,1]
    for (int i = 0; i < n; ++i) {
        x[i] = dist(gen);
    }
    return x;
}
template std::vector<float> generateRandomVector<float>(int n);
template std::vector<double> generateRandomVector<double>(int n);

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


// Main test function
int main() {
    const int ntrial = 3;
    int n_min = 2;
    int n_max = 1024; // 1024;

    long double nano_to_secondL = 1.e-9L;

    std::ofstream L1performanceCSV("L1-performance.csv");
    L1performanceCSV << "n,time,FLOPs" << std::endl;
    std::ofstream L2performanceCSV("L2-performance.csv");
    L2performanceCSV << "n,time,FLOPs" << std::endl;
    std::ofstream L3performanceCSV("L3-performance.csv");
    L3performanceCSV << "n,time,FLOPs" << std::endl;

    for (int n=n_min; n <= n_max; n+=2) {
        int m = n;
        int p = n;

        // Calculate the number of flops
        long double axpy_flop_count_per_trial = static_cast<long double>((2 * n));
        long double gemv_flop_count_per_trial = static_cast<long double>((m * (2*n + 1)));
        long double gemm_flop_count_per_trial = static_cast<long double>((m * n * (2*p + 1)));

        // Generate random x, y, a
        std::vector<float> x = generateRandomVector<float>(p);
        std::vector<float> y = generateRandomVector<float>(m);
        std::vector<std::vector<float>> A = generateRandomMatrix<float>(m, p);
        std::vector<std::vector<float>> B = generateRandomMatrix<float>(p, n);
        std::vector<std::vector<float>> C = generateRandomMatrix<float>(m, n);
        float a = generateRandomScalar<float>();
        float b = generateRandomScalar<float>();

        // Time axpy function
        auto start = std::chrono::high_resolution_clock::now();
        for (int j=0; j < ntrial; j++) {
            axpy(a, x, y);
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        long double durationL = static_cast<long double>(duration.count());
        // std::cout << "Total duration of axpy runs (n=" << n << "): " << durationL << " nanoseconds" << std::endl;
        long double flops_per_trial = axpy_flop_count_per_trial / (durationL * nano_to_secondL);
        long double flops = flops_per_trial * ntrial;
        std::cout << "(n=" << n << ") axpy performance (MFLOPs): " << flops / (1.e6L) << std::endl;
        L1performanceCSV << n << "," << durationL << "," << flops << std::endl;
    

        // Time gemv function
        start = std::chrono::high_resolution_clock::now();
        for (int j=0; j < ntrial; j++) {
            gemv(a, A, x, b, y);
        }
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        durationL = static_cast<long double>(duration.count());
        // std::cout << "Total duration of gemv runs (n=" << n << "): " << durationL << " nanoseconds" << std::endl;
        flops_per_trial = gemv_flop_count_per_trial / (durationL * nano_to_secondL);
        flops = flops_per_trial * ntrial;
        std::cout << "(n=" << n << ") gemv performance (MFLOPs): " << flops / (1.e6L) << std::endl;
        L2performanceCSV << n << "," << durationL << "," << flops << std::endl;


        // Time gemm function
        start = std::chrono::high_resolution_clock::now();
        for (int j=0; j < ntrial; j++) {
            gemm(a, A, B, b, C);
        }
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        durationL = static_cast<long double>(duration.count());
        // std::cout << "Total duration of gemm runs (n=" << n << "): " << durationL << " nanoseconds" << std::endl;
        flops_per_trial = gemm_flop_count_per_trial / (durationL * nano_to_secondL);
        flops = flops_per_trial * ntrial;
        std::cout << "(n=" << n << ") gemm performance (MFLOPs): " << flops / (1.e6L) << std::endl;
        L3performanceCSV << n << "," << durationL << "," << flops << std::endl;
    }

    // Close the CSV file
    L1performanceCSV.close();
    L2performanceCSV.close();
    L3performanceCSV.close();

    return 0;
}