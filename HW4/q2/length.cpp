#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include <chrono>
#include <fstream>

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
double calculateLength(double a, double b, int numPoints) {
    double dx = (b - a) / numPoints;
    double length = 0.0;

    for (int i = 0; i < numPoints; ++i) {
        double x = a + i * dx;
        double fPrime = 1 / x - (1 / 4.0) * x;
        double integrand = std::sqrt(1 + std::pow(fPrime, 2));
        length += integrand * dx;
    }

    return length;
}

double riemann_sum(double a, double b, int n) {
	double h = (b - a) / n;
	double sum = 0.0;
	for (int i = 0; i < n; ++i) { 
		double x = a + i * h;
		sum += f(x);
	}
	sum *= h;
	return sum;
}

int main() {
    double a = 1.0;  // Lower bound of the interval
    double b = 6.0;  // Upper bound of the interval

    double length = calculateLength(a, b, 1e8);

    std::cout << "Length of f(x) on the interval [" << a << ", " << b << "]: " << length << std::endl;

    // Save results to a CSV file
    // std::ofstream outputFile("results.csv");
    // outputFile << "numThreads,elapsed_time" << std::endl;

    // for (int numThreads = 1; numThreads <= 8; ++numThreads) {
	// 	int numPoints = 1e8 / numThreads;

    //     auto start = std::chrono::steady_clock::now();

    //     // Split the interval equally across threads
    //     double intervalSize = (b - a) / numThreads;

    //     // Calculate length using multiple threads
    //     double totalLength = 0.0;
    //     std::vector<std::thread> threads(numThreads);
    //     for (int i = 0; i < numThreads; ++i) {
    //         double subIntervalStart = a + i * intervalSize;
    //         double subIntervalEnd = subIntervalStart + intervalSize;

    //         threads[i] = std::thread([&totalLength, subIntervalStart, subIntervalEnd, numPoints]() {
    //             double subLength = calculateLength(subIntervalStart, subIntervalEnd, numPoints);
    //             totalLength += subLength;
    //         });
    //     }

    //     // Join all the threads
    //     for (auto& thread : threads) {
    //         thread.join();
    //     }

    //     auto end = std::chrono::steady_clock::now();
    //     auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    //     std::cout << "Number of threads: " << numThreads << ", Elapsed time: " << elapsed << " ms, Total Sum: " << totalLength << std::endl;

    //     // Save thread count and elapsed time to CSV file
    //     outputFile << numThreads << "," << elapsed << "\n";
	// }
	
	// outputFile.close();

	std::ofstream outputFile2("resultsError.csv");
	outputFile2 << "n,error" << std::endl;

	int n_min = 10;
	int n_max = 1e6;

	int threadCount = 6;

	for (int n=n_min; n <= n_max; n *= 10) {
		int numPoints = n / threadCount;

        // Split the interval equally across threads
        double intervalSize = (b - a) / threadCount;

        // Calculate length using multiple threads
        double totalLength = 0.0;
        std::vector<std::thread> threads(threadCount);
        for (int i = 0; i < threadCount; ++i) {
            double subIntervalStart = a + i * intervalSize;
            double subIntervalEnd = subIntervalStart + intervalSize;

            threads[i] = std::thread([&totalLength, subIntervalStart, subIntervalEnd, numPoints]() {
                double subLength = riemann_sum(subIntervalStart, subIntervalEnd, numPoints);
                totalLength += subLength;
            });
        }

        // Join all the threads
        for (auto& thread : threads) {
            thread.join();
        }
		double error = abs(totalLength - length);

		// Print n and error
		std::cout << "n: " << n << ", Error: " << error << std::endl;

		// Write to file
		outputFile2 << n << "," << error << "\n";
	}

	outputFile2.close();

	return 0;
}







