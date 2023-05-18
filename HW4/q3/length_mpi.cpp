#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <ios>
#include <fstream>

#include <mpi.h>

// Function f(x) = ln(x) - (1/8)x^2
double f(double x) {
    return std::log(x) - (1.0 / 8.0) * std::pow(x, 2);
}

double fprime(double x) {
    return 1.0/x - (1.0 / 4.0) * x;
}

double sequential_riemann_sum(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        double x = a + i * h;
        sum += std::sqrt(1 + std::pow(fprime(x), 2));
    }
    return h * sum;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size, len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &len);

    auto start = std::chrono::steady_clock::now();

    const double a = 1.0;  // Lower bound of the interval
    const double b = 6.0;  // Upper bound of the interval

    const int n = 1e8;  // Number of points to use for numerical integration

    double local_a = a + rank * (b - a) / size;
    double local_b = a + (rank + 1) * (b - a) / size;
    int local_n = n / size;

    double local_sum = sequential_riemann_sum(local_a, local_b, local_n);

    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        std::ofstream threads("threads.csv", std::ios_base::app | std::ios_base::out);
        threads << size << "," << elapsed * 1e-9 << std::endl;

        std::cout << "Result: " << global_sum << "(NThreads=" << size << ", time=" << elapsed << "ns)" << std::endl;
    }
    MPI_Finalize();

    return 0;
}
