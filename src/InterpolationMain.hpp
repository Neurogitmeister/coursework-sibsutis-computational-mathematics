#include "dataClasses.hpp"
#include "interpolationMethods.hpp"
#include "makePlot.h"
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct termios term_save;


// Interpolation related Actions
int interpolate(Result *result, Buff *buff, PointsArray Pts, double x, Epsilon Eps, unsigned func_idx, keys key);
inline bool exists (const std::string& name);
int rewrite_data(string path);
int record_function_data(string path, PointsArray Pts, Epsilon Eps, keys key, func_struct func_curr, float h, unsigned int n = 0, unsigned int sect = 1);
int plot_record_data_menu(string *path, Buff *buff, PointsArray p, func_struct func_curr, Epsilon Eps, float plot_density, InterpFlags *Flags);
int arrange_plot(string data_path, string png_path, string func_name, double x_min, double x_max, double y_min, double y_max, short unsigned n_xtics, short unsigned n_ytics);
void extremum_from_points(vector<myPoint> p, unsigned a, unsigned b, double *y_max, double *y_min);

// Console right column graphics
void print_shroom(Buff *buff);
void print_one_millionth_chance_error(Buff *buff, int code);
void print_fun_error(Buff *buff);
void print_points(Buff *buff, vector<myPoint> p);
void print_points_ab(Buff *buff, PointsArray Pts);
void get_offset(Buff *buff, vector<myPoint> p, double x);
void print_result(Buff *buff, vector<Result> results, string buf);

// Main column graphics
void print_hello_screen();
void print_menu(unsigned N, unsigned rows, unsigned menu_height);
void print_interpolation_status(bool init_flag);
void press_continue();

// Console right side interface flow
void enter_command(enum keys *key);
bool ask_agree();
bool ask_not_agree();
void enter_a_b(Buff *buff, unsigned N, unsigned *a, unsigned *b);
void enter_x(Buff *buff, PointsArray Pts, double *x);
int enter_double(Buff *buff, double *x);
int enter_int(Buff *buff, unsigned int *x);
int enter_command(Buff *buff, unsigned int *x);
void press_continue_right(Buff *buff);
void reset_right_column(deque<string> *str_dq, unsigned int rows, unsigned int first_line_offset);