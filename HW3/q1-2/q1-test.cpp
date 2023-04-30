// Test that the header and function files are working together

#include "hw3_p1_header.hpp"
#include <iostream>
#include <vector>

int main() {
    int m = 3;
    int p = 2;
    int n = 4;
    std::vector<double> A = {1, 2, 3, 4, 5, 6};
    std::vector<double> B = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<double> C = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    std::cout << "mm_ijk: ";
    mm_ijk(1.5, A, B, 2.0, C, m, p, n);
    for (int i = 0; i < m*n; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;

    C = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::cout << "mm_ikj: ";
    mm_ikj(1.5, A, B, 2.0, C, m, p, n);
    for (int i = 0; i < m*n; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;

    C = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::cout << "mm_kij: ";
    mm_kij(1.5, A, B, 2.0, C, m, p, n);
    for (int i = 0; i < m*n; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;


    C = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::cout << "mm_jik: ";
    mm_jik(1.5, A, B, 2.0, C, m, p, n);
    for (int i = 0; i < m*n; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;


    C = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::cout << "mm_kji: ";
    mm_kji(1.5, A, B, 2.0, C, m, p, n);
    for (int i = 0; i < m*n; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;


    C = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::cout << "mm_jki: ";
    mm_jki(1.5, A, B, 2.0, C, m, p, n);
    for (int i = 0; i < m*n; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}