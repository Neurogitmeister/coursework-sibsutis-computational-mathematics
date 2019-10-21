#include "functions.hpp"
#include "gauss.h"

#include <vector>
#include <cmath>
#include <iostream>

#define LINE "\n================================================\n"
#define THIN_LINE "_________________________________________________"

using namespace std;


class myPoint // Point object that stores two connected values in 'Real' number form
{
    public:
        double X;
        double Y;
        // Constructor and methods. Extended in '~.cpp' file
        myPoint() : X(0), Y(0) {};
        myPoint(double x, double y) : X(x), Y(y) {}; 
        void set(double x, double y) {
            X = x; Y = y;
        };
        // Destructor. Extended in '~.cpp' file
        ~myPoint(){}; 
};

class PointsArray{
    public:
        vector<myPoint> p_arr;
        unsigned a, b;
    PointsArray() : a(0), b(19) {
        for(unsigned i = 0; i < 20; i++) {
            p_arr.push_back(myPoint());
        }
    };
    PointsArray(unsigned count) : a(0), b(count - 1) {
        for(unsigned i = 0; i < count; i++) {
            p_arr.push_back(myPoint());
        }
    };
};

class Epsilon{
    public:
        const short int max_eps_pow = 8; // Max epsilon power
        const double eps_min = 0.5; // Min epsilon value
        double eps_max = 1;
        /* Max epsilon value 'eps_max' is calculated via constructor
        using 'max_eps_pow' */
        double value;
        short int set_value(double x) {
            value = x;
            if(value < eps_max)
            {
                value = eps_max;
                return 1;
            }
            if(value > eps_min)
            {
                value = eps_min;
                return 2;
            }
            return 0;
        };
        Epsilon() {
            for(int i = 0; i < max_eps_pow; i++)
                eps_max*= 0.1;
        };
        ~Epsilon() {};
};


// *Methods                     *Variables*                                                    *Defaults*
//                              |Full description in '~.cpp' file|                             |'interface' allows method to display|
//                              |                                |                             |it's actions, if set 'true'          |
double Lagrange                 (double x, vector<myPoint> p, int N, int start_index, int end_index,   bool interface = true);
double Eitken                   (double x, vector<myPoint> p, Epsilon Eps, int N,                      bool interface = true);
double Newton                   (double x, vector<myPoint> p, short int precission, int N, int n,      bool interface = true);
double cubicSpline              (double x, vector<myPoint> p, short int precission, int N, int n,      bool interface = true);
int Trigonometric    (double *y, double x, vector<myPoint> p, short int precission, int N, \
    double input_period, int n, bool interface = true, int sections = 2);

// *Helping methods*
double delta(int k, vector<myPoint> p, int n, bool interface = true);
int gauss2(double arr[20][20 + 1], double x[20], int n);
void reset_points(vector<myPoint> p, int N);
int set_points(vector<myPoint> *p, int N, double x, double h, func_ptr_t f, bool fun_flag = true);
int randomize_points(vector<myPoint> *p, int N, double *h, double h_min, double h_max, \
    func_struct func_curr, int *iter);

// *UI*
int input_int(int *x);