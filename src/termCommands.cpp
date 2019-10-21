#include "termCommands.hpp"

int mt_clrscr()
{
    mt_gotoXY(0,0);
    std::cout << "\E[H\E[J";
    return 0;
}

int mt_gotoXY(unsigned int x, unsigned int y)
{
    unsigned int rows, cols;
    mt_getscreensize(&rows, &cols);
    if (x < 0 || x > cols || y < 0 || y > rows) 
        return -1;
    std::cout << "\033[" << y << ";" << x << "H";
    return 0;
}

int mt_getscreensize(unsigned int *rows, unsigned int *cols)
{
    struct winsize 
    {
        unsigned short ws_row;
        unsigned short ws_cols;
        unsigned short ws_xpixel;
        unsigned short ws_ypixel;
    } ws;
    if (!ioctl(1, TIOCGWINSZ, &ws)) 
    {
        *rows = ws.ws_row;
        *cols = ws.ws_cols;
        return 0;
    }
    fprintf (stderr, "Ошибка получения размера экрана.\n");
    return -1;
}

int mt_setfgcolor(enum colors colors)
{
    if ((int)colors < 0 || (int)colors >= DEFAULT_COLOR)
        return -1;
    int color = (int)colors;
    std::string buf;
    buf += "\E[3%dm" + color;
    std::cout << buf;
    return 0;
}

int mt_setbgcolor(enum colors colors)
{
    if ((int)colors < 0 || (int)colors >= DEFAULT_COLOR)
        return -1;
    int color = (int)colors;
    std::string buf;
    buf += "\E[3%dm" + color;
    std::cout << buf;
    return 0;
}

bool rk_agree()
{
  char buff[5];
  read(1, buff, 5);
  if(!strncmp(buff, "n", 1)) return 0;
  else return 1;
}

bool rk_not_agree()
{
  char buff[5];
  read(1, buff, 5);
  if(!strncmp(buff, "y", 1)) return 0;
  else return 1;
}

int rk_readkey(enum keys *key)
{
  //std::cout << "Entered readkey:";
	char buff[5];
    *key = keys::F12;
    if(read(1, buff, 5) < 0) return -1;

    if(!strncmp(buff, "0", 1))         { *key = keys::Key_0;    return 0; }
    if(!strncmp(buff, "1", 1))         { *key = keys::Key_1;    return 0; }
    if(!strncmp(buff, "2", 1))         { *key = keys::Key_2;    return 0; }
    if(!strncmp(buff, "3", 1))         { *key = keys::Key_3;    return 0; }
    if(!strncmp(buff, "4", 1))         { *key = keys::Key_4;    return 0; }
    if(!strncmp(buff, "5", 1))         { *key = keys::Key_5;    return 0; }
    if(!strncmp(buff, "6", 1))         { *key = keys::Key_6;    return 0; }
    if(!strncmp(buff, "z", 1))         { *key = keys::Key_Z;    return 0; }
    if(!strncmp(buff, "a", 1))         { *key = keys::Key_A;    return 0; }
    if(!strncmp(buff, "e", 1))         { *key = keys::Key_E;    return 0; }
    if(!strncmp(buff, "f", 1))         { *key = keys::Key_F;    return 0; }
    if(!strncmp(buff, "i", 1))         { *key = keys::Key_I;    return 0; }
    if(!strncmp(buff, "n", 1))         { *key = keys::Key_N;    return 0; }
    if(!strncmp(buff, "s", 1))         { *key = keys::Key_S;    return 0; }
    if(!strncmp(buff, "r", 1))         { *key = keys::Key_R;    return 0; }
    if(!strncmp(buff, "x", 1))         { *key = keys::Key_X;    return 0; }
    if(!strncmp(buff, "y", 1))         { *key = keys::Key_Y;    return 0; }
    if(!strncmp(buff, "q", 1))         { *key = keys::Key_Q;    return 0; }
    if(!strncmp(buff, "h", 1))         { *key = keys::Key_H;    return 0; }
    if(!strncmp(buff, "p", 1))         { *key = keys::Key_P;    return 0; }
    if(!strncmp(buff, "\E[15~", 1))    { *key = keys::F5;       return 0; }
    if(!strncmp(buff, "\E[17~", 1))    { *key = keys::F6;       return 0; }
    if(!strncmp(buff, "\E[A", 1))      { *key = keys::UP;       return 0; }
    if(!strncmp(buff, "\E[B", 1))      { *key = keys::DOWN;     return 0; }
    if(!strncmp(buff, "\E[C", 1))      { *key = keys::LEFT;     return 0; }
    if(!strncmp(buff, "\E[D", 1))      { *key = keys::RIGHT;    return 0; }
    if(!strncmp(buff, "\n", 1))        { *key = keys::ENTER;    return 0; }
    if(!strncmp(buff, "\E[24~", 1))    { *key = keys::F12;      return 0; }
  return 0; 
}

int rk_mytermsave()
{
    return tcgetattr(1, &term_save);
}

int rk_mytermrestore()
{
    return tcsetattr(1, TCSANOW, &term_save);
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
  struct termios options;

  if (tcgetattr(1, &options) != 0)
    return -1;
  if (regime == 1)
    options.c_lflag |= ICANON;
  else
  if (regime == 0)
    options.c_lflag &= ~ICANON;
  else
    return -1;
  if (regime == 0) {
    options.c_cc[VTIME] = vtime;
    options.c_cc[VMIN] = vmin;
    if (echo == 1)
      options.c_lflag |= ECHO;
    else
    if (echo == 0)
      options.c_lflag &= ~ECHO;
    else
      return -1;
    if (sigint == 1)
      options.c_lflag |= ISIG;
    else
    if (sigint == 0)
      options.c_lflag &= ~ISIG;
    else
      return -1;
  }
  if (tcsetattr(1, TCSANOW, &options) != 0)
    return -1;

  return 0;
}
