#include "gauss.h"

using namespace std;

double *gauss(int mode, bool not_silent)
{
	// Attaching the ifstream to .txt file
	ifstream textfile;
	textfile.open(matrix_path);
	int dimension = 0;
	// Reading first line as dimension value
	textfile >> dimension;
	// Initializing dynamic matrix array
	double **matrix = new double*[dimension];
	for (int i = 0; i < dimension; i++) matrix[i] = new double[dimension+1];
	// Initializing dynamic roots array
	double *roots = new double[dimension];
	for (int i = 0; i < dimension; i++) roots[i] = 0;

	// Launching the method
	if(not_silent)
	{
		cout << "\ndim = " << dimension << endl;
		if(mode == 1)
			cout << "\nLAES 'Gauss' method" << endl;
		else if(mode == 2) 
			cout << "\nLAES 'Modified Gauss' method" << endl;
		if (mode != 1 && mode != 2) return NULL;
		cout << "Source matrix:\n\n";
	}
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j <= dimension; j++)
		{
			// Reading separated words as values for matrix
			textfile >> matrix[i][j];
		}		
	}

	if(not_silent)
	{
		for(int i = 0; i < dimension; i++)
		{
			cout << "|";
			for(int j = 0; j <= dimension; j++)
			{				
			cout << fixed;
			cout.width(10);
			cout.precision(5);			
			cout << matrix[i][j] << " ";
			}
			cout << "|" << endl;
		}
	cout << endl << endl;
	}
	// Forward
	if (mode == 1)
	{
		for (int i = 0; i < dimension - 1; i++)
		{
			matrix = matrix_string_conv(matrix, dimension, dimension + 1, i);
		}
		if(not_silent)
		{
			cout << "Transformed matrix:\n\n";
			print_matrix(matrix, dimension);
		}
	}
	if (mode == 2)
	{
		for (int i = 0; i < dimension - 1; i++)
		{
			matrix = matrix_set_top_max_of_column(matrix, i, dimension);
			matrix = matrix_string_conv(matrix, dimension, dimension + 1, i);
		}
		if(not_silent)
		{
			cout << "Transformed matrix:\n\n";
			print_matrix(matrix, dimension);
		}
	}
	// Reverse
	for (int i = dimension - 1; i >= 0; i--)
	{
		roots[i] = calculate_root(matrix, i, roots, dimension);
	}
	
	if(not_silent)
	{
		for (int i = 0; i < dimension; i++)
			cout << "root[" << i << "] = " << roots[i] << endl;
		cout << "\nMethod success!\n";
	}
		
	return roots;
}

double **matrix_string_conv(double **matrix, int i, int j, int str)
{
	if (matrix[str][str] == 0.0)
	{
		if (i == str + 1)
		{
			cout << "No roots." << endl << endl;
			return NULL;
		}
		double *temparr = new double[j]; 
		int x = str;
		while (matrix[x][str] == 0.0)
		{
			x++;			
		}
		for (int a = 0; a < j; a++)
		{
			temparr[a] = matrix[x][a];
			matrix[x][a] = matrix[str][a];
			matrix[str][a] = temparr[a];
		}
		
		delete temparr;
	}

	double *num = new double[i]; 
	for (int a = str; a < i - 1; a++)
	{
		num[a] = -1.0*(matrix[a + 1][str] / matrix[str][str]);
	}


	for (int a = str; a < i - 1; a++)
	{
		for (int b = str; b < j; b++)
		{
			matrix[a + 1][b] = matrix[a + 1][b] + matrix[str][b] * num[a];
		}

	}
	delete num;
	return matrix;
}


void print_matrix(double **matrix, int a)
{
	for (int i = 0; i < a; i++)
	{
		cout << "|";
		for (int j = 0; j <= a; j++)
		{
			cout << fixed;
			cout.precision(5);
			cout.width(10);
			cout << matrix[i][j] << " ";
		}
		cout << "|" << endl;
	}
	cout << endl;
}

double calculate_root(double **matrix, int string, double *root, int dimension)
{
	double unknowroot = 0.0, sum = matrix[string][dimension];
	for (int i = string, a = string + 1; (i != dimension - 1 && a != dimension); i++, a++)
	{
		sum = sum - matrix[string][i + 1] * root[a];
	}
	unknowroot = sum / matrix[string][string];
	return unknowroot;
}

double** matrix_set_top_max_of_column(double** matrix, int column, int dimension)
{

	int i;
	int max_index = 0;
	int max_val = abs(matrix[column][column]);
	for (int a = column + 1; a < dimension; a++)
	{
		if (abs(matrix[a][column]) > max_val)
		{
			max_val = abs(matrix[a][column]);
			max_index = a;
		}
	}
	i = max_index;
	double *temparr = new double[dimension + 1];
	
	while (i > column)
	{	
		for (int j = 0; j < dimension + 1; j++)
		{
			temparr[j] = matrix[i][j];
			matrix[i][j] = matrix[i - 1][j];
			matrix[i - 1][j] = temparr[j];
		}
		i--;
	}
	delete temparr;

	return matrix;
}
