#include "axpy.hpp"

template <typename T>
void axpy (T a, const std::vector<T> &x, std::vector<T> &y) {
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

// Explicit template instantiation for float and double
template void axpy<float>(float a, const std::vector<float> &x, std::vector<float> &y);
template void axpy<double>(double a, const std::vector<double> &x, std::vector<double> &y);