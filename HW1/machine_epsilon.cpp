#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int main()
{
    float a = 1.0f, b = 1.0f / pow(2, 23);
    int j = 0;
    while ((1 - (a + b)) != 0)
    {
        j++;
        b = b / 2;
    }
    cout << "Single-Precision (SP) Precision: " << setprecision(10) << b << endl;

    double c = 1.0, d = 1.0 / pow(2, 52);
    j = 0;
    while ((1 - (c + d)) != 0)
    {
        j++;
        d = d / 2;
    }
    cout << "Double-Precision (DP) Precision: " << setprecision(10) << d << endl;

    return 0;
}