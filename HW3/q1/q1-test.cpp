// Test that the header and function files are working together

#include "hw3_p1_header.hpp"
#include <iostream>
#include <vector>

int main() {
    // FIX THIS
    std::vector<double> A = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> B = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> C(9);
    mm_ijk(1, A, B, 0, C, 3, 3, 3);
    for (int i = 0; i < 9; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;
    mm_ikj(1, A, B, 0, C, 3, 3, 3);
    for (int i = 0; i < 9; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;
    mm_kij(1, A, B, 0, C, 3, 3, 3);
    for (int i = 0; i < 9; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;
    mm_jik(1, A, B, 0, C, 3, 3, 3);
    for (int i = 0; i < 9; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;
    mm_kji(1, A, B, 0, C, 3, 3, 3);
    for (int i = 0; i < 9; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;
    mm_jki(1, A, B, 0, C, 3, 3, 3);
    for (int i = 0; i < 9; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}