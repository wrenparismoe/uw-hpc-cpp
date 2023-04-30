#include "mm_jik.hpp"

template<typename T>
void mm_jik(T a, const std::vector<T>& A, const std::vector<T>& B, T b, std::vector<T>& C, int m, int p, int n) {
    for (int i = 0; i < m; ++i) {
        for (int k = 0; k < n; ++k) {
            C[i + k * m] *= b;
        }
    }

    for (int j = 0; j < p; j++) {
        for (int i = 0; i < m; i++) {
            T Atemp = A[i + j * m] * a;
            for (int k = 0; k < n; k++) {
                C[i + k * m] += Atemp * B[j + k * p];
            }
        }
    }
}

// Create explicit template instantiations
template void mm_jik(int a, const std::vector<int>& A, const std::vector<int>& B, int b, 
                        std::vector<int>& C, int m, int p, int n);
template void mm_jik(float a, const std::vector<float>& A, const std::vector<float>& B, float b,
                        std::vector<float>& C, int m, int p, int n);
template void mm_jik(double a, const std::vector<double>& A, const std::vector<double>& B, double b,
                        std::vector<double>& C, int m, int p, int n);