#include "strassen_mm.hpp"
#include <iostream>
#include <stdexcept>

template <typename T>
std::vector<std::vector<T>> add_matrices(const std::vector<std::vector<T>> &A,
                                         const std::vector<std::vector<T>> &B) {
    int rows = A.size();
    int cols = A[0].size();
    std::vector<std::vector<T>> result(rows, std::vector<T>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }

    return result;
}

template <typename T>
std::vector<std::vector<T>> subtract_matrices(const std::vector<std::vector<T>> &A,
                                              const std::vector<std::vector<T>> &B) {
    int rows = A.size();
    int cols = A[0].size();
    std::vector<std::vector<T>> result(rows, std::vector<T>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }

    return result;
}

template <typename T>
std::vector<std::vector<T>> standard_mm(const std::vector<std::vector<T>> &A,
                                        const std::vector<std::vector<T>> &B) {
    int rows_A = A.size();
    int cols_A = A[0].size();
    int rows_B = B.size();
    int cols_B = B[0].size();

    if (cols_A != rows_B) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }

    std::vector<std::vector<T>> result(rows_A, std::vector<T>(cols_B, 0));

    for (int i = 0; i < rows_A; i++) {
        for (int j = 0; j < cols_B; j++) {
            for (int k = 0; k < cols_A; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

template <typename T>
std::vector<std::vector<T>> strassen_mm(const std::vector<std::vector<T>> &A,
                                        const std::vector<std::vector<T>> &B) {
    int m = A.size();
	int p = B.size();
	int n = B[0].size();

    if (n <= 2) {
        return standard_mm(A, B);
    }

    int mid = n / 2;

    // Divide input matrices into 4 submatrices
    // Divide input matrices into 4 submatrices
    std::vector<std::vector<T>> A11(mid, std::vector<T>(mid));
    std::vector<std::vector<T>> A12(mid, std::vector<T>(mid));
    std::vector<std::vector<T>> A21(mid, std::vector<T>(mid));
    std::vector<std::vector<T>> A22(mid, std::vector<T>(mid));
    std::vector<std::vector<T>> B11(mid, std::vector<T>(mid));
    std::vector<std::vector<T>> B12(mid, std::vector<T>(mid));
    std::vector<std::vector<T>> B21(mid, std::vector<T>(mid));
    std::vector<std::vector<T>> B22(mid, std::vector<T>(mid));
    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + mid];
            A21[i][j] = A[i + mid][j];
            A22[i][j] = A[i + mid][j + mid];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + mid];
            B21[i][j] = B[i + mid][j];
            B22[i][j] = B[i + mid][j + mid];
        }
    }


    // Calculate the 7 products using Strassen's formula
    auto P1 = strassen_mm(A11, subtract_matrices(B12, B22));
    auto P2 = strassen_mm(add_matrices(A11, A12), B22);
    auto P3 = strassen_mm(add_matrices(A21, A22), B11);
    auto P4 = strassen_mm(A22, subtract_matrices(B21, B11));
    auto P5 = strassen_mm(add_matrices(A11, A22), add_matrices(B11, B22));
    auto P6 = strassen_mm(subtract_matrices(A12, A22), add_matrices(B21, B22));
    auto P7 = strassen_mm(subtract_matrices(A11, A21), add_matrices(B11, B12));

    // Calculate the resulting submatrices
    auto C11 = add_matrices(subtract_matrices(add_matrices(P5, P4), P2), P6);
    auto C12 = add_matrices(P1, P2);
    auto C21 = add_matrices(P3, P4);
    auto C22 = add_matrices(subtract_matrices(add_matrices(P5, P1), P3), P7);

    // Combine the resulting submatrices into the final matrix
    std::vector<std::vector<T>> C(n, std::vector<T>(n));
    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            C[i][j] = C11[i][j];
            C[i][j + mid] = C12[i][j];
            C[i + mid][j] = C21[i][j];
            C[i + mid][j + mid] = C22[i][j];
        }
    }

    return C;
}

// Explicit template instantiation
template std::vector<std::vector<int>> strassen_mm(const std::vector<std::vector<int>> &A,
                                                   const std::vector<std::vector<int>> &B);
template std::vector<std::vector<float>> strassen_mm(const std::vector<std::vector<float>> &A,
                                                     const std::vector<std::vector<float>> &B);
template std::vector<std::vector<double>> strassen_mm(const std::vector<std::vector<double>> &A,
                                                      const std::vector<std::vector<double>> &B);
