#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <fstream>

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

// Function that returns a randoming generated scalar
double generateRandomScalar() {
    // if nis not specified, then return a random scalar
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0,1.0); // range [0,1]
    return dist(gen);
}

// Function that returns a set of randoming generated matrices A,B,C
std::vector<std::vector<double>> generateRandomMatrix (int m, int n = 0) {
    std::vector<std::vector<double>> A(m, std::vector<double>(n));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0,1.0); // range [0,1]
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = dist(gen);
        }
    }
    return A;
}

int main() {
    const int ntrial = 3;
    int n_min = 2;
    int n_max = 1024;

    long double micro_to_secondL = 1.e-6L;
    

    std::ofstream performanceCSV("performance.csv");
    performanceCSV << "n,elapsed_time,FLOPs" << std::endl;

    // n_min = n_max = 5;

    for (int n=n_min; n <= n_max; n+=2) {
        // Calculate the number of flops
        int m = n;
        int p = n;
        
        // long double flop_count = static_cast<long double>(ntrial * (m * n * (2*p + 1)));
        long double flop_count_per_trial = static_cast<long double>((m * n * (2*p + 1)));

        // Generate random A, B, and C matrices
        std::vector<std::vector<double>>A = generateRandomMatrix(m, p);
        std::vector<std::vector<double>>B = generateRandomMatrix(p, n);
        std::vector<std::vector<double>>C = generateRandomMatrix(m, n);
        double a = generateRandomScalar();
        double b = generateRandomScalar();

        // Start timer
        auto start = std::chrono::high_resolution_clock::now();

        // Code segment to time
        for (int j=0; j < ntrial; j++) {
            // Run dgemv function
            dgemm(a, A, B, b, C);
        }

        // Stop timer
        auto stop = std::chrono::high_resolution_clock::now();
        // Calculate the duration of the code segment
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        // Convert to long double
        long double durationL = static_cast<long double>(duration.count());

        // Output the duration to the console
        std::cout << "Total duration of daxpy runs (n=" << n << "): " << durationL << " microseconds" << std::endl;

        // long double flops = flop_count / (durationL * micro_to_secondL);
        long double flops_per_trial = flop_count_per_trial / (durationL * micro_to_secondL);
        long double flops = flops_per_trial * ntrial;

        // Output the flops value to the console
        std::cout << "daxpy performance (MFLOPs): " << flops / (1.e6L) << std::endl;

        // Write output to CSV file
        performanceCSV << n << "," << durationL << "," << flops << std::endl;
    }

    // Close the CSV file
    performanceCSV.close();

    return 0;
}
