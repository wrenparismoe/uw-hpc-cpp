"""
The following program implements a function for $y=a*x+y$ where $x,y \in \mathbb{R^n}, \alpha \in \mathbb{R}$ and measures the performance for $n = 2$ to $n = 1024$. Let each $n$ be measured $ntrial$ times so that we can plot the performance for each case versus $n$. Here, $ntrial=1000$ was used to avoid dividing by zero. Using the below code as a starting point, now implement a function that computes $y=a*A*x + b*y$ where $A \in \mathbb{R^{m \times n}}, x \in \mathbb{R^n}, y \in \mathbb{R^m}$ and $a,b \in \mathbb{R}$. Again, measure the performance for $n = 2$ to $n = 1024$. Use the following function structure:
`void dgemv (double a, const std::vector<std::vector<double>> &A, const std::vector<double> &x, double b, std::vector <double> &y)`
"""

```
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <fstream>


void daxpy (double a, const std::vector<double> &x, std::vector<double> &y) {
    // Handling for when x and y are different dimensions
    if (x.size() != y.size()) {
        std::cout << "Error: x and y are different dimensions" << std::endl;
        return;
    }

    for (int i = 0; i < x.size(); i++) {
        y[i] += a * x[i];
    }
}

int main() {
    const int ntrial = 1000;
    int n_min = 2;
    int n_max = 1024;

    long double micro_to_secondL = 1.e-6L;

    std::ofstream performanceCSV("performance.csv");
    performanceCSV << "n,elapsed_time,FLOPs" << std::endl;

    for (int n=n_min; n <= n_max; n++) {
        // Calculate the number of flops
        long double flop_count = static_cast<long double>(ntrial * (2 * n));

        // Generate random x, y, a
        std::vector<double> x(n), y(n);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(0,1.0); // range [0,1]
        double a = dist(gen);
        for (int i = 0; i < n; ++i) {
            x[i] = dist(gen); // generate random number and assign to vector element index i
            y[i] = dist(gen);
        }

        // Start timer
        auto start = std::chrono::high_resolution_clock::now();

        // Code segment to time
        for (int j=0; j < ntrial; j++) {
            // Run daxpy function
            daxpy(a, x, y);
        }

        // Stop timer
        auto stop = std::chrono::high_resolution_clock::now();
        // Calculate the duration of the code segment
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        // Convert to long double
        long double durationL = static_cast<long double>(duration.count());

        // Output the duration to the console
        std::cout << "Total duration of daxpy runs (n=" << n << "): " << durationL << " microseconds" << std::endl;

        long double flops = flop_count / (durationL * micro_to_secondL);

        // Output the flops value to the console
        std::cout << "daxpy performance (MFLOPs): " << flops / (1.e6L) << std::endl;

        // Write output to CSV file
        performanceCSV << n << "," << durationL << "," << flops << std::endl;
    }

    // Close the CSV file
    performanceCSV.close();

    return 0;
}
```