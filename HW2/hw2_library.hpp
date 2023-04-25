#ifndef HW2_LIBRARY_HPP
#define HW2_LIBRARY_HPP

#include "matrix.hpp"
#include "dgemv.hpp"

/* (compiler.cmpl)
// compile shared library file libmylibrary.so by:
g++ -std=c++14 -c -fPIC matrix.cpp ; // repeat with other .cpp files to include (1 file per fn/class)
...
g++ -shared -o libmylibrary.so matrix.o ...
echo "built shared object library";
ls -lstr libmylibrary.so;

// then built program binary that uses shared object library by:
g++ -std=c++14 -o file.o -I. file.cpp -L. -lmylibrary
echo "built program binary";
ls -lstr file.o;
*/

/* (compile time)
source compiler.cmpl
*/


#endif // HW2_LIBRARY_HPP
