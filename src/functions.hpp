#include <cmath>
#include <iostream>

#define E 2.7182818
#define PI 3.141592654

const int n_func = 5; // Amount of functions

typedef double(*func_ptr_t)(double);

struct func_struct{
    func_ptr_t f;
    double period = 0.0;
    char text[50];
};

void initialize_func_arr(func_struct *func_arr);

double f0(double x);
double f1(double x);
double f2(double x);
double f3(double x);
double f4(double x);
