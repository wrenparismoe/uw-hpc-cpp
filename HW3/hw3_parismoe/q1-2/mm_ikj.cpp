#include "mm_ikj.hpp"


template<typename T>
void mm_ikj(T a, const std::vector<T>& A, const std::vector<T>& B, T b, std::vector<T>& C, int m, int p, int n) {
    for (int i = 0; i < m; i++) { // 3
        for (int k = 0; k < n; k++) { // 4
            T sum = 0;
            for (int j = 0; j < p; j++) { // 2
                sum += A[i + j * m] * B[j + k * p];
            }
            C[i + k * m] = a * sum + b * C[i + k * m];
        }
    }
}

// Create explicit template instantiations
template void mm_ikj(int a, const std::vector<int>& A, const std::vector<int>& B, int b, 
                        std::vector<int>& C, int m, int p, int n);
template void mm_ikj(float a, const std::vector<float>& A, const std::vector<float>& B, float b,
                        std::vector<float>& C, int m, int p, int n);
template void mm_ikj(double a, const std::vector<double>& A, const std::vector<double>& B, double b,
                        std::vector<double>& C, int m, int p, int n);