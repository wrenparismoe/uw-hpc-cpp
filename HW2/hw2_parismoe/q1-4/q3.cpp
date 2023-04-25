#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include "blas.hpp"

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

int main() {
    const int ntrial = 4;
    int n_min = 2;
    int n_max = 1024;

    long double micro_to_secondL = 1.e-6L;

    std::ofstream performanceCSV("q3-performance.csv");
    performanceCSV << "n,elapsed_time,FLOPs" << std::endl;

    // n_min = n_max = 5;

    for (int n=n_min; n <= n_max; n++) {
        // Calculate the number of flops
        int m = n;
        long double flop_count = static_cast<long double>(ntrial * (m * (2*n + 1)));

        // Generate random A, x, y, a, b
        std::vector<double> x(n), y(m);
        std::vector<std::vector<double>> A(m, std::vector<double>(n));
        
        // Uncomment to use random generations rather than seeded
        std::random_device rd;
        std::mt19937 gen(rd());
        // const unsigned int seed_value = 12345;
        // std::mt19937 gen(seed_value);
        std::uniform_real_distribution<double> dist(0,1.0); // range [0,1]
        double a = dist(gen);
        double b = dist(gen);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                //A.set_ij(i, j, dist(gen));
                A[j][i] = dist(gen);
            }
            // generate random numbers and assign to vector x,y element index i
            x[i] = dist(gen); 
            y[i] = dist(gen);
        }

        // Start timer
        auto start = std::chrono::high_resolution_clock::now();

        // Code segment to time
        for (int j=0; j < ntrial; j++) {
            // Run dgemv function
            dgemv(a, A, x, b, y);
        }

        // Stop timer
        auto stop = std::chrono::high_resolution_clock::now();
        // Calculate the duration of the code segment
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        // Convert to long double
        long double durationL = static_cast<long double>(duration.count());

        // Output the duration to the console
        std::cout << "Total duration of daxpy runs (n=" << n << "): " << durationL << " microseconds" << std::endl;

        long double flops = flop_count / (durationL * micro_to_secondL);

        // Output the flops value to the console
        std::cout << "daxpy performance (MFLOPs): " << flops / (1.e6L) << std::endl;

        // Write output to CSV file
        performanceCSV << n << "," << durationL << "," << flops << std::endl;
    }

    // Close the CSV file
    performanceCSV.close();

    return 0;
}
