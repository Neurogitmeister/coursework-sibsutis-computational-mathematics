#include "interpolationMethods.hpp"

/*************************
 * Interpolation methods *
 *************************/

double Lagrange(double x, vector<myPoint> p, int N, int start_index, int end_index, bool interface)
{
    double x_part;
    double value = 0.0;
    if(interface){
        cout << LINE << "Lagrange's method:   \\ From p["<< start_index << "] to p[" << end_index << "] /" << LINE;
    }
    if(start_index == N || end_index == N || start_index < 0 || end_index < 0)
    {
        if(interface){
            cout << "Error: index out of range!\n";
        }
        return 1;
    }
    else if (end_index - start_index < 0)
    {
        if(interface){
            char key[1];
            cout << "Error: negative interval!\nDo you want to switch? [Y/n]\n";
            cin >> key;
            if (*key == 'n')
                return 2;
        } 
        else return 2;
    }
    else if (end_index - start_index < 2)
    {
        if(interface){
            cout << "Error: interpolation with one point is not a thing!\n";
        }
        return 3;
    }
    if(interface){
        cout << "f(";
        cout << fixed;
        cout << x << ") = ";
        cout << defaultfloat;
    }
    for (int i = start_index; i <= end_index; i++)
    {
        x_part = 1.0;
        for (int j = start_index; j <= end_index; j++)
        {
            if(!(p[i].X == p[j].X))
                x_part *= (x - p[j].X)/(p[i].X - p[j].X);
        }
        value += p[i].Y * x_part;
        if(interface){
            cout << "(" << p[i].Y << "*" << x_part << ") = " << value << ";\n+ ";
        }
    }
    if(interface){
        cout << LINE << endl;
    }
    return value;
}

double Eitken(double x, vector<myPoint> p, Epsilon Eps, int N, bool interface)
{
    double polinom_n[N];
    double polinom_n_plus_1[N];
    int iter = 1; // Iterations increasing counter default
    int size = 1; // Size of polinom(n+1) decreasing counter default
    int offset = 0;
    int distance = 0;
    double tempval;
	while (p[offset].X <= x)
		offset++;
	offset--;
    if(interface){
        cout << LINE << "Eitken's method: \\ From p[" << offset << "] < f(";
        cout << fixed;
        cout.width(Eps.max_eps_pow + 5);
        cout << x << ") /" << LINE << endl;
        cout << defaultfloat;
    }
    do
    {
        for(int i = 0; i < iter + 1; i++)
        {
            polinom_n[i] = p[offset+i].Y; // Reset polinom(n)'s values for next iteration
        }
        if(interface){
            cout << "\nIteration = " << iter << ":\n\n";
        }
        for (int j = 0; j < iter; j++)
        {
            for (int i = 0; i < size; i++) 
            {
                distance = iter-size+1;
                polinom_n_plus_1[i] = ( polinom_n[i] * (p[offset+i+distance].X - x) - polinom_n[i+1] * (p[offset+i].X - x) ) \
                / (p[offset+i+distance].X - p[offset+i].X);
            }
            if(interface){
                cout << "P" << j + 1 << "(";
                cout << fixed;
                cout << x << "): ";
                cout << defaultfloat;
            }
            for (int i = 0; i < size; i++)
            {
                polinom_n[i] = polinom_n_plus_1[i];
            }
            if(interface)
            {
                for (int i = 0; i < size; i++)
                {
                    cout << polinom_n[i] << " ";
                }
            cout << "\n\n";
            }

            size--;
        }
        size = ++iter; // Restore size value from last iter's value and increase iter counter
        if (fabs(tempval - polinom_n_plus_1[0]) <= Eps.value)
        {
            if(interface){
                cout << LINE << endl;
            }
            return polinom_n_plus_1[0];
        }
        tempval = polinom_n_plus_1[0];
    } while(iter + offset != N);
    if(interface){
        cout << LINE << endl;
    }
    return polinom_n_plus_1[0];
}

