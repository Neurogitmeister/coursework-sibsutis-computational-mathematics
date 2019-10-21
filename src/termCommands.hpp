#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>

#include <string.h>
#include <iostream>

extern struct termios term_save;

enum class keys {
    Key_0,  Key_1,  Key_2,  Key_3,  Key_4,  Key_5,  Key_6, 
    Key_Z,  Key_N,  Key_E,  Key_X,  Key_I,  Key_F, Key_H, 
    Key_R,  Key_S,  Key_A,  Key_Y, Key_P,
    Key_Q, 
	F5,
	F6,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ENTER,
    F12
};
bool rk_agree();
bool rk_not_agree();
int rk_readkey(enum keys *key);
int rk_mytermsave();
int rk_mytermrestore();
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint);

enum colors 
{
    BLACK = 0, 
    RED, 
    GREEN, 
    YELLOW, 
    BLUE, 
    PURPLE, 
    CYAN, 
    WHITE, 
    DEFAULT_COLOR
};

int mt_clrscr();
int mt_gotoXY(unsigned int col, unsigned int row);
int mt_getscreensize(unsigned int *rows, unsigned int *cols);
int mt_setfgcolor(enum colors colors);
int mt_setbgcolor(enum colors colors);