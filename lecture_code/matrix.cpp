#include "matrix.hpp"
#include <iostream>
#include <vector>

// simple matrix class functions
Matrix::Matrix(int rows, int cols)
    : matrix_(rows, std::vector<double>(cols)), rows_(rows), cols_(cols)
{ // matrix constructor
}

Matrix::~Matrix()
{ // deallocate the memory used by the vector of vectors
    matrix_.clear();
    std::cout << "Matrix destructed" << std::endl;
}

Matrix::Matrix(const Matrix &other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_)
{ // copy constructor
}

void Matrix::print() const
{
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < cols_; j++)
        {
            std::cout << matrix_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::operator*(const Matrix &other) const
{ // operator overload multiplication operator *, A * B
    if (cols_ != other.rows_)
    {
        throw std::invalid_argument("Matrices are not compatible for multiplication");
    }
    Matrix result(rows_, other.cols_);
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < other.cols_; j++)
        {
            double sum = 0;
            for (int k = 0; k < cols_; k++)
            {
                sum += matrix_[i][k] * other.matrix_[k][j];
            }
            result.set_ij(i, j, sum);
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix &other) const
{ // operator overload addition operator +, A + B
    if (rows_ != other.rows_ || cols_ != other.cols_)
    {
        throw std::invalid_argument("Matrices are not compatible for addition");
    }
    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < cols_; j++)
        {
            result.set_ij(i, j, matrix_[i][j] + other.matrix_[i][j]);
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const
{ // scalar multiplication on matrix elements *, a * A
    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < cols_; j++)
        {
            result.matrix_[i][j] = scalar * matrix_[i][j];
        }
    }
    return result;
}

Matrix operator*(double scalar, const Matrix &matrix)
{ // scalar multiplication on matrix elements *, A * a
    return matrix * scalar;
}

Matrix Matrix::operator-(const Matrix &other) const
{ // operator overload addition operator -, A - B
    if (rows_ != other.rows_ || cols_ != other.cols_)
    {
        throw std::invalid_argument("Matrix dimensions must match");
    }

    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < cols_; j++)
        {
            result.set_ij(i, j, matrix_[i][j] - other.matrix_[i][j]);
        }
    }

    return result;
}