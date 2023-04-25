#include "hw3_p1_header.hpp"

template<typename T>
void mm_jik(T a, const std::vector<T>& A, const std::vector<T>& B, T b, std::vector<T>& C, int m, int p, int n) {
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < m; ++i) {
            for (int k = 0; k < p; ++k) {
                C[i + j * m] += a * A[i + k * m] * B[k + j * p];
            }
            C[i + j * m] *= b;
        }
    }
}