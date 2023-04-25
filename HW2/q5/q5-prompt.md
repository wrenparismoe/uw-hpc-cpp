```
#include <iostream>
#include <vector>
#include <stdexcept>

void daxpy (double a, const std::vector<double> &x, std::vector<double> &y) {
    // Handling for when x and y are different dimensions
    if (x.size() == 0 || y.size() == 0) {
        std::cout << "Error: x and y are empty" << std::endl;
        return;
    }

    if (x.size() != y.size()) {
        std::cout << "Error: x and y are different dimensions" << std::endl;
        return;
    }

    for (int i = 0; i < x.size(); i++) {
        y[i] += a * x[i];
    }
}

void daxpy_unroll(double a, const std::vector<double> &x, std::vector<double> &y, int blocksize, int unroll_factor=4) {
    int n = x.size();
    int i;
    
    // Run daxpy if blocksize greater than n 
    if (blocksize > n) {
        // print statement
        std::cout << "Blocksize greater than n, running daxpy" << std::endl;
        daxpy(a, x, y);
        return;
    }

    // Handling for when x and y are different dimensions
    if (x.size() != y.size()) {
        std::cout << "Error: x and y must be same dimensions" << std::endl;
        return;
    }
    
    unroll_factor = 8;
    for (i = 0; i < n - (n % (unroll_factor * blocksize)); i += unroll_factor * blocksize) {
        for (int j = 0; j < blocksize; j++) {
            y[i + j] += a * x[i + j];
            y[i + j + blocksize] += a * x[i + j + blocksize];
            y[i + j + 2 * blocksize] += a * x[i + j + 2 * blocksize];
            y[i + j + 3 * blocksize] += a * x[i + j + 3 * blocksize];
            y[i + j + 4 * blocksize] += a * x[i + j + 4 * blocksize];
            y[i + j + 5 * blocksize] += a * x[i + j + 5 * blocksize];
            y[i + j + 6 * blocksize] += a * x[i + j + 6 * blocksize];
            y[i + j + 7 * blocksize] += a * x[i + j + 7 * blocksize];
        }
    }

    for (; i < n; i++) {
        y[i] += a * x[i];
    }
}

void dgemv (double a, const std::vector<std::vector<double>> &A, 
            const std::vector<double> &x, double b, std::vector <double> &y) {
    int m = A.size();
    int n = A[0].size();
    // Handle when A, x, or y are incorrect dimensions
    if (n != x.size()) {
        std::cout << "Error: A and x do not have same dimension n" << std::endl;
        return;
    }
    if (m != y.size()) {
        std::cout << "Error: A and y do not have same dimension m" << std::endl;
        return;
    }
    for (int i = 0; i < m; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            y[i] += a * A[i][j] * x[j];
            // sum += A[i][j] * x[j];
        }
        // y[i] = a * sum + b * y[i];
        y[i] += b * y[i];
    }
}

bool checkMatrixDimensions(const std::vector<std::vector<double>> &A,
                     const std::vector<std::vector<double>> &B,
                     const std::vector<std::vector<double>> &C) {
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

// C = aAB + bC
void dgemm (double a, const std::vector<std::vector<double>> &A,
            const std::vector<std::vector<double>> &B, double b,
            std::vector<std::vector<double>> &C) {

    // Check if dimensions are correct for matrix multiplication
    if (!checkMatrixDimensions(A, B, C)) {
        return;
    }

    int m = A.size();
    int n = B[0].size();
    int p = A[0].size();

    // C = aAB + bC
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double sum = 0;
            for (int k = 0; k < p; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = a * sum + b * C[i][j];
        }
    }
}
```

I have the above code in C++ that runs Level 1, 2, and 3 BLAS operations with respective functions `daxpy`, `dgemv`, and `dgemm`. I want to convert this functions to C++ templates so that I can call each function from a separate `.cpp` file. The templates should be named `axpy`, `gemv`, and `gemm`. The goal is to construct a shared object library, `librefBLAS.so`, from a header file `refBLAS.hpp` that contains the specification of each function. I also want a source file `refBLAS.cpp` that includes this header file and calls each function to test that the library is working correctly. Please write the C++ `.cpp` and `.hpp` files to accomplish this task.
The templates should be defined for types float and double, and should be constructed as follows:
```
template <typename T>
void axpy (T a, const std::vector<T> &x, std::vector<T> &y);

template <typename T>
void gemv (T a, const std::vector<std::vector<T>> &A, 
            const std::vector<T> &x, T b, std::vector <T> &y);

template <typename T>
void gemm (T a, const std::vector<std::vector<T>> &A,
            const std::vector<std::vector<T>> &B, T b,
            std::vector<std::vector<T>> &C);

```