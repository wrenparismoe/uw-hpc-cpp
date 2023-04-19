#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <fstream>


void daxpy (double a, const std::vector<double> &x, std::vector<double> &y) {
    // Handling for when x and y are different dimensions
    if (x.size() != y.size()) {
        std::cout << "Error: x and y are different dimensions" << std::endl;
        return;
    }

    for (int i = 0; i < x.size(); i++) {
        y[i] += a * x[i];
    }
}

void daxpy_unroll(double a, const std::vector<double> &x, std::vector<double> &y, int blocksize, int unroll_factor=4) {
    int n = x.size();
    int i;
    
    // Run daxpy if blocksize greater than n 
    if (blocksize > n) {
        // print statement
        std::cout << "Blocksize greater than n, running daxpy" << std::endl;
        daxpy(a, x, y);
        return;
    }

    // Handling for when x and y are different dimensions
    if (x.size() != y.size()) {
        std::cout << "Error: x and y must be same dimensions" << std::endl;
        return;
    }
    
    for (i = 0; i < n - (n % (blocksize * unroll_factor)); i += blocksize * unroll_factor) {
        for (int j = 0; j < blocksize; j++) {
            for (int k = 0; k < unroll_factor; k++) {
                y[i + j + k * blocksize] += a * x[i + j + k * blocksize];
            }
        }
    }

    for (; i < n; i++) {
        y[i] += a * x[i];
    }
}

int main() {
    const int ntrial = 10000;
    int n = 2048;
    std::vector<int> blocks = {1, 2, 4, 8, 16, 32, 64};

    long double micro_to_secondL = 1.e-6L;
    
    // Calculate the number of flops
    long double flop_count = static_cast<long double>(ntrial * (2 * n));
    
    // Generate random x, y, a
    std::vector<double> x(n), y(n);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0,1.0); // range [0,1]
    double a = dist(gen);
    for (int i = 0; i < n; ++i) {
        x[i] = dist(gen); // generate random number and assign to vector element index i
        y[i] = dist(gen);
    }

    std::ofstream performanceCSV("performance.csv");
    performanceCSV << "blocksize,elapsed_time,FLOPs" << std::endl;

    for (auto &blocksize : blocks) {
        // Start timer
        auto start = std::chrono::high_resolution_clock::now();

        // Code segment to time
        for (int j=0; j < ntrial; j++) {
            // Run daxpy function
            daxpy_unroll(a, x, y, blocksize, 4);
        }

        // Stop timer
        auto stop = std::chrono::high_resolution_clock::now();
        // Calculate the duration of the code segment
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        // Convert to long double
        long double durationL = static_cast<long double>(duration.count());

        // Output the duration to the console
        std::cout << "Total duration of unroll_daxpy runs (n=" << n << "): " << durationL << " microseconds" << std::endl;

        long double flops = flop_count / (durationL * micro_to_secondL);

        // Output the flops value to the console
        std::cout << "unroll_daxpy performance (MFLOPs): " << flops / (1.e6L) << std::endl;

        // Write output to CSV file
        performanceCSV << blocksize << "," << durationL << "," << flops << std::endl;
    }

    // Close the CSV file
    performanceCSV.close();

    return 0;
}
