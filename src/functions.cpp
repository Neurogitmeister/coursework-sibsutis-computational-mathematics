#include "functions.hpp"

double f0(double x)
{
    return 3.0*x*x*x - x*x;
}

double f1(double x)
{
    return sqrt(x);
}

double f2(double x)
{
    return pow(E,40.0/(x + 20.0)) * sin(x);
}

double f3(double x)
{
    return pow(E, sin(x) + cos(x));
}

double f4(double x)
{
    return 5.0*cos(2.0*x);
}

void initialize_func_arr(func_struct *func_arr)
{ // ⁰ ¹ ² ³ ⁴ ⁵ ⁶ ⁷ ⁸ ⁹  ½ ⅓ ⅔ ⅕ ⅖ ⅗ ¼ ¾ ⅘ ⅙ ⅚ ⅛ ⅜ ⅝ ⅞");
    func_arr[0].f = &f0;
    func_arr[1].f = &f1;
    func_arr[2].f = &f2;
    func_arr[2].period = 2.0*PI;
    func_arr[3].f = &f3;
    func_arr[3].period = 2.0*PI;
    func_arr[4].f = &f4;
    func_arr[4].period = PI;

    sprintf(func_arr[0].text, "f(x) = 3x³ - x²");
    sprintf(func_arr[1].text, "f(x) = x½");
    sprintf(func_arr[2].text, "f(x) = exp( 40/(10 + x)) * sin(x)");
    sprintf(func_arr[3].text, "f(x) = exp(sin(x) + cos(x))");
    sprintf(func_arr[4].text, "f(x) = 5cos(2x)");
}