double Newton(double x, vector<myPoint> p, short int precission, int N, int n, bool interface)
{
    int offset = 0;
    double sum;
    double h, denom;
    double subtract = 0;
	while (p[offset].X <= x)
		offset++;
	offset--;
    if(interface){
        cout << LINE << "Newton's method: \\ From p[" << offset << "] < f(";
        cout << fixed;
        cout.width(precission+5);
        cout << x << ") /" << LINE << endl;
        cout << defaultfloat;
    }
    sum = p[offset].Y; // a = 0, b = 19
    h = p[offset+1].X - p[offset].X;

    // Delta of power n requires n+1 points ahead. So for amount of points ahead that is equal to 'N - offset' we can only have
    // 'N - offset - 1' iterations
 
    for(int i = 0; i < min(N - offset - 1, n); i++)
    {
        // Function delta uses : (starting point index, myPoint object pointer, power of delta)
        subtract = delta(offset, p, i+1, false);
        if(interface){
            cout << "\nIteration " << i << ":\n";
            cout << "Partitial subtraction = " << subtract << "\n\n";
        }
        if(abs(subtract) <= pow(0.1,precission)) // Check for delta < epsilon
            break;
        for(int j = 0; j <= i; j++)
        {                     
            denom = (j + 1) * h; // Short for denominator. Contains j! and h^j
            //cout << "denom = " << denom << endl;
            subtract /= denom; 
            subtract *= (x - p[j+offset].X); // (x - X[k])(x - X[k+1])*...*(x - X[n-1])
            //cout << "prod = " << subtract << endl;
        }
        sum += subtract;
        if(interface){
            cout << "P[" << i << "](" << x << ") = " << sum << "\n\n";
        }
        
    }
    return sum;
}
// max_pow+5
double cubicSpline(double x, vector<myPoint> p, short int precission, int N, int n, bool interface)
{
    if(n < 3)
    {
        if(interface){
            cout << LINE << "Cubic spline method: \\ error /" << LINE << endl;
            cout << "Error! Less than 3 points!\n";
        }
        return 0.0;
    }
    // Searching index 'offset' just by right of 'x' 
    int offset = 0;
    while(offset != N)
    {
        if(p[offset].X > x)
            break;
        offset++;
    }

    // Check for 'x' presence
    if(offset == N || offset == 0)
    {
        if(interface){
            cout << LINE << "Cubic spline method: \\ error /" << LINE << endl;
            cout << "Error! 'x' not found in sector x[0] x[N]!\n";
        }
        return 0.0;
    } 

    offset -= n/2 + 1 ; // Setting 'x' to be the middle/middle-left of index sector of size 'n'
    if(offset + n > N - 1)
        offset = N - 1 - n; // Setting to max if overflow
    else if(offset < 0)
        offset = 0; // Setting to min if underflow

    
    // Searching the sector number of the momentum's array starting from 1
    int num = 1;
    while(p[num+offset].X < x)
    {
        num++;
    }

    // 'h' - array of x[i+1] - x[i] distances
    double h[n - 1];
    for (int i = 0; i < n - 1; i++)
    {
        h[i] = p[i+1+offset].X - p[i+offset].X;
    }

    // Making the header for method
    if(interface){
        cout << LINE << "Cubic spline method: \\ From p[" << offset << "] < f(";
        cout << fixed;
        cout.width(precission+5);
        cout << x << ") /" << LINE << endl;
        cout << "\noffset = " << offset << ", num = " << num << endl;
        cout << "h[] = ";
        for (int i = 0; i < n - 1; i++)
        {
            cout << h[i] << " ";
        }
        cout << defaultfloat;
        cout << "\n\n";
    }

    // Initializing and filling out matrix 'С'
    double C[n - 2][n - 2];

	for (int i = 0; i < n - 2; i++)
	{
		for (int j = 0; j < n - 2; j++)
		{
            if (i == j)
				C[i][j] = ( h[i] + h[i + 1] ) / 3.0;
            else if (j == (i - 1))
				C[i][j] = h[i] / 6.0;
            else if (j == (i + 1))
                C[i][j] = h[i + 1] / 6.0;
            else 
                C[i][j] = 0.0;
		}
	}

	// Making vercotr 'd'
    double d[n - 2] = { 0.0 };
	for (int i = 1; i < n - 1; i++)
	{
		d[i-1] = ((p[i + offset + 1].Y - p[i + offset].Y) / h[i]) - ((p[i + offset].Y - p[i + offset - 1].Y) / h[i-1]);
	}

	// Concatination of matrix 'C and vector 'd'
	double Cd[n][n + 1] = { 0.0 };
    ofstream matrixFile;
    matrixFile.open (matrix_path);
    matrixFile << n - 2 <<"\n";
	for (int i = 0; i < n - 2; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (j == n - 2)
				Cd[i][j] = d[i];
			else
				Cd[i][j] = C[i][j];
            // Writing matrix results to .txt file
            matrixFile << Cd[i][j] << " ";
		}
        matrixFile <<"\n";
	}
    matrixFile.close();

    double *tmp4M = (double*)malloc(sizeof(double)*(n - 2));
    
    // Choosing LAES solving method
    int mode = 0;
    if(interface){
        cout << "Choose the LAES solving method:\n1 - Gauss; \n2 - Modified Gauss;\n";
        input_int(&mode);
        while(mode < 1 || mode > 2)
        {     
            cout <<  "Try one more time\n";                
            input_int(&mode);
        }
    }
    else mode = 2;
	tmp4M = gauss(mode, interface);
    
    // Initializing and filling out Momentums array 'M'
    double M[n];
    M[0] = 0.0;
    M[n-1] = 0.0;
	for (int i = 1; i < n - 1; i++)
	{
		M[i] = tmp4M[i - 1];
	}
    if(interface){

        // Printing 'M' array
        for (int i = 0; i < n; i++)
        {
            printf("M(%d) = %lf\n", i, M[i]);
        }
        // Printing calculation debug info
        cout << "x[i] = x["<< offset+num << "] = "; cout.width(12); cout << p[offset+num].X;
        cout << "\t\tx[i-1] = x["<< offset+num-1<< "] = "; cout.width(12); cout << p[offset+num-1].X  << endl;
        cout << "y[i] = y["<< offset+num << "] = "; cout.width(12); cout << p[offset+num].Y;
        cout << "\t\ty[i-1] = y["<< offset+num-1<< "] = "; cout.width(12); cout << p[offset+num-1].Y << endl;
        cout << "M[i] = M["<< num << "] = "; cout.width(12); cout << M[num];
        cout << "\t\tM[i-1] = M["<<num-1<< "] = "; cout.width(12); cout << M[num-1] << endl;
    }

    // Calculating splined polinomial:
	double S = M[num-1] * pow((p[offset+num].X - x), 3.0) / (6.0 * h[num-1]);
	S += M[num] * pow((x - p[offset+num-1].X), 3.0) / (6 * h[num-1]);
	S += (p[offset+num-1].Y - M[num-1] * pow(h[num], 2.0) / 6.0) * (p[offset+num].X - x) / h[num-1];
	S += (p[offset+num].Y - M[num] * pow(h[num-1], 2.0) / 6.0) * (x - p[offset+num-1].X) / h[num-1];
    if(interface){
	    printf("\nx = %lf \t S = %lf\n", x, S);
    }
	return S;
}

