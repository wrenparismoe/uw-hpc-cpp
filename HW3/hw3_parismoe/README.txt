# Homework 3 - AMATH 583

## Problems 1-2
First, change into the directory with the files pertaining to problems 1 and 2 with `cd q1-2`.

### Problem 1
To compile and run the main source file test code for problem 1, run the following commands in the 
shell:
```
g++ -std=c++14 q1-test.cpp mm_{ijk,ikj,jki,jik,kij,kji}.cpp -o q1.o
./q1.o
``

### Problem 2
To compile and run the main source file test code for problem 2 with no optimizations, run the 
following commands in the shell:
```
g++ -std=c++14 q2-test.cpp mm_*.cpp -o q2.o
./q2.o
``

Subplots for the test results were placed in the `plots` directory. Results for tests run with 
double precision are provided as subplots within a single file called `q2-plot-double.png`. 
Several optimization flag tests were run for for loop permutations {jki} and {kij} of the gemm BLAS 
algorithm. The three optimizations tested along with the default case were `-O3`, `-ffast-math`, and 
`-O3 -march=native`. To test these optimizations with double precision, the following commands were 
run in the shell:
```
g++ -std=c++14 -O3 q2-test.cpp mm_*.cpp -o q2-O3.o
./q2-O3.o
g++ -std=c++14 -ffast-math q2-test.cpp mm_*.cpp -o q2-ffast-math.o
./q2-ffast-math.o
g++ -std=c++14 -O3 -march=native q2-test.cpp mm_*.cpp -o q2-O3-native.o
./q2-O3-native.o
```

To run the tests with single precision float data types, run the following commands in the shell:
```
g++ -std=c++14 q2-test-float.cpp mm_*.cpp -o q2-float.o
./q2-float.o
g++ -std=c++14 -O3 q2-test-float.cpp mm_*.cpp -o q2-float-O3.o
./q2-float-O3.o
g++ -std=c++14 -ffast-math q2-test-float.cpp mm_*.cpp -o q2-float-ffast-math.o
./q2-float-ffast-math.o
g++ -std=c++14 -O3 -march=native q2-test-float.cpp mm_*.cpp -o q2-float-O3-native.o
./q2-float-O3-native.o
```
The plots for these tests are located in the `plots` directory in a file named `q2-plot-float.png`.


## Problem 3
First, cd into the `q3` directory to reach the files included for problem 3. To compile and run the 
program, execute the following commands in the shell:
```
g++ -std=c++14 -O3 -ffast-math -o q3.o q3.cpp strassen_mm.cpp
./q3.o
```
Two tests were done with the Strassen algorithm, though both run with `-O3 -ffast-math` flags. The 
first test was done with the default base case of n=1 for only n powers of 2 (ie: n=2,4,8,16,32,...). 
The plot for this test is located in the `plots` directory in a file named `q3-plot-O3-base1.png`.

The second test was done by changing it to run for all n values from n=2 to n=1024 with a base case
of n<=32. When the recursive algorithm hits the base case, regular matrix multiplication is used. The
plot for this test is located in the `plots` directory in a file named `q3-plot-O3-allN-base32.png`. 