#include "hw3_p1_header.hpp"

template<typename T>
void mm_kji(T a, const std::vector<T>& A, const std::vector<T>& B, T b, std::vector<T>& C, int m, int p, int n) {
    for (int k = 0; k < p; ++k) {
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < m; ++i) {
                C[i + j * m] += a * A[i + k * m] * B[k + j * p];
            }
        }
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i + j * m] *= b;
        }
    }
}