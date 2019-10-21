#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

const std::string matrix_path = "temp/matrix.txt";

double *gauss(int mode, bool not_silent = true);

double **matrix_string_conv(double **matrix, int i, int j, int string);

void print_matrix(double **matrix, int a);

double calculate_root(double **matrix, int string, double *var, int dimension);

double** matrix_set_top_max_of_column(double** matrix, int column, int dimension);
