#include <matplot/matplot.h>
#include <set>

int main() {
    // Read data from the CSV file, headers are n,time_elapsed,FLOPs
    auto data = matplot::csvread("performance_data.csv");
}