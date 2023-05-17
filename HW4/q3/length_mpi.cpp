#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <fstream>

#include <mpi.h>

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

double parallel_reimann_sum(double a, double b, int n, int rank, int size) {
    double h = (b - a) / n;
    double sum = 0.0;
    double partial_sum = 0.0;

    // Calculate the range of indices for the current process
    int start = (n / size) * rank;
    int end = (rank == size - 1) ? n : (n / size) * (rank + 1);

    // Compute the partial sum for the current process
    for (int j = start; j < end; ++j) {
        double x = a + j * h;
        partial_sum += f(x);
    }
    partial_sum *= h;

    // Reduce the partial sums from all processes
    MPI_Reduce(&partial_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return sum;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size, len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &len);

    const double a = 1.0;  // Lower bound of the interval
    const double b = 6.0;  // Upper bound of the interval

    const int numPoints = 1e8;  // Number of points to use for numerical integration

    double length = calculateLength(a, b, numPoints);

    if (rank == 0) {
        std::cout << "Length of f(x) on the interval [" << a << ", " << b << "]: " << length << std::endl;

        // Save results to a CSV file
        std::ofstream outputFile("results.csv");
        outputFile << "numProcesses,elapsed_time" << std::endl;

        for (int numProcesses = 1; numProcesses <= size; ++numProcesses) {
            auto start = std::chrono::steady_clock::now();

            // Calculate length using multiple processes
            double sum_parallel = parallel_reimann_sum(a, b, numPoints, rank, numProcesses);

            auto end = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            std::cout << "Number of processes: " << numProcesses << ", Elapsed time: " << elapsed << " ms, Total Sum: " << sum_parallel << std::endl;

            // Save process count and elapsed time to CSV file
            outputFile << numProcesses << "," << elapsed << "\n";
        }

        outputFile.close();
    } else {
        // Calculate length using multiple processes
        parallel_reimann_sum(a, b, numPoints, rank, size);
    }

    MPI_Finalize();

    return 0;
}