int Trigonometric(double *y,double x, vector<myPoint> p, short int precission, int N, double input_period, int n, bool interface, int sections)
{
   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
    * Function that interpolates 'x' from given array of equidistant points using trigonometric polinomial.
    * 
    * args: x - 'x' axis coordinate of the requested point with unknown 'y',
    *       **p - pointer to an array of points,
    *       h - distance between points on 'x' axis,
    *       n - polinomial degree.
    * 
    * Trigonometric polinomial: Pn(x) = a[0]/2 + SUM[l=0..2n][a[l]*cos(l*x) + b[l]*sin(l*x)]
    *
    * where a[l] = 2 / (2n+1) * SUM[k=0..2n][f(t[k])*cos(l*t[k])], including a0, 
    *       b[l] = 2 / (2n+1) * SUM[k=1..2n][f(t[k])*sin(l*t[k])], excluding b0,
    *       l = 0,1,...,2n,
    * 
    *       where t[k] is a grid of equidistant points t[k] = 2*PI / (2*n+1) * k, k = 0,1,...,2n;
    *       and t[0] = 0, t[2n] = 2*PI * 2n/(2n+1), t[2n+1] = 2*PI = T;
    * 
    *       Total: 2n+1 coefficients.
    * 
    * In a case with dynamic points interval we end up with different t[0] = p[0].X and t[2n] = p[2n].X, but their f(t[k])
    * is already known and located in p[k].Y. Each 'x' between p[0] and p[2n] only needs to be moved
    * to the [0.. 2PI] section to get the value from transformed function. The exact method automatically creates
    * a transformed function on [0..2PI] from the p[k].Y values.
    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


   // Check for 'x' presence
    if(p[0].X > x || p[N-1].X < x)
    {
        if(interface){
            cout << LINE << "Trigonometric method: \\ error /" << LINE << endl;
            cout << "Error! 'x' not found in sector x[0] x[N]!\n";
        }
        return 1;
    }

    /* Setting 'offset' index just left of 'x'.
    * 
    * Offset step is controlled by functions input variable 'sections', which sets the amount of sections
    * per one calculated function.
    */
    int offset = 0;
    while(offset <= N)
    {
        if(p[offset].X > x)
            break;      
        offset += sections;
    }
    offset -= sections;
    
     if(interface){ cout << "sections = " << sections << " offset = " << offset << endl;}
    // Calculating the amount of points
    double p_amt = n * 2 + 1;
    if(p_amt > N - 1)
    {
        if(interface){
            cout << LINE << "Trigonometric method: \\ error /" << LINE << endl;
            cout << "Error! Points needed is more than is available!\n";
        }
        return 2;
    }
    // Calculating left offset shift
        offset -= floor((p_amt - sections) / 2.0);
    
    // Checking min-max offset
    if(offset + p_amt > N - 1)
        offset = N - 1 - p_amt; // Setting to max if overflow
    else if(offset < 0)
        offset = 0; // Setting to min if underflow

    // Transforming 'x' to fit [0..2*PI]
    x = (x - p[offset].X) * 2.0 * PI / input_period;

    if(interface){
        // Making the header for method using offset
        cout << LINE << "Trigonometric method: \\ From p[" << offset << "] < f(";
        cout << fixed;
        cout.width(precission+5);
        cout << x << ") /" << LINE << endl;
        cout << defaultfloat;
        cout << "\noffset - shift = " << offset << endl; //<< ", num = " << num << endl;
        cout << "x transformed = " << x << endl;
    }

    // Period
	double period = 2.0 * PI / p_amt;

    // Array for coefficients
	double a[n] = { 0.0 };
    double b[n] = { 0.0 };

    // Finding all the coefficients by summing formula
	for (int l = 0; l <= n; l++)
	{
        for (int k = 0; k <= 2 * n; k++)
        {
            a[l] += p[offset+k].Y * cos(l * period * k);
            b[l] += p[offset+k].Y * sin(l * period * k);
        }
		a[l] *= 2.0 / p_amt;    
        b[l] *= 2.0 / p_amt;      
	}
    if(interface){
        for (int l = 0; l <= n; l++)
        {
            cout << " a[" << l << "]=" << a[l];
            cout << " b[" << l << "]=" << b[l] << endl;
        }
    }

    // Result
	*y = a[0] / 2;
	for (int l = 1; l <= n; l++)
	{
		*y += a[l]*cos(l*x) + b[l]*sin(l*x);
    }

	return 0;
}

