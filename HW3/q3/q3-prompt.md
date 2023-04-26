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
