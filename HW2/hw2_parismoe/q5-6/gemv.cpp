#include "gemv.hpp"

template <typename T>
void gemv (T a, const std::vector<std::vector<T>> &A, 
            const std::vector<T> &x, T b, std::vector <T> &y) {
    int m = A.size();
    int n = A[0].size();
    // Handle when A, x, or y are incorrect dimensions
    if (n != x.size()) {
        std::cout << "Error: A and x do not have same dimension n" << std::endl;
        return;
    }
    if (m != y.size()) {
        std::cout << "Error: A and y do not have same dimension m" << std::endl;
        return;
    }
    for (int i = 0; i < m; i++) {
        T sum = 0;
        for (int j = 0; j < n; j++) {
            y[i] += a * A[i][j] * x[j];
            // sum += A[i][j] * x[j];
        }
        // y[i] = a * sum + b * y[i];
        y[i] += b * y[i];
    }
}

// Explicit template instantiation for float and double
template void gemv<float>(float a, const std::vector<std::vector<float>> &A,
                          const std::vector<float> &x, float b, std::vector<float> &y);
template void gemv<double>(double a, const std::vector<std::vector<double>> &A,
                           const std::vector<double> &x, double b, std::vector<double> &y);