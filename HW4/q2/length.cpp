#include <thread>

/* 
Length of a graph of f(x) on [a,b] is $L = \int_a^b \sqrt{1 + f'(x)^2} dx$. Let f(x) = ln(x) - 1/8 x^2 on [1,6].
find the length of f on the interval analytically. You will need this to calculate the error in your code.
Write a C++ code parallelized using the C++ threads library that numerically evaluates the length of
this function on the interval. Your code should accept the number of points used to partition the interval
and the number of threads to spawn. Plot the strong scaling efficiency (time versus thread count) of your
code for n = 1.e8 partition points for 1 to 6 threads (or more). Plot the numerical error for n = 10 to
n = 1.e6 partition points, increasing by factors of 10 each time. Submit your code and plots.
*/


int main(int argc, char *argv[]) {
	int n_min = 10;
	int n_max = 1.e8;
	
	
}
