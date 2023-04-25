"""
Write a C++ function that computes $y=a*A*x + b*y$ where $A \in \mathbb{R^{m \times n}}, x \in \mathbb{R^n}, y \in \mathbb{R^m}$ and $a,b \in \mathbb{R}$. Again, measure the performance for $n = 2$ to $n = 1024$. Let each $n$ be measured $ntrial$ times so that we can plot the performance for each case versus $n$. Use $ntrial=1000$ to avoid dividing by zero. The correctness of your function will be tested against a general m, n test system with known result Check for and flag incorrect cases. Use the following function structure to write your C++ code:
`void dgemv (double a, const std::vector<std::vector<double>> &A, const std::vector<double> &x, double b, std::vector <double> &y)`
"""
