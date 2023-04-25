#include <chrono>
#include <random>
#include <fstream>
#include "refBLAS.hpp"

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

int main() {
    int n = 512;
    int m = 1024;
    int p = 256;

    // Test BLAS functions from refBLAS.hpp

    // axpy
    std::vector<double> x = generateRandomVector<double>(n);
    std::vector<double> y = generateRandomVector<double>(n);
    double a = generateRandomScalar<double>();
    axpy(a, x, y);
    std::cout << "axpy test passed" << std::endl;

    // gemv
    x = generateRandomVector<double>(n);
    y = generateRandomVector<double>(m);
    std::vector<std::vector<double>> A = generateRandomMatrix<double>(m, n);
    double b = generateRandomScalar<double>();
    gemv(a, A, x, b, y);
    std::cout << "gemv test passed" << std::endl;

    // gemm
    A = generateRandomMatrix<double>(m, p);
    std::vector<std::vector<double>> B = generateRandomMatrix<double>(p, n);
    std::vector<std::vector<double>> C = generateRandomMatrix<double>(m, n);
    gemm(a, A, B, b, C);
    std::cout << "gemm test passed" << std::endl;

    return 0;
}