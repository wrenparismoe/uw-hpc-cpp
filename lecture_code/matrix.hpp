#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "mylibrary.hpp"
#include <iostream>
#include <vector>

// simple matrix class
class Matrix
{
public:
    Matrix(int, int);            // constructor
    ~Matrix();                   // destructor
    Matrix(const Matrix &other); // copy constructor

    // accessor methods - class functions that can access private foo
    int getRows() const { return rows_; }
    int getCols() const { return cols_; }
    double get_ij(int i, int j) const { return matrix_[i][j]; }
    void set_ij(int i, int j, double value) { matrix_[i][j] = value; }
    void print() const;
    // alternate reference notation ... A[i][j]
    // element access operators
    std::vector<double> &operator[](int i) { return matrix_[i]; }
    const std::vector<double> &operator[](int i) const { return matrix_[i]; }

    Matrix operator*(const Matrix &other) const; // matrix multiply
    Matrix operator+(const Matrix &other) const; // matrix addition
    Matrix operator*(double scalar) const;       // scale matrix
    Matrix operator-(const Matrix &other) const; // matrix subtraction is redundant

private:
    std::vector<std::vector<double>> matrix_;
    int rows_;
    int cols_;
};

#endif // MATRIX_HPP