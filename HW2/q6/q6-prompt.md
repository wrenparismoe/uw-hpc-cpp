```
template <typename T>
void gemm (T a, const std::vector<std::vector<T>> &A,
            const std::vector<std::vector<T>> &B, T b,
            std::vector<std::vector<T>> &C) {

    int m = A.size();
    int n = B[0].size();
    int p = A[0].size();

    // C = aAB + bC
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            T sum = 0;
            for (int k = 0; k < p; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = a * sum + b * C[i][j];
        }
    }
}

// Explicit template instantiation for float and double
template void gemm<float>(float a, const std::vector<std::vector<float>> &A,
                          const std::vector<std::vector<float>> &B, float b,
                          std::vector<std::vector<float>> &C);
template void gemm<double>(double a, const std::vector<std::vector<double>> &A,
                           const std::vector<std::vector<double>> &B, double b,
                           std::vector<std::vector<double>> &C);
template void gemm<long double>(long double a, const std::vector<std::vector<long double>> &A,
                           const std::vector<std::vector<long double>> &B, long double b,
                           std::vector<std::vector<long double>> &C);
```
The above code is producing a "segmentation fault" error due to incorrect memory allocation occurring at some iteration of the loop after the line `sum += A[i][k] * B[k][j];` is run. Altering the matrix dimensions can change the error to "double free or corruption (!prev)" or "corrupted size vs. prev_size". Valgrind gives the following trace:
```
==27159== Invalid read of size 8
==27159==    at 0x10EB6A: std::vector<double, std::allocator<double> >::operator[](unsigned long) const (stl_vector.h:1143)
==27159==    by 0x10F370: void gemm<double>(double, std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > const&, std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > const&, double, std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >&) (gemm.cpp:48)
==27159==    by 0x10A52D: main (q6.cpp:72)
==27159==  Address 0x4ed1e80 is 0 bytes after a block of size 6,144 alloc'd
==27159==    at 0x4842003: operator new(unsigned long) (vg_replace_malloc.c:434)
==27159==    by 0x10DB96: std::__new_allocator<std::vector<double, std::allocator<double> > >::allocate(unsigned long, void const*) (new_allocator.h:137)
==27159==    by 0x10D64F: std::allocator_traits<std::allocator<std::vector<double, std::allocator<double> > > >::allocate(std::allocator<std::vector<double, std::allocator<double> > >&, unsigned long) (alloc_traits.h:464)
==27159==    by 0x10D2F1: std::_Vector_base<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >::_M_allocate(unsigned long) (stl_vector.h:378)
==27159==    by 0x10CA96: std::_Vector_base<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >::_M_create_storage(unsigned long) (stl_vector.h:395)
==27159==    by 0x10C158: std::_Vector_base<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >::_Vector_base(unsigned long, std::allocator<std::vector<double, std::allocator<double> > > const&) (stl_vector.h:332)
==27159==    by 0x10B614: std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >::vector(unsigned long, std::vector<double, std::allocator<double> > const&, std::allocator<std::vector<double, std::allocator<double> > > const&) (stl_vector.h:566)
==27159==    by 0x10AD1F: std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > generateRandomMatrix<double>(int, int) (q6.cpp:34)
==27159==    by 0x10A4AE: main (q6.cpp:69)
```
and GDB gives the following:
```
Program received signal SIGSEGV, Segmentation fault.
0x000055555555b2a7 in gemm<double> (a=0.30234653915087017, A=std::vector of length 256, capacity 256 = {...}, B=std::vector of length 128, capacity 128 = {...}, b=0.39934180410485409, C=std::vector of length 256, capacity 256 = {...}) at gemm.cpp:48
48                      sum += A[i][k] * B[k][j];
```
What changes to the code can be made to fix this error?