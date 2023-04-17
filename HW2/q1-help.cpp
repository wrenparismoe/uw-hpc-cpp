#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>

void daxpy(double a, const std::vector<double> &x, std::vector<double> &y) {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }

    for (size_t i = 0; i < x.size(); ++i) {
        y[i] += a * x[i];
    }
}

int main() {
    const int ntrial = 3;
    std::ofstream data("performance_data.csv");
    std::mt19937 gen(1); // Random number generator
    std::uniform_real_distribution<> dis(1, 2);

    data << "n,FLOPs" << std::endl;

    for (int n = 2; n <= 1024; n *= 2) {
        double totalTime = 0;

        for (int trial = 0; trial < ntrial; ++trial) {
            std::vector<double> x(n), y(n);
            double alpha = dis(gen);

            for (int i = 0; i < n; ++i) {
                x[i] = dis(gen);
                y[i] = dis(gen);
            }

            auto start = std::chrono::high_resolution_clock::now();
            daxpy(alpha, x, y);
            auto end = std::chrono::high_resolution_clock::now();

            totalTime += std::chrono::duration<double>(end - start).count();
        }

        double avgTime = totalTime / ntrial;
        double FLOPs = (n * 2) / avgTime; // 2 floating-point operations per element
        
        data << n << "," << FLOPs << std::endl;
    }

    data.close();
    return 0;
}

// Agent 

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <fstream>

// The provided daxpy function goes here.
void daxpy(double a, const std::vector<double> &x, std::vector<double> &y) {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Input vectors x and y must have the same size");
    }

    for (size_t i = 0; i < x.size(); ++i) {
        y[i] += a * x[i];
    }
}

int main() {
    const int ntrial = 3; // You can change the number of trials.
    int min_n = 2;
    int max_n = 1024;
    double alpha = 1.5; // You can change the scaling factor.

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1000.0, 1000.0);

    for (int n = min_n; n <= max_n; n <<= 1) {
        double total_time = 0;

        for (int trial = 0; trial < ntrial; ++trial) {
            std::vector<double> x(n);
            std::vector<double> y(n);

            for (int i = 0; i < n; ++i) {
                x[i] = dist(gen);
                y[i] = dist(gen);
            }

            auto start = std::chrono::high_resolution_clock::now();
            daxpy(alpha, x, y);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            total_time += elapsed.count();
        }

        // Calculate average time per trial and compute FLOPs.
        double avg_time = total_time / ntrial;
        double flops = (2.0 * n) / avg_time;

        std::cout << "n = " << n << "; FLOPs = " << flops << std::endl;
    }

    return 0;
}

// Final Agent

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>
#include <cmath>
#include <fstream>

// daxpy implementation
void daxpy(double a, const std::vector<double> &x, std::vector<double> &y) {
    for (size_t i = 0; i < x.size(); i++) {
        y[i] = a * x[i] + y[i];
    }
}

// Helper function to check the correctness of the daxpy function
bool test_daxpy(double a, const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &result) {
    const double epsilon = 1e-6;
    std::vector<double> y_copy(y);

    daxpy(a, x, y_copy);

    for (size_t i = 0; i < result.size(); i++) {
        if (std::abs(y_copy[i] - result[i]) > epsilon) {
            return false;
        }
    }

    return true;
}

int main() {
    std::mt19937 gen(0); // random number generator, seeded with 0
    std::uniform_real_distribution<double> dist(-1, 1);

    const int ntrial = 3;

    std::ofstream performanceFile("performance.csv");
    performanceFile << "n,avg_time,FLOPs" << std::endl;

    for (int n = 2; n <= 1024; n *= 2) {
        std::vector<double> x(n), y(n), result(n);
        double a = dist(gen);

        for (int i = 0; i < n; ++i) {
            x[i] = dist(gen);
            y[i] = dist(gen);
            result[i] = a * x[i] + y[i];
        }

        if (!test_daxpy(a, x, y, result)) {
            std::cerr << "Error: daxpy function has incorrect output for n = " << n << std::endl;
            continue;
        }

        double accum_elapsed_time = 0.0;

        for (int trial = 0; trial < ntrial; ++trial) {
            std::vector<double> y_trial(y);
            auto start = std::chrono::steady_clock::now();

            daxpy(a, x, y_trial);

            auto end = std::chrono::steady_clock::now();
            accum_elapsed_time += std::chrono::duration<double, std::milli>(end - start).count();
        }

        double avg_time = accum_elapsed_time / ntrial;
        double FLOPs = static_cast<double>(n) / (avg_time * 1e-3);

        performanceFile << n << "," << avg_time << "," << FLOPs << std::endl;
    }

    performanceFile.close();

    return 0;
}


// Bing
#include <iostream>
#include <vector>
#include <chrono>
#include <random>

void daxpy(double a, const std::vector<double> &x, std::vector<double> &y) {
    for (int i = 0; i < x.size(); i++) {
        y[i] = a * x[i] + y[i];
    }
}

int main() {
    std::vector<double> performance;
    int ntrial = 3;
    for (int n = 2; n <= 1024; n *= 2) {
        double avg_time = 0;
        for (int trial = 0; trial < ntrial; trial++) {
            std::vector<double> x(n), y(n);
            std::default_random_engine generator;
            std::uniform_real_distribution<double> distribution(-1.0,1.0);
            for (int i = 0; i < n; i++) {
                x[i] = distribution(generator);
                y[i] = distribution(generator);
            }
            double a = distribution(generator);

            auto start = std::chrono::high_resolution_clock::now();
            daxpy(a, x, y);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            avg_time += duration;
        }
        avg_time /= ntrial;
        performance.push_back(avg_time);
    }

    for (int i = 0; i < performance.size(); i++) {
        std::cout << "n=" << (i + 1) * 2 << ", avg_time=" << performance[i] << "ns" << std::endl;
    }

    return 0;
}