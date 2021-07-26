Coursework on computational mathematics
# Function interpolation methods

This work is a proof-of-concept for interpolation methods, which can show with a plot how close are certain interpolation methods to the original function plot.

Program offers you to select any desired number of points and set each X coordinate manually or get them from dividing by a period.
## Technologies:
- c++ language
  - overload standard cout "<<" operator
  - add custom cout stream manipulators: right, print_right
  - vector, deque, struct
  - function pointers, array of pointers
  - write to and read from gluplot script file
  - extra POSIX thread for launching gnuplot without exiting program
  - memory usage optimizations
- termios (caret movement, graphics)
- gnuplot (function plotting)
## Functionality:
- Console interface
  - Adaptive and responsive 2-column design
  - Command menu on the left
  - Command result output on the right (with optional intermediate calculations output)
- List of available functions (options can be customized in the code)
  - f(x) = 3x³ - x²
  - f(x) = x½
  - f(x) = 5cos(2x), T = Pi
  - f(x) = exp( 40/(10 + x)) * sin(x), T' = 2*Pi
  - f(x) = exp(sin(x) + cos(x)), T' = 2*Pi
- Interpolation methods
  - Newton
  - Eitken
  - Lagrange
  - Cubic spline
  - Trigonometric
- Function graph plotter
  - select any amount of original functions to graph
  - select interpolation methods one by one until you're ready to plot
  - able to select same method multiple times
  - plot every graph point-by-point with automatically calculated optimized plot step
