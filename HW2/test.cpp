#include <vector>
#include <random>
#include <iostream>
// include boost library
// #include <boost/multiprecision/cpp_dec_float.hpp>

// Function that returns a set of randoming generated matrices A,B,C
std::vector<std::vector<double>> generateRandomMatrix (int m, int n = 0) {
    std::vector<std::vector<double>> A(m, std::vector<double>(n));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0,1.0); // range [0,1]
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = dist(gen);
        }
    }
    return A;
}

void printMatrix (const std::vector<std::vector<double>> &A) {
    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < A[i].size(); j++)
        {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int m = 3;
    int n = -1;
    //std::vector<std::vector<double>>A = generateRandomMatrix(m, p);
    std::vector<std::vector<double>> A(m, std::vector<double>(n));

    printMatrix(A);

    // print dimensions of A
    std::cout << "A -> (m,n)=(" << A.size() << "," << A[0].size() << ")" << std::endl;
}