/*******************
 * Extra functions *
 *******************/

double delta(int k, vector<myPoint> p, int n, bool interface)
{
    if (n <= 0)
    {
        if(interface)
            cout << "Error: power less than 1!" << endl;
        return 1;
    }
    if (n == 1)
        return p[k+1].Y - p[k].Y;
    return delta(k+1, p, n-1) - delta(k, p, n-1);
}

int gauss2(double arr[20][20 + 1], double x[20], int n)
{
	// Forward cycle
    int N = 20;
	for (int k = 0; k < N; k++)
	{
		for (int i = k + 1; i < N; i++)
		{
			double tmp = (arr[i][k] / arr[k][k]);
			for (int j = k; j < N + 1; j++)
			{
				arr[i][j] -= arr[k][j] * tmp;
			}
		}
	}

	// Backward cycle
	int count = N - 1;
	for (int i = N - 1; i >= 0; i--)
	{
		x[i] = arr[i][N] / arr[i][i];
		if (count != N - 1)
			for (int j = N - 1; j > count; j--)
			{
				x[i] -= arr[i][j] * x[j] / arr[i][i];
			}
		count--;
	}
	return 0;
}

int set_points(vector<myPoint> *p, int N, double x0, double h, func_ptr_t f, bool fun_flag)
{
    if(h == 0.0)
    {
        return 1;
    }
    for (int i = 0; i < N; i++)
    {
        if (f(x0) != f(x0))
        {
            for (int j = 0; j < N; j++)
            {
                p->at(j).X = p->at(j).Y = 0;
            }
            return 2;
        }
        p->at(i).set(x0 ,f(x0)); //+ pow(-1,i)*0.75);
        x0 += h;
    }
    return 0;
}

int randomize_points(vector<myPoint> *p, int N, double *h, double h_min, double h_max, func_struct func_curr, int *iter)
{
    int exception = 1;
    while (exception != 0)
    {
        *iter += 1;
        if(func_curr.period != 0.0){ // Using custom randomization for periodic functions
            *h = (double)(rand()%2 + 1) / (rand()%8 + 1 ) * func_curr.period;
            p->at(0).X = (rand()%201 - 100) * (func_curr.period / 4.0); 
        } else {
            *h = (rand()%1000+1) * (-h_min) + h_max;
            p->at(0).X = (double)(rand()%3400 - 1700 ) / 17.0;
        }
        if(*h == 0.0) return 2;
        exception = set_points(p, N, p->at(0).X, *h, func_curr.f, false);
        if (*iter == 100) return 1; // Takes 100 tries to change the function
    } 
    return 0;
}

void reset_points(vector<myPoint> p, int N)
{
    for (int i = 0; i < N; i++)
    {
        p[i].set(0,0);
    }
}

int input_int(int *x)
{
    if(scanf("%d", &*x) != 1)
    {
		string buf;
        cout <<  "So smart of you!";
        getline(cin, buf);
        getline(cin, buf);
        return 1;
    }
    return 0;
}

