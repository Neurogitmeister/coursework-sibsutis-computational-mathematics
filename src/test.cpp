#include "dataClasses.hpp"
#include <list>



#define LINE "\n================================================\n"
#define THIN_LINE "_________________________________________________"

struct termios term_save;

void print_test(Buff *buff2);

void print_test(Buff *buff2)
{
    const int i = 30;
    long double pika = 15/13;
    char * c_str;
    string s = " easy, ";
    c_str = "chego nado?";
    *buff2 << "You gotta be crazy!" << right;
    cout << buff2;
    *buff2 << i << 5 << " " << 30  << s << c_str << "ueba " << pika << right;
    *buff2 << "THIS IS WORKING, HALLELUYAH!" << right;
    for(int i = 0; i < 128; i++)
    {
        *buff2 <<"rand[";
        buff2->width(3);
        *buff2 << i << "] = " << rand()%100  << right_right;
    }
    cout << "Test complete!\n";
    getchar();
}

void print_menu(Buff *buff, unsigned int N)
{
    cout << LINE << "          This is Interpolation menu" << LINE << "\n Available commands:\n\n" 
         << THIN_LINE << endl
         << "Z  - set everything 'Random' using points array;\n" 
         << "N  - set interpolation points array size;\n"
         << "E  - set precision value (Epsilon);\n" 
         << "X  - set argument 'x' value;\n" 
         << "I  - set Index Interval [a,b] {0,..," << N - 3 << "; 2,..," << N - 1 << "};\n" 
         << "F  - chose function and/or initialize points;\n\n"
         << THIN_LINE << endl
         << "1  - interpolate using Lagrange's mehtod;\n" 
         << "2  - interpolate using Eitkin's mehtod;\n" 
         << "3  - interpolate using Newton's mehtod;\n"
         << "4  - interpolate using Cubic Spline method;\n" 
         << "5 -  interpolate using Trigonometric method;\n"
         << "R - Record interpolation data to file;\n\n"
         << THIN_LINE << endl
         << "A - Print all points;\n"
         << "S - Print points in Section [a..b];\n"
         << "Y - Print found values;\n\n"
         << "(any other key to exit)\n\n";
    for(unsigned int i = 0; i < buff->menu.rows - buff->menu.left_height - 1; i++)
        cout << endl;
    mt_gotoXY(17, 8);
    cout << "Initialization:";
    mt_gotoXY(15, 16);
    cout << "Interpolation";
    mt_gotoXY(22, 24);
    cout << "Print:";
    mt_gotoXY(1,buff->menu.left_height);
}

int main()
{
    Buff buff;

    list<Result> *y_found;
    string buf = " ";
    mt_clrscr();
    print_menu(&buff, 20);
    print_test(&buff);
    
    cout << "Exit!\n";
}