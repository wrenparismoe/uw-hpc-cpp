//
// k.j. roche, k8r@uw.edu
// examples for AMATH483/583
// Lectures 3, 4
// compile from Terminal:
// g++ -o xlect4 -std=c++14 kr-hw.cpp
// exectute from Terminal:
// ./xlect4
//

#include <iostream>

void fnc(std::string a, std::string b = "hpc is cool")
{
    std::cout << a << " " << b << std::endl;
}

void changei(int &i)
{
    i += 10;
}

int main()
{
    std::cout << "Hello, world!" << std::endl;
    // integers
    std::cout << "int: " << sizeof(int) << std::endl;
    std::cout << "int8_t: " << sizeof(int8_t) << std::endl;
    std::cout << "int16_t: " << sizeof(int16_t) << std::endl;
    // demo of overflow ...
    int16_t i = 32767;
    std::cout << "int16_t i = " << i << std::endl;
    i++; // add one and print it again
    std::cout << "int16_t i++ : " << i << std::endl;

    // demo 2 of overflow ...
    unsigned int iu = 4294967295;
    std::cout << "int16_t i = " << iu << std::endl;
    iu++; // add one and print it again
    std::cout << "int16_t i++ : " << iu << std::endl;

    // demo of underflow ...
    int16_t ii = -32768;
    std::cout << "int16_t i = " << ii << std::endl;
    ii--; // add one and print it again
    std::cout << "int16_t i-- : " << ii << std::endl;

    std::cout << "int32_t: " << sizeof(int32_t) << std::endl;
    std::cout << "int64_t: " << sizeof(int64_t) << std::endl;
    std::cout << "long: " << sizeof(long int) << std::endl;
    std::cout << "long long: " << sizeof(long long int) << std::endl;
    std::cout << "unsigned int: " << sizeof(unsigned int) << std::endl;
    std::cout << "char: " << sizeof(char) << std::endl;
    std::cout << "float: " << sizeof(float) << std::endl;
    std::cout << "double: " << sizeof(double) << std::endl;

    // pointer demo
    int *p;
    int j = 17;
    p = &j;
    int **r = &p;
    std::cout << "j (j)	: " << j << " addr j (&j): " << &j << " addr j (p)	: " << p << std::endl;
    std::cout << "j (*p)	: " << *p << " addr p (&p): " << &p << " addr p (r)	: " << r << std::endl;
    std::cout << "j (**r)	: " << **r << " addr j (*r): " << *r << " addr r (&r)	: " << &r << std::endl;
    **r = 9; // change the value bc I can ...
    std::cout << "j (j)	: " << j << " j (*p)	: " << *p << " j (**r)	: " << **r << std::endl;

    // reference demo
    j = 7;
    int &ref = j;
    std::cout << "j (j)	: " << j << " addr j (&j): " << &j << " r (r)	: " << ref << " addr r (&r): " << &ref << std::endl;
    ref = -3;
    std::cout << "j (j)	: " << j << " r (r)	: " << ref << std::endl;

    // IF
    bool condition1 = false;
    if (condition1)
    {
        // do things if <condition1> is TRUE
        std::cout << "hello from COND1: " << std::endl;
    }
    else if (!condition1)
    {
        // do things if <condition1> is FALSE since ~FALSE = TRUE
        std::cout << "hello from NOT COND1: " << std::endl;
    }
    else
    {
        // this case cannot be base on my code, but is useful very often
    }

    // TERNARY
    //  (condition) ? (use if condition TRUE) : (use if condition FALSE)
    int w = (condition1) ? -1 : 1;
    std::cout << "w: " << w << " COND: " << condition1 << std::endl;
    w = (w > 0) ? 0 : 1;
    std::cout << "w: " << w << std::endl;

    switch (w)
    {
    case 0: // code block for when w=0 case ...
        std::cout << "switch case 0!" << std::endl;
        break; // this is significant
    case 1:    // code block for when w=1 case ..
        std::cout << "switch case 1!" << std::endl;
        break;
    default: // if none of the cases are true, do this
        std::cout << "switch case default!" << std::endl;
        break; // good practice ...
    }

    // WHILE LOOP
    int l = 0;
    while (l < 10)
    { // do stuff
        std::cout << l << " ";
        l++;
    }
    std::cout << std::endl;
    // l = ???
    std::cout << "after WHILE l = " << l << std::endl;

    // DO-WHILE LOOP
    l = 10;
    do
    { // do stuff
        std::cout << "in DO-WHILE: " << l << " ";
        l++;
    } while (l < 10);
    std::cout << std::endl;
    // l = ???
    std::cout << "after DO-WHILE l = " << l << std::endl;

    // FOR
    //  for (init cntr1, init cntr2; condition; increment cntrs){//do stuff}
    int k;
    for (l = 0, k = 4; k >= 0; l++, k--)
    {
        std::cout << "l: " << l << " k: " << k << std::endl;
    }
    std::cout << "AFTER FOR l: " << l << " k: " << k << std::endl;

    // FOR w/ CONTINUE
    for (k = 0; k < 4; k++)
    {
        if (k == 2)
            continue;
        std::cout << "k: " << k << std::endl;
    }
    std::cout << "AFTER FOR CONTINUE k: " << k << std::endl;

    // infinite loop
    for (;;)
    { // buyer beware! useful but ...
        std::cout << "inf-loop!";
        break; // edit for class discussion only
    }

    std::cout << std::endl;

    // infinite loop w/ break condition
    int loopbreak = 0;
    for (;;)
    { // buyer beware! useful but ...
        if (loopbreak == 4)
            break;
        std::cout << "break loop";
        // do stuff
        loopbreak++;
    }
    std::cout << "AFTER FOR loopbreak: " << loopbreak << std::endl;

    // iterating in containers
    int a[3] = {1, 2, 3};
    for (int &l : a) // iterator scope initializer
        std::cout << "a[]: " << l << std::endl;
    std::cout << "AFTER iterator l: " << l << std::endl;

    // FUNCTIONS ...
    fnc("Hi there");                     // default issued
    fnc("Hi there", " I want cookies!"); // default overridden

    int ix = 4;
    std::cout << "init ix: " << ix << std::endl;
    changei(ix);
    std::cout << "new ix: " << ix << std::endl;
}
