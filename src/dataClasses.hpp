#include "termCommands.hpp"
#include <deque>
#include <vector>
#include <iostream>
#include <sstream>


using namespace std;

struct InterpFlags{
    bool init_flag : 1;
    bool empty_file_flag : 1;
    bool new_params_flag : 1;
};

struct WindowProps{
    unsigned int cols;
    unsigned int rows;
    short unsigned int bottom_space : 4;
    short unsigned int offset : 6;
    short unsigned int left_height : 6;
    short unsigned int right_height;
    WindowProps() {
        cols = 0;
        rows = 0;
        offset = 54;
        bottom_space = 3;
        left_height = 32;
        right_height = 0;
    };
};

struct Result{
    double x;
    double y;
    short unsigned int func_idx : 8;
    short unsigned int method_idx : 8;
    Result() : x(0), y(0), func_idx(0), method_idx(0) {};
    Result(int _x, int _y, int _idx, int m_idx) : x(_x), y(_y), func_idx(_idx), method_idx(m_idx) {};
};

   /* * * * * * * * * * * * * *
    * Column creating class!  *
    * * * * * * * * * * * * * */


#define EMPTY_COL_LINES_PTR(idx, SIZE) buff->str_dq[idx]->resize(buff->str_dq[idx]->size()+SIZE);
#define EMPTY_COL_LINES(idx, SIZE) buff.str_dq[idx]->resize(buff.str_dq[idx]->size()+SIZE);

class Buff : public ostream {
    private:   
        unsigned short int *col_sizes;
    public:
        stringstream content;
        WindowProps menu;
        short unsigned n_cols = 2;
        deque<string> **str_dq;
        short unsigned get_offset(short unsigned idx);
        void move_side_column(short unsigned idx, short unsigned col_offset);
        void print_to_side_column(short unsigned idx, short unsigned col_offset);
        void print_input_to_column(short unsigned idx, short unsigned col_offset);
        void print_side_column(short unsigned idx, short unsigned col_offset);
        short unsigned width_of_col(short unsigned idx);

        Buff(){   
            str_dq = new deque<string>* [n_cols];
            for(short unsigned i = 0; i < n_cols; i++)   
            {
                str_dq[i] = new deque<string>;
            }    
            col_sizes = new short unsigned[n_cols];
            /*for(short unsigned i = 0; i < n_cols; i++)
            {
                col_sizes[i] = 20;
            }*/
            col_sizes[0] = 60;
            col_sizes[1] = 20;
            mt_getscreensize(&menu.rows, &menu.cols);
        };
        ~Buff(){
            delete[] str_dq;
        };
};


void Buff::move_side_column(short unsigned idx, short unsigned col_offset)
{
    //mt_getscreensize(&menu.rows, &menu.cols);
    menu.right_height = menu.rows - menu.bottom_space;
    while(menu.right_height <= str_dq[idx]->size()) 
    {
        str_dq[idx]->pop_front();
        for (int i = 0; i < menu.right_height - 1; i++)
        {
            mt_gotoXY(menu.offset + col_offset, i + 2);
            cout.width(col_sizes[idx]);
            cout << "";
            mt_gotoXY(menu.offset + col_offset, i + 2);
            cout << str_dq[idx]->at(i);
        }
        mt_gotoXY(menu.offset + col_offset, menu.right_height + 1);
        cout.width(col_sizes[idx]);
        cout << "";
    }
}

void Buff::print_to_side_column(short unsigned idx, short unsigned col_offset)
{
    str_dq[idx]->push_back(content.str());
    mt_gotoXY(menu.offset + col_offset, str_dq[idx]->size() + 1);
    cout << content.str();
    //usleep(500000);
    //cout.clear();
    content.str(string());
    content.clear();
}

void Buff::print_side_column(short unsigned idx, short unsigned col_offset)
{
    while(str_dq[idx]->size() >= menu.right_height)
        str_dq[idx]->pop_front();
    unsigned i = 0;
    for(auto it = str_dq[idx]->cbegin(); it != str_dq[idx]->cend() ; it++, i++)
    {
        mt_gotoXY(menu.offset + col_offset, 2 + i);
        cout << str_dq[idx]->at(i) << "";
    }
}
void Buff::print_input_to_column(short unsigned idx, short unsigned col_offset)
{
    mt_gotoXY(menu.offset + col_offset, str_dq[idx]->size() + 2);
    //cout.width(col_sizes[idx]);
    cout << content.str();
}

short unsigned Buff::get_offset(short unsigned idx)
{
    short unsigned col_offset = 0;
    for(short unsigned i = 0; i < idx; i++)
    {
        col_offset += col_sizes[i];
    }
    return col_offset;
}


short unsigned Buff::width_of_col(short unsigned idx)
{
    return col_sizes[idx];
}

// Overloaded '<<' operator

template<typename V> 
Buff &operator<<(Buff &Obj, V buf) ;
template<typename V>
Buff &operator<<(Buff &Obj, V buf)
{
    Obj.content << buf;
    return Obj;
}

template<>
Buff &operator<<(Buff &Obj, string buf)
{
    Obj.content << buf;
    return Obj;
}


Buff &operator<<(Buff &Obj, const char *buf) ;
Buff &operator<<(Buff &Obj, const char *buf)
{
    Obj.content << buf;
    return Obj;
}

Buff &operator<<(Buff &Obj, char *buf) ;
Buff &operator<<(Buff &Obj, char *buf)
{
    Obj.content << buf;
    return Obj;
}


// User stream manipulators:

struct smanip{

    Buff& (*MyManip)(Buff&, int) ;
    int i;
    smanip(Buff& (*ff)(Buff&, int), int ii) : MyManip(ff), i(ii){}
};

template<> // Making template for 'V' ignore the declaration for 'smanip' structure
Buff& operator<<(Buff &Obj, smanip buf){
    return buf.MyManip(Obj,buf.i);
};

// Single word:

typedef Buff& (*MyStreamManipulator)(Buff&) ;

Buff& operator<<(Buff &Obj, MyStreamManipulator manip)
{
    return manip(Obj);
}


Buff& fixed(Buff& Obj)
{
    Obj.content << fixed;
    return Obj;
}

Buff& defaultfloat(Buff& Obj)
{
    Obj.content << defaultfloat;
    return Obj;
}


// With arguments:

Buff& right(Buff& Obj, int idx)
{
    cout << endl;
    Obj.move_side_column(idx, Obj.get_offset(idx));
    Obj.print_to_side_column(idx, Obj.get_offset(idx));
    return Obj;
}

Buff& right_input(Buff& Obj, int idx)
{
    cout << endl;
    Obj.move_side_column(idx, Obj.get_offset(idx) ) ;
    Obj.print_input_to_column(idx, Obj.get_offset(idx));
    return Obj;
}

inline smanip right(int idx)
{
    return smanip(right,idx);
}


inline smanip right_input(int idx)
{
    return smanip(right_input, idx);
}


