# HW2 README

## Problems 1-4
Files for problems 1, 2, 3, and 4 are located in the `q1-4` directory. Functions for `daxpy`, `daxpy_unroll`, 
`dgemv`, and `dgemm` are location in the file `blas.hpp`. Each problem has a corresponding function in the file 
`q1.cpp`, `q2.cpp`, `q3.cpp`, and `q4.cpp` respectively. A CSV file is created by each file named

### Level 1 BLAS
To compile and run the executable file for the Level 1 BLAS, run the following commands in the terminal:
```
g++ -std=c++14 -o q1.o q1.cpp
./q1.o
```
Note: A plot exists for the result of the above commands along with a version run with the `-O1` optimization flag.
These are plots directory named `q1-plot.png` and `q1-plot-O1.png`. A similar notation exists for the remaining 
problems.

### Level 2 BLAS
To compile and run the executable file for the Level 2 BLAS, run the following commands in the terminal:
```
g++ -std=c++14 -o q2.o q2.cpp
./q2.o
```

### Level 3 BLAS
To compile and run the executable file for the Level 3 BLAS, run the following commands in the terminal:
```
g++ -std=c++14 -o q3.o q3.cpp
./q3.o
```

### Level 4 BLAS
To compile and run the executable file for the Level 4 BLAS, run the following commands in the terminal:
```
g++ -std=c++14 -o q4.o q4.cpp
./q4.o
```
Note: This plot was generated with the `-O2` optimization flag to speed up the execution time.



## Problems 5-6
Files for problems 5 and 6 are located in the `q5-6` directory. This includes the main source file 
`q5.cpp` as well as `.cpp` and `.hpp` files for functions `axpy`, `gemv`, and `gemm`.
 
 ### Compiling/Running Problem 5
 To compile and run the executable file that tests the performance of the Level 1, 2, and 3 BLAS 
 template functions, run the following commands in the terminal:
 ```
g++ -std=c++14 -o q5.o q5.cpp axpy.cpp gemv.cpp gemm.cpp
./q5.o
```

### Compiling/Running Problem 6
The `q6.cpp` file contains the main function that tests the functions outlined in the header file
`refBLAS.hpp`. To compile and run this file, we can use the following commands in the terminal:
```
g++ -std=c++14 -o q6.o q6.cpp axpy.cpp gemv.cpp gemm.cpp 
./q6.o
```

For this next step, make sure to append the working directory to the `LD_LIBRARY_PATH` environment 
variable (ie: `export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}`).
Now, to build the shared object file, and compile `q6.cpp` into an executable `xq6` using the shared 
object file, we run the bash script `q6.cmpl` from the q5-6 directory which executes the following 
commands:
```
rm *.o x* *.so;
date;
g++ -std=c++14 -c -fPIC axpy.cpp;
g++ -std=c++14 -c -fPIC gemv.cpp;
g++ -std=c++14 -c -fPIC gemm.cpp;

export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH};
g++ -std=c++14 -shared -o librefBLAS.so axpy.o gemv.o gemm.o;
echo "Built shared object library librefBLAS.so";
g++ -std=c++14 -o xq6 -I. q6.cpp -L. -lrefBLAS
echo "Built program binary xq6 using shared object library";
ls -lstr xq6;
```

To run the executable `xq6`, finally we can use the command:
```
./xq6
```

