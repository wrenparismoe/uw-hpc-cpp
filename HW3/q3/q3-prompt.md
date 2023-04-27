# Strassen Prompt (Q3)

Strassen Matrix Multiplication. Implement a C++ template for the (recursive) Strassen matrix multiplication algorithm. Write a `.cpp` function file that implements the algorithm and a `.hpp` header file that contains the function template definition/prototype. The function should take two matrices as input and return the product matrix. The function should be templated so that it can be used with matrices of any type (e.g., `int`, `double`, `float`, etc.). The function should be recursive and should use the base case of the standard matrix multiplication algorithm when the matrices are small enough. Use the following template structure:

```
template <typename T>
vector<vector<T>> strassen_mm(const vector<vector<T>> &A,
const vector<vector<T>> &B); // vector<vector<double>>C=strassen_mm(A, B);
```



## Part 2

In the below C++ template function `strassen_mm`, we are implementing Strassen's algorithm for recursive matrix multiplication. Help me fill in the missing code by completing the part where we need to:
1. "Divide input matrices into 4 submatrices", and
2. "Combine the resulting submatrices into the final matrix".

```
template <typename T>
std::vector<std::vector<T>> strassen_mm(const std::vector<std::vector<T>> &A,
                                        const std::vector<std::vector<T>> &B) {
    int n = A.size();

    if (n <= 2) {
        return standard_mm(A, B);
    }

    int mid = n / 2;

    // Divide input matrices into 4 submatrices
    // ... (code to divide matrices)

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
    // ... (code to combine matrices)

    return C;
}
```


## Part 3

"""
87 142 0 200 151 0 0 228 126 0 191 231 0 0 
88 98 0 174 179 0 0 191 117 0 149 175 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 
61 94 0 181 173 0 0 209 81 0 67 113 0 0 
22 71 0 93 90 0 0 137 39 0 95 106 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 
121 117 0 230 194 0 0 339 164 0 204 281 0 0 
77 98 0 188 175 0 0 249 142 0 109 193 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 
44 84 0 170 112 0 0 151 69 0 111 97 0 0 
47 109 0 131 76 0 0 152 73 0 159 164 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0
"""

I have an implementation of Strassen's algorithm for recursive matrix multiplication that is producing incorrect results. I am testing it for square matrices of only even dimensions $n x n$. For some dimensions, the algorithm will get the correct result, but for others it will produce intermitent rows and columns of zeros like in the triple quotations above. I have implemented the algorithm in the below C++ template function `strassen_mm`. Help me find the bug in the code. I have a suspicion it may be due to the base case, but that may be an incorrect assumption. See what you can find.
```
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
    // M1 = (A11 + A22) (B11 + B22)
    std::vector<std::vector<T>> M1 = strassen_mm(add_matrices(A11, A22), add_matrices(B11, B22));
    // M2 = (A21 + A22) B11
    std::vector<std::vector<T>> M2 = strassen_mm(add_matrices(A21, A22), B11);
    // M3 = A11 (B12 − B22)
    std::vector<std::vector<T>> M3 = strassen_mm(A11, subtract_matrices(B12, B22));
    // M4 = A22 (B21 − B11)
    std::vector<std::vector<T>> M4 = strassen_mm(A22, subtract_matrices(B21, B11));
    // M5 = (A11 + A12) B22
    std::vector<std::vector<T>> M5 = strassen_mm(add_matrices(A11, A12), B22);
    // M6 = (A21 − A11) (B11 + B12)
    std::vector<std::vector<T>> M6 = strassen_mm(subtract_matrices(A21, A11), add_matrices(B11, B12));
    // M7 = (A12 − A22) (B21 + B22)
    std::vector<std::vector<T>> M7 = strassen_mm(subtract_matrices(A12, A22), add_matrices(B21, B22));

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
```
