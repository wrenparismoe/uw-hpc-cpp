#include "strassen_mm.hpp"
#include <iostream>
#include <stdexcept>
#include <array>

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
            T sum = 0;
            for (int k = 0; k < cols_A; k++) {
                sum += A[i][k] * B[k][j];
            }
            result[i][j] = sum;
        }
    }

    return result;
}

template <typename T>
std::array<std::vector<std::vector<T>>,2> pad_matrices(const std::vector<std::vector<T>> &A,
                                        const std::vector<std::vector<T>> &B) {
    int m = A.size();
	int p = B.size();
	int n = B[0].size();
    
    // Pad matrices to be square dimensions of power 2
    int max_dim = std::max(m, std::max(p, n));
    int pad_dim = 1;
    while (pad_dim < max_dim) {
        pad_dim *= 2;
    }

    std::vector<std::vector<T>> A_pad(pad_dim, std::vector<T>(pad_dim, 0));
    std::vector<std::vector<T>> B_pad(pad_dim, std::vector<T>(pad_dim, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            A_pad[i][j] = A[i][j];
        }
    }
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < n; j++) {
            B_pad[i][j] = B[i][j];
        }
    }
    return {A_pad, B_pad};
}

template <typename T>
std::vector<std::vector<T>> strassen_mm_recursive(const std::vector<std::vector<T>> &A,
                                                  const std::vector<std::vector<T>> &B) {
    int m = A.size();
	int p = B.size();
	int n = B[0].size();

    std::vector<std::vector<T>> C(m, std::vector<T>(n));

    // Base case with significantly better performance (ie 36 MFLOPs --> 700+ MFLOPs)
    // if (n <= 32) {
    //     return standard_mm(A, B);
    // }

    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }
    else {
        int mid = n / 2;

        // Divide input matrices into 4 submatrices
        std::vector<std::vector<T>> A11(mid, std::vector<T>(mid));
        std::vector<std::vector<T>> A12(mid, std::vector<T>(mid));
        std::vector<std::vector<T>> A21(mid, std::vector<T>(mid));
        std::vector<std::vector<T>> A22(mid, std::vector<T>(mid));
        std::vector<std::vector<T>> B11(mid, std::vector<T>(mid));
        std::vector<std::vector<T>> B12(mid, std::vector<T>(mid));
        std::vector<std::vector<T>> B21(mid, std::vector<T>(mid));
        std::vector<std::vector<T>> B22(mid, std::vector<T>(mid));

        // FIXME: Try changing loop to iterate over m instead of mid?
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
        // M1 = (A11 + A22) (B11 + B22)
        std::vector<std::vector<T>> M1 = strassen_mm_recursive(add_matrices(A11, A22), add_matrices(B11, B22));
        // M2 = (A21 + A22) B11
        std::vector<std::vector<T>> M2 = strassen_mm_recursive(add_matrices(A21, A22), B11);
        // M3 = A11 (B12 − B22)
        std::vector<std::vector<T>> M3 = strassen_mm_recursive(A11, subtract_matrices(B12, B22));
        // M4 = A22 (B21 − B11)
        std::vector<std::vector<T>> M4 = strassen_mm_recursive(A22, subtract_matrices(B21, B11));
        // M5 = (A11 + A12) B22
        std::vector<std::vector<T>> M5 = strassen_mm_recursive(add_matrices(A11, A12), B22);
        // M6 = (A21 − A11) (B11 + B12)
        std::vector<std::vector<T>> M6 = strassen_mm_recursive(subtract_matrices(A21, A11), add_matrices(B11, B12));
        // M7 = (A12 − A22) (B21 + B22)
        std::vector<std::vector<T>> M7 = strassen_mm_recursive(subtract_matrices(A12, A22), add_matrices(B21, B22));

        // Calculate the submatrices of C
        // C11 = M1 + M4 - M5 + M7
        std::vector<std::vector<T>> C11 = add_matrices(subtract_matrices(add_matrices(M1, M4), M5), M7);
        // C12 = M3 + M5
        std::vector<std::vector<T>> C12 = add_matrices(M3, M5);
        // C21 = M2 + M4
        std::vector<std::vector<T>> C21 = add_matrices(M2, M4);
        // C22 = M1 - M2 + M3 + M6
        std::vector<std::vector<T>> C22 = add_matrices(subtract_matrices(add_matrices(M1, M3), M2), M6);

        // Combine the resulting submatrices into the final matrix
        for (int i = 0; i < mid; i++) {
            for (int j = 0; j < mid; j++) {
                C[i][j] = C11[i][j];
                C[i][j + mid] = C12[i][j];
                C[i + mid][j] = C21[i][j];
                C[i + mid][j + mid] = C22[i][j];
            }
        }
    }
    return C;
}

template <typename T>
std::vector<std::vector<T>> strassen_mm(const std::vector<std::vector<T>> &A,
                                        const std::vector<std::vector<T>> &B) {
    int m = A.size();
	int p = B.size();
	int n = B[0].size();

    if (n != p || n != m) {
        throw std::invalid_argument("Matrices must be square and of equal dimensions");
    }

    std::vector<std::vector<T>> C(m, std::vector<T>(n));

    // Check if dimensions are a power of 2, if not pad to the next power of 2
    if ( !((n & (n - 1)) == 0) ) {
        std::array<std::vector<std::vector<T>>,2> padded_matrices = pad_matrices(A, B);
        std::vector<std::vector<T>> A_pad = padded_matrices[0];
        std::vector<std::vector<T>> B_pad = padded_matrices[1];

        std::vector<std::vector<T>> C_pad = strassen_mm_recursive(A_pad, B_pad);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                C[i][j] = C_pad[i][j];
            }
        }

    } else {
        C = strassen_mm_recursive(A, B);
    }

    return C;
}

// Explicit template instantiation
template std::vector<std::vector<int>> standard_mm(const std::vector<std::vector<int>> &A, 
                                                   const std::vector<std::vector<int>> &B);
template std::vector<std::vector<float>> standard_mm(const std::vector<std::vector<float>> &A, 
                                                     const std::vector<std::vector<float>> &B);
template std::vector<std::vector<double>> standard_mm(const std::vector<std::vector<double>> &A, 
                                                      const std::vector<std::vector<double>> &B);

template std::vector<std::vector<int>> add_matrices(const std::vector<std::vector<int>> &A, 
                                                  const std::vector<std::vector<int>> &B);
template std::vector<std::vector<float>> add_matrices(const std::vector<std::vector<float>> &A, 
                                                    const std::vector<std::vector<float>> &B);
template std::vector<std::vector<double>> add_matrices(const std::vector<std::vector<double>> &A, 
                                                     const std::vector<std::vector<double>> &B);

template std::vector<std::vector<int>> subtract_matrices(const std::vector<std::vector<int>> &A, 
                                                       const std::vector<std::vector<int>> &B);
template std::vector<std::vector<float>> subtract_matrices(const std::vector<std::vector<float>> &A, 
                                                         const std::vector<std::vector<float>> &B);
template std::vector<std::vector<double>> subtract_matrices(const std::vector<std::vector<double>> &A, 
                                                          const std::vector<std::vector<double>> &B);

template std::vector<std::vector<int>> strassen_mm(const std::vector<std::vector<int>> &A, 
                                                   const std::vector<std::vector<int>> &B);
template std::vector<std::vector<float>> strassen_mm(const std::vector<std::vector<float>> &A, 
                                                     const std::vector<std::vector<float>> &B);
template std::vector<std::vector<double>> strassen_mm(const std::vector<std::vector<double>> &A, 
                                                      const std::vector<std::vector<double>> &B);