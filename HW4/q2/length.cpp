#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include <chrono>
#include <fstream>
#include <mutex>

/*
Length of a graph of f(x) on [a,b] is $L = \int_a^b \sqrt{1 + f'(x)^2} dx$. Let f(x) = ln(x) - 1/8 x^2 on [1,6].
find the length of f on the interval analytically. You will need this to calculate the error in your code.
Write a C++ code parallelized using the C++ threads library that numerically evaluates the length of
this function on the interval. Your code should accept the number of points used to partition the interval
and the number of threads to spawn. Plot the strong scaling efficiency (time versus thread count) of your
code for n = 1.e8 partition points for 1 to 6 threads (or more). Plot the numerical error for n = 10 to
n = 1.e6 partition points, increasing by factors of 10 each time. Submit your code and plots.
*/

// Function f(x) = ln(x) - (1/8)x^2
double f(double x) {
    return std::log(x) - (1.0 / 8.0) * std::pow(x, 2);
}

// Numerical integration to calculate the length of the graph
double calculateLength(double a, double b, int numPoints)
{
    double dx = (b - a) / numPoints;
    double length = 0.0;

    for (int i = 0; i < numPoints; ++i)
    {
        double x = a + i * dx;
        double fPrime = 1 / x - (1 / 4.0) * x;
        double integrand = std::sqrt(1 + std::pow(fPrime, 2));
        length += integrand * dx;
    }

    return length;
}

double riemann_sum(double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; ++i)
    {
        double x = a + i * h;
        sum += f(x);
    }
    sum *= h;
    return sum;
}

void compute_partial_sum(double &sum, std::mutex &sum_mutex, double a, double h, int n, int num_threads, int i)
{
    double partial_sum = 0.0;
    int start = (n / num_threads) * i;
    int end = (i == num_threads - 1) ? n : (n / num_threads) * (i + 1);
    for (int j = start; j < end; ++j)
    {
        double x = a + j * h;
        partial_sum += f(x);
    }
    partial_sum *= h;

    // Lock the mutex and update the sum variable
    sum_mutex.lock();
    sum += partial_sum;
    sum_mutex.unlock();
}

double parallel_reimann_sum(double a, double b, int n, int num_threads)
{
    double h = (b - a) / n;
    double sum = 0.0;
    std::vector<std::thread> threads(num_threads);

    // Mutex for protecting the sum variable
    std::mutex sum_mutex;

    // Spawn threads to compute partial sums
    for (int i = 0; i < num_threads; ++i)
    {
        threads[i] = std::thread(compute_partial_sum, std::ref(sum), std::ref(sum_mutex), a, h, n, num_threads, i);
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; ++i)
    {
        threads[i].join();
    }
    return sum;
}

int main(int argc, char *argv[])
{
    const double a = 1.0; // Lower bound of the interval
    const double b = 6.0; // Upper bound of the interval

    const int numPoints = 1e8; // Number of points to use for numerical integration

    double length = calculateLength(a, b, 1e8);

    std::cout << "Length of f(x) on the interval [" << a << ", " << b << "]: " << length << std::endl;

    // Save results to a CSV file
    std::ofstream outputFile("results.csv");
    outputFile << "numThreads,elapsed_time" << std::endl;

    for (int numThreads = 1; numThreads <= 8; ++numThreads)
    {
        auto start = std::chrono::steady_clock::now();

        // Calculate length using multiple threads
        double sum_parallel = parallel_reimann_sum(a, b, numPoints, numThreads);

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "Number of threads: " << numThreads << ", Elapsed time: " << elapsed << " ms, Total Sum: " << sum_parallel << std::endl;

        // Save thread count and elapsed time to CSV file
        outputFile << numThreads << "," << elapsed << "\n";
    }

    outputFile.close();

    std::ofstream outputFile2("resultsError.csv");
    outputFile2 << "n,error" << std::endl;

    int n_min = 10;
    int n_max = 1e6;

    int threadCount = 6;

    for (int n = n_min; n <= n_max; n *= 10)
    {
        // Calculate length using multiple threads
        double totalLength = parallel_reimann_sum(a, b, n, threadCount);

        double error = length - totalLength;

        // Print n and error
        std::cout << "n: " << n << ", Error: " << error << std::endl;

        // Write to file
        outputFile2 << n << "," << error << "\n";
    }

    outputFile2.close();

    return 0;
}
