#include "gemm.hpp"

template <typename T>
bool checkMatrixDimensions(const std::vector<std::vector<T>> &A,
                     const std::vector<std::vector<T>> &B,
                     const std::vector<std::vector<T>> &C) {
    int A_m = A.size();
    int A_p = A[0].size();
    int B_p = B.size();
    int B_n = B[0].size();
    int C_m = C.size();
    int C_n = C[0].size();

    // Check if dimensions are correct for matrix multiplication
    if (A_m <= 0 || A_p <= 0 || B_p <= 0 || B_n <= 0 || C_m <= 0 || C_n <= 0) {
        throw std::invalid_argument("Matrix dimensions must be non-zero.");
        return false;
    }

    // Check if dimensions are compatible for multiplication and addition
    if (A_p != B_p || A_m != C_m || B_n != C_n) {
        throw std::invalid_argument("Matrix dimensions are not compatible for the operation.");
        return false;
    }

    return true;
}

template <typename T>
void gemm (T a, const std::vector<std::vector<T>> &A,
            const std::vector<std::vector<T>> &B, T b,
            std::vector<std::vector<T>> &C) {

    // Check if dimensions are correct for matrix multiplication
    if (!checkMatrixDimensions(A, B, C)) {
        return;
    }

    int m = A.size();
    int n = B[0].size();
    int p = A[0].size();

    // C = aAB + bC
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            T sum = 0;
            for (int k = 0; k < p; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = a * sum + b * C[i][j];
        }
    }
}

// Explicit template instantiation for float and double
template void gemm<float>(float a, const std::vector<std::vector<float>> &A,
                          const std::vector<std::vector<float>> &B, float b,
                          std::vector<std::vector<float>> &C);
template void gemm<double>(double a, const std::vector<std::vector<double>> &A,
                           const std::vector<std::vector<double>> &B, double b,
                           std::vector<std::vector<double>> &C);
template void gemm<long double>(long double a, const std::vector<std::vector<long double>> &A,
                           const std::vector<std::vector<long double>> &B, long double b,
                           std::vector<std::vector<long double>> &C);
