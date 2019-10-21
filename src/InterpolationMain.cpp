#include "InterpolationMain.hpp"

    /* * * * * * * * * * * * * * * *
    * Interpolation() global vars *
    * * * * * * * * * * * * * * * */

    
    // Clean defaults:                                            Stack size:

    /* Uninitialized */                                                               
    unsigned int sections, n_points_or_degree, n_plot_points;   // 5x32bit = 20bytes
    
   /* * * * * * * * * * * * * * * * * * * Can be freed  * * * * * * * * * * * * * * * * * * *
    * FREED:                                                                                *
    * // Change 'init_flag' to '1' to load custom defaults, '0' if clean defaults           *
    * bool init_flag = 0;                                   // 8bit = 1byte                 *
    * double x, x0, h, h_max, h_min;                        // 5x64bit = 40bytes            *
    * unsigned int y_counter;                               // 1x32bit = 4bytes             *
    * double *results = (double*)malloc(sizeof(double)*20); // 20x64bit = 160bytes          *
    *                                                                                       *
    * Buffer string                                                                         *
    *  string buf = "";                                     // 8-256bit = 1byte-32bytes     *
    *                                                                                       *
    * Currently used function struct                                                        *
    *   func_struct func_curr;                              // 50x8bit char +               *
    *                                                       // 64bit double +               *
    *                                                       // 64bit->64bit ptr to double = *
    *                                                       // 592bit = 74bytes             *
    * short unsigned int curr_func_idx = 0, last_recorded_func_idx = 0;                     *
    *                                                       // 2x16bit = 4bytes             *
    *                                                                                       *
    * float plot_density = 0.01;                            // 1x32bit = 4bytes             *
    *                                                                                       *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    * NOT FREED:       <EMPTY>                                                              *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
    *           Total potentially free:                 *
    *               283..314bytes                       *
    *                                                   *
    *           Total min (empty buf):                  *
    *               2448bit = 311bytes                  *
    *                                                   *
    *           Total max (32 chars in buf):            *
    *               2736bit = 342bytes                  *
    *                                                   *
    *           Total after free:                       *
    *               220bit = 28bytes (-76.9%, -79%)     *
    *                                                   *
    * * * * * * * * * * * * * * * * * * * * * * * * * * */

void load_defaults(func_struct *func_mas, func_struct *func_curr, short unsigned *curr_func_idx, PointsArray *Pts, double *x, double *x0, \
    double *h, double *h_min, double *h_max)
{
    Pts->p_arr.resize(20);  // General interpolation points pool size

    *x = 5.0;                // Requested 'x'
    *x0 = -5;                // Starting point's 'x' argument
    *h = 4; //PI/5;          // step = 0;
    *h_max = 5.0;            // Maximum augment for x
    *h_min = 0.1 / Pts->p_arr.size();        // Minimum augment for x

    Pts->a = 0, Pts->b = 19;          // Index interval
    sections = 1; // Value for sections per single trigonometric polinomia

    n_plot_points = 200;  

    *curr_func_idx = 4; // Kept index 'curr_func_idx' of currently used function
    *func_curr = func_mas[*curr_func_idx];
}

int main()
{  
    // Console columns mode buffer
    Buff buff;
    
    // Array of interpolation results
    vector<Result> results;
    Result result; // Temp result

    // Class that contains information about points array and a-b section
    PointsArray* Pts_ptr = new PointsArray();
    PointsArray Pts = *Pts_ptr;
    cout << Pts.p_arr.size();
    unsigned N = Pts.p_arr.size();

    // Array of flags (1 bit each)
    InterpFlags Flags;

   /* * * * * * * * * * * * * * *
    * Variables for user input  *
    * * * * * * * * * * * * * * */

    enum keys key, key_main = keys::F5;
    unsigned int key2 = 0;
    string buf = " ";
    int exception = 0;
    double double_tmp;


   /* * * * * * * * * * * * * * * * * * * * * * * *
    * Variables and structures for interpolation: *
    * * * * * * * * * * * * * * * * * * * * * * * */

    Epsilon Pepsilon;
    Pepsilon.set_value(0.01);
    // Setting initial precision
    cout.precision(Pepsilon.max_eps_pow);
    buff.content.precision(Pepsilon.max_eps_pow);

    func_struct func_arr[n_func]; // Array of functions
    initialize_func_arr(func_arr);

    short unsigned int curr_func_idx = 0;
    func_struct func_curr = func_arr[curr_func_idx]; // Currently used function
    double x, x0, h, h_max, h_min;
    h_max = 5.0;            // Maximum augment for x
    h_min = 0.1 / N;        // Minimum augment for x

   /* * * * * * * * * * * * * * * * * * * * * * * *
    * Variables used for interpolation plotting : *
    * * * * * * * * * * * * * * * * * * * * * * * */
    n_plot_points = 200;
    double plot_density = (Pts.b - Pts.a) * h / n_plot_points; /* Plot points density */
    double y_max, y_min;
    // Last used for recording 
    unsigned a_last = -1, b_last = -1;
    double h_last = -1, x0_last;
    (*((long long*)&x0_last))= ~(1LL<<52);
    short unsigned last_recorded_func_idx = 0;
    string plotdat_path = "plot/datafiles/plotdat.txt";
        /*// ( Rewriting plot data file if needed )
        Flags.empty_file_flag = 1;
        rewrite_data(plotdat_path);*/

   /* * * * * * * * * * * *
    * Starting programm!  *
    * * * * * * * * * * * */

    // Change 'init_flag' to '1' to load custom defaults, '0' if clean defaults
    Flags.init_flag = 0;

    print_hello_screen();
    mt_getscreensize(&buff.menu.rows, &buff.menu.cols);
    rk_mytermsave();
    // Randomizer for random programm inputs option
    srand(time(NULL)); 
    int counter = 0; // Randomizer iterations counter

    // Printing start menu
    mt_clrscr();
    print_menu(N, buff.menu.rows, buff.menu.left_height);
    print_interpolation_status(Flags.init_flag);
    if(Flags.init_flag)
    {
        load_defaults(func_arr, &func_curr, &curr_func_idx, &Pts, &x, &x0, &h, &h_min, &h_max);
        set_points(&Pts.p_arr, N, x0, h, func_curr.f, false);
        print_points(&buff, Pts.p_arr);
        get_offset(&buff, Pts.p_arr, x);
    }

     //////////     ///  ///  ///     ///////////     ///////////      //////////    ///     ///
    ///             ///  ///  ///         ///             ///         ///            ///     ///
     /////////      ///  ///  ///         ///             ///         ///            ///////////
            ///     ///  ///  ///         ///             ///         ///            ///     ///
    //////////      /////////////     ///////////         ///          //////////    ///     ///

    while (key_main != keys::Key_Q)
	{
        mt_gotoXY(1,buff.menu.left_height+1);
        cout << "Command =";
        cout.width(buff.menu.offset - 10);
        cout << " ";

        mt_gotoXY(11,buff.menu.left_height+1); // Entering command
        enter_command(&key_main);

        if(key_main == keys::Key_Y) 
        {
            EMPTY_COL_LINES(0,1);
            //print_results(&buff, *results);
            continue;
        } 
        else if(key_main == keys::Key_1 || key_main == keys::Key_2 || key_main == keys::Key_3 || key_main == keys::Key_4 || key_main == keys::Key_5 || key_main == keys::Key_R || key_main == keys::Key_P || \
                key_main == keys::Key_Z || key_main == keys::Key_N || key_main == keys::Key_E || key_main == keys::Key_X || key_main == keys::Key_I || key_main == keys::Key_F || key_main == keys::Key_H || \
                key_main == keys::Key_A || key_main == keys::Key_S)
        {
            buff << THIN_LINE << right(0); 
            EMPTY_COL_LINES(0,1);  
        }
        

		switch (key_main) {

		case keys::F5:
			break;

        // Section 'Initialization'
        case keys::Key_Z:
            counter = 0;
            key2 = 3; // just not to trigger '1' and '2' -->
            do
            {
                if(key2 == 2)
                {
                    print_one_millionth_chance_error(&buff, 2);
                    h = h_min;
                    buff << "h = " << h << right(0);
                } else if(key2 == 1)
                {
                    buff <<  "Couldn't pick the right initial data. Switching function." << right(0); 
                    break;
                }
                key2 = rand()%n_func;               // <-- key2 goes here
                func_curr = func_arr[key2];
                key2 = randomize_points(&Pts.p_arr, N, &h, h_min, h_max, func_curr, &counter);
            } while(key2 != 0);

            if(counter != 1)
            {
                buff <<  "After " << counter << " tries:" << right(0); 
            }
            else if (counter == 1000) // If 10 function changes wasn't enough
            {
                reset_points(Pts.p_arr, N);
                print_one_millionth_chance_error(&buff, 1);
                Flags.init_flag = 0;
                break;
            }
            buff <<  "Random function: "<< func_curr.text << right(0); 
            EMPTY_COL_LINES(0,1);
            buff <<  "New starting point p[0]:" << right(0); 
            buff <<  "X = " << Pts.p_arr[0].X << ", Y = " << Pts.p_arr[0].Y << right(0); 
            buff <<  "h = " << h << right(0); 
            press_continue_right(&buff);
            buff <<  "Random points array:" << right(0); 
            print_points(&buff, Pts.p_arr);
            press_continue_right(&buff);

            Pts.a = rand()%(N - 2);
            if(Pts.a == N - 2) 
                Pts.b = N;
            else
                Pts.b = rand()%(N - (Pts.a + 2)) + (Pts.a + 2);
            buff <<  "Section of " << Pts.b - Pts.a << "/" << N << " points : p_arr[" << Pts.a << ",..," << Pts.b << "]" << right(0); 
            EMPTY_COL_LINES(0,1);

            double bx, ax;      
            bx = Pts.p_arr[Pts.b].X;
            ax = Pts.p_arr[Pts.a].X; 

            do {
                x = (bx + ax) / 2.0; // Coordinate of the middle [ax,bx]

                // Random int dispersion of length [ax,bx]
                if ((int)bx - (int)ax != 0)        
                    x += rand()%( (int)bx - (int)ax) - ( bx - ax ) / 2.0; 
                else 
                    x = rand()%(10*Pepsilon.max_eps_pow + 1) * (bx - ax) / (10.0*(double)Pepsilon.max_eps_pow) + ax;
            } while(x > Pts.p_arr[N-1].X && x < Pts.p_arr[0].X);

            buff <<  "X should be in x[" << Pts.p_arr[Pts.a].X << "..." << Pts.p_arr[Pts.b].X << "]" << right(0); 
            buff <<  "x = " << x << right(0); 
            EMPTY_COL_LINES(0,1);

            do {
                Pepsilon.value = rand()%(8-1)+1;
                Pepsilon.value = pow(0.1, Pepsilon.value);
            } while(Pepsilon.value < Pepsilon.eps_max || Pepsilon.value > Pepsilon.eps_min);

            buff <<  fixed << "Random epsilon = " << Pepsilon.value << defaultfloat << right(0); 
            EMPTY_COL_LINES(0,1);
            buff <<  "Randomizing's successful!" << right(0); 
            press_continue_right(&buff);

            Flags.init_flag = 1;
            break;

        case keys::Key_N:
        { 
            buff << "Enter points amount: " << right; 
            buff << "N = " << right_input(0); 
            enter_int(&buff, &key2);
            vector<myPoint> points_temp(key2); 
            Pts.p_arr.swap(points_temp);
            vector<myPoint>().swap(points_temp); // Constructor that replaces temp array with unitialized local array to be discarded.

            N = key2;
            while(set_points(&Pts.p_arr, N, x0, h, func_curr.f, false) != 0)
            {
                buff << "Can't use the current settings. Setting them to default" << right(0);
                x0 = 0.0;
                h = 1.0;
                func_curr = func_arr[0];
            }
            if(Pts.a > N - 1) Pts.a = N - 1;
            Flags.init_flag = 1;
            break;
        }

		case keys::Key_E:

			buff <<  "Epsilon = " << right_input(0); 
			enter_double(&buff, &double_tmp);
            exception = Pepsilon.set_value(double_tmp);
            if(exception == 1)
            {
                buff <<  "Precision too high." << right(0); 
                buff <<  "Setting it to max allowed." << right(0); 
                EMPTY_COL_LINES(0,1);
            }
            else if(exception == 2)
            {
                buff <<  "Precision too low." << right(0); 
                buff <<  "Setting it to min allowed." << right(0); 
                EMPTY_COL_LINES(0,1);
            }
			break;


		case keys::Key_X:

            if(Flags.init_flag)
               enter_x(&buff, Pts, &x);
            else
            {
                buff <<  "Points not initialized! Try going to '4'." << right(0); 
            }
			break;


		case keys::Key_I:

            if(Flags.init_flag == 0)
            {
                buff <<  "Points not initialized! Try going to '4'." << right(0); 
                break;
            }
			enter_a_b(&buff, N, &Pts.a,&Pts.b);
            enter_x(&buff, Pts, &x);
            Flags.init_flag = 1;
			break;


		case keys::Key_F:

            buff <<  "Choose function (type in the number):" << right(0); 
            EMPTY_COL_LINES(0,1);
            for(unsigned int i = 0; i < n_func; i++)
            {
                buff << i << " - " << func_arr[i].text << right(0); 
            }
            EMPTY_COL_LINES(0,1);
            buff <<  "(Else - leave last used or default)" << right(0); 
            EMPTY_COL_LINES(0,1);
            enter_command(&key);
            if((int)key >= 0 && (int)key <= 4)
            {
                func_curr = func_arr[(int)key];
                curr_func_idx = (int)key;
            }

            // Genius branching below! :^)
            key2 = 0;
            if (Flags.init_flag)
            {
                buff << "Skip initialization? [Y/n]" << right(0);     
                key2 = ask_agree();  
            }
            if(key2)
            {
                buff <<  "Using old data:" << right(0); 
                buff << fixed << "h = " << h << right(0); 
                buff <<  "x0 = " << x0 << right(0);              
                key2 = (unsigned)~0; 
            }
            else {
                buff << "Init points (" << N <<  " points):" << right(0);
                buff << "Set 'x0' = " << right_input(0);						
                enter_double(&buff, &x0);         
                key2 = (unsigned)~0 - 1;
            }
            // Continue by falling into 'Key_H' to set points

        case keys::Key_H:
            if(key2 != (unsigned)~0)
            {
                buff << "Set 'h' = "  << right_input(0);				
                enter_double(&buff, &h);
                while(h*pow(10,Pepsilon.max_eps_pow) < 1) 
                {
                    buff << "Error! 'h' is too low!" << right(0);
                    buff << "h = "  << right_input(0);				
                    enter_double(&buff, &h);
                }
                key2++;
            }        
            
            
            if(set_points(&Pts.p_arr, N, x0,h,func_curr.f) == 0)
            {
                // Proceeding if function didn't have complex ('nan') values
                if(key2 == (unsigned)~0)
                {
                    enter_a_b(&buff, N, &Pts.a, &Pts.b);
                    enter_x(&buff, Pts, &x); 
                }
                else if((x < Pts.p_arr[Pts.a].X) || (x > Pts.p_arr[Pts.b].X))
                    enter_x(&buff, Pts, &x);
                get_offset(&buff, Pts.p_arr, x);
                Flags.init_flag = 1;
                EMPTY_COL_LINES(0,1);
                print_points(&buff, Pts.p_arr);
            }
            else
            {
                print_fun_error(&buff);
            }
            key2 = 0;
            break;
        // Section 'Print'
        case keys::Key_A:
            print_points(&buff, Pts.p_arr);
            break;

        case keys::Key_S:
            print_points_ab(&buff, Pts);
            break;

        // Section 'Interpolation'
        case keys::Key_R: 
            plot_density = (Pts.p_arr[Pts.b].X - Pts.p_arr[Pts.a].X) / n_plot_points;
            if(curr_func_idx != last_recorded_func_idx || h_last != h || a_last != Pts.a || x0_last != x0 || b_last != Pts.b) 
                 Flags.new_params_flag = 1;
            else Flags.new_params_flag = 0;
            if(!(plot_record_data_menu(&plotdat_path, &buff, Pts, func_curr, Pepsilon, plot_density, &Flags))) {
                a_last = Pts.a;
                b_last = Pts.b;
                h_last = h;
                x0_last = x0;
                last_recorded_func_idx = curr_func_idx;
            }
            break;

        case keys::Key_P:{
            unsigned int xtics, ytics;
            string png_path = "plot/images/plot_img0.png";
            unsigned img_counter = 1;
            while(exists(png_path)) {
                png_path.resize(20);
                png_path += to_string(img_counter++) + ".png";
            }
            buff << "Select number of tics:" << right(0);
            buff << "x: " << right_input(0);
            cin >> xtics;
            buff << xtics << right(0);
            buff << "y: " << right_input(0);
            cin >> ytics;
            if (ytics < 1)
                ytics = 1;
            buff << ytics << right(0);
            extremum_from_points(Pts.p_arr, Pts.a, Pts.b, &y_max, &y_min);
            key2 = arrange_plot(plotdat_path, png_path, func_curr.text, Pts.p_arr[Pts.a].X, Pts.p_arr[Pts.b].X, y_min, y_max, xtics, ytics);
            switch(key2) {
                case -1:
                    buff << "Error! Script file - 'plot/plot_script.gnu' not found!" << right(0);
                    break;
                case -2:
                    buff << "Error! Data file not found!" << right(0);
                    break;
                case 1:
                    buff << "Error! Plot tics can't be equal to 0!" << right(0);
                    break;
                case 2:
                    buff << "Error! No interpolation info in the file!" << right(0);
                    break;
                default: break;
            }
            system("gnuplot plot/plot_script.gnu");
            png_path = "eog " + png_path;
            system(png_path.c_str());
            break;
        }

		case keys::Key_1:
        case keys::Key_2:
        case keys::Key_3:
        case keys::Key_4:
        case keys::Key_5:
            switch(key_main){
                case keys::Key_1:
                    buf = "Lagrange";
                    break;
                case keys::Key_2:
                    buf = "Eitken";
                    break;
                case keys::Key_3:
                    buf = "Newton";
                    break;
                case keys::Key_4:
                    buf = "Cubic Spline";
                    break;
                case keys::Key_5:
                    buf = "Trigonometric";
                    break;
                default: break;
            }
            buff << buf << " interpolation:" << right(0);
            if(Flags.init_flag)
                key2 = interpolate(&result, &buff, Pts, x, Pepsilon, curr_func_idx, key_main);
            else key2 = -2;

            switch(key2){
            case 0: // Interpolation successful
                results.emplace_back(result);

                press_continue();

                print_menu(N, buff.menu.rows, buff.menu.left_height);                   // Print menu nav and interpolation status           
                print_interpolation_status(Flags.init_flag);
                buff.print_side_column(0, buff.get_offset(0));                          // Resume printing to right column from last point   
                //EMPTY_COL_LINES(0,1)
                buff << "f("<< x << ") ~ " << results.back().y << right(0);            // Print last found value

                buff.print_side_column(1, buff.get_offset(1));
                print_result(&buff, results, buf); // Print last 'results_max' found values

                press_continue_right(&buff);
                break;
            // Catching errors:
            case -2: 
                buff <<  "Points not initialized! Try going to '4'." << right(0);
                break;
            case 2:
                buff << "Error! Trigonometric method failed for '2n+1 > N-1'!" << right(0); 
                break;
            case 1:
                buff << "Error! 'x' not found in sector x[0] x[N]!" << right(0); 
                break;
            case -1:
                buff << "Wrong key!" << right(0);
                break;
            default: break;
            }
        	break;
        // Default for main switch
		default: break;
        }
    

    ///////////      //////////      ///////////     ///       ///     ///////////   
    ///      ///     ///     ///         ///         //////    ///         ///           
    ///////////      //////////          ///         /// ///// ///         ///           
    ///              ///  ///            ///         ///    //////         ///           
    ///              ///    ///      ///////////     ///       ///         ///          


    ///       ///     //////////     ///       ///    ///       /// 
    /////   /////     ///            //////    ///    ///       /// 
    /// ///// ///     //////////     /// ///// ///    ///       /// 
    ///  //   ///     ///            ///    //////    ////     ////  
    ///       ///     //////////     ///       ///      /////////

        // Actions for changing settings 'Z - I - E - F - X - N' 
        switch(key_main){
            case keys::Key_Z:
            case keys::Key_I:
            case keys::Key_E:
            case keys::Key_F:
            case keys::Key_N:
            case keys::Key_X:

                if(Flags.init_flag != 0)
                    // Print current 'x' and it's nearest-left point
                    //buff << "changing settings" << right(0);
                    get_offset(&buff, Pts.p_arr, x);
                
                print_interpolation_status(Flags.init_flag);
                break;

            default: break;
        }

	}
    // Exit sequence
    mt_gotoXY(1,buff.menu.rows);
    vector<myPoint>().swap(Pts.p_arr);
    return 0;
}

// uses N
void print_menu(unsigned N, unsigned rows, unsigned menu_height)
{
    cout << LINE << "          This is Interpolation menu" << LINE << "\n Available commands:\n\n" 
         << THIN_LINE << endl
         << "Z  - set everything 'Random' using points array;\n" 
         << "N  - set interpolation points array size;\n"
         << "E  - set precision value (Epsilon);\n" 
         << "X  - set argument 'x' value;\n" 
         << "I  - set Index Interval [a,b] {0,..," << N - 3 << "; 2,..," << N - 1 << "};\n"
         << "H  - set 'H' - distance between points;\n"
         << "F  - chose function and/or initialize points;\n\n"
         << THIN_LINE << endl
         << "1  - interpolate using Lagrange's mehtod;\n" 
         << "2  - interpolate using Eitkin's mehtod;\n" 
         << "3  - interpolate using Newton's mehtod;\n"
         << "4  - interpolate using Cubic Spline method;\n" 
         << "5  - interpolate using Trigonometric method;\n"
         << "R  - Record interpolation data to file;\n"
         << "P  - make Plot from datafile;\n\n"
         << THIN_LINE << endl
         << "A  - print All points;\n"
         << "S  - print points in Section [a..b];\n"
         << "Y  - print found Y values;\n\n"
         << "('Q' to exit)\n";
    for(unsigned int i = 0; i < rows - menu_height; i++)
        cout << "\n";
    mt_gotoXY(17, 8);
    cout << "Initialization:";
    mt_gotoXY(15, 17);
    cout << "Interpolation";
    mt_gotoXY(22, 26);
    cout << "Print:";
    mt_gotoXY(1,menu_height);
}

void print_hello_screen()
{
    cout << LINE << "* This is Interpolation menu *" << LINE;
    cout << "\n\n* We need you to maximize the terminal window size before we start! *";
    cout << "\n\n/ Press any key when you're ready! /";
    press_continue();
}

void print_interpolation_status(bool init_flag)
{
    mt_gotoXY(28,17);
    if(init_flag == 1)
    {
        cout << "(ready):";
    }
    else
    {       
        cout << "(locked):";
    }
}

    ///////////     ///       ///     ///////////    ///////////    ///////////      //////////
        ///         //////    ///         ///        ///            ///      ///     ///     ///
        ///         /// ///// ///         ///        ///////////    ///////////      //////////
        ///         ///    //////         ///        ///            ///   ///        ///
    ///////////     ///       ///         ///        ///////////    ///     ////     ///

    ///       ///     ///////////     ///////////     ///       ///       /////////       //////////
    /////   /////     ////                ///         ///       ///      ///     ///      ///      ///
    /// ///// ///     ///////////         ///         /////////////     ///       ///     ///       ///
    ///  //   ///     ////                ///         ///       ///      ///     ///      ///      ///
    ///       ///     ///////////         ///         ///       ///       /////////       //////////

     //////////     ///  ///  ///     ///////////     ///////////      //////////    ///     ///
    ///             ///  ///  ///         ///             ///         ///            ///     ///
     /////////      ///  ///  ///         ///             ///         ///            ///////////
            ///     ///  ///  ///         ///             ///         ///            ///     ///
    //////////      /////////////     ///////////         ///          //////////    ///     ///


void set_interpolation_vars(Buff *buff, keys key, unsigned N, unsigned *n, unsigned *sections) // Result is in pointers
{
    if((key == keys::Key_3) || (key == keys::Key_4) || (key == keys::Key_5)) {
        string buf = "";
        unsigned int n_top = N;
        // 'n' insert dialog
        if(key == keys::Key_3)
            buf = "Chose the degree of polinomial Pn (1 < n < " + to_string(N) + ").";
        else
        if(key == keys::Key_4)
            buf = "Chose the number of points (1 < n < " + to_string(N) + ").";
        else
        if(key == keys::Key_5) {
            n_top = N/2;
            buf = "Chose the degree of polinomial Pn (1 < n < " + to_string(N/2) + ").";
        }   
        *buff << buf << right(0); 
        *buff <<  "n = " << right_input(0); 
        enter_int(buff, n);
        while(*n < 1 || *n > n_top - 1) { // Input error catch infinite loop
            if (*n < 1) {
                EMPTY_COL_LINES_PTR(0,1)
                *buff <<  "Very unlikely..." << right(0); 
            }
            if (*n > n_top - 1) {
                EMPTY_COL_LINES_PTR(0,1)
                *buff <<  "It won't work" << right(0); 
            }
            *buff <<  "Try one more time" << right(0);   
            EMPTY_COL_LINES_PTR(0,1)    
            *buff <<  "n = " << right_input(0);                
            enter_int(buff, n);
        }
    }
    else return;
    if(key == keys::Key_5) {
        *buff << "Enter amount of sections per function recalculation: " << right_input(0); 
        enter_int(buff, sections);
    }
}

int interpolate(Result *result, Buff *buff, PointsArray Pts, double x, Epsilon Eps, unsigned func_idx, keys key)
{
    unsigned n;
    unsigned sections;

    set_interpolation_vars(buff, key, Pts.p_arr.size(), &n, &sections);

    mt_clrscr();
    // reset_right_column(right_col, first_line_offset);

    result->x = x;
    result->func_idx = func_idx;
    switch(key){
    case keys::Key_1: 
        result->y = Lagrange(x, Pts.p_arr, Pts.p_arr.size(), Pts.a, Pts.b);
        break;
    case keys::Key_2:
        result->y = Eitken(x, Pts.p_arr, Eps, Pts.p_arr.size());
        break;
    case keys::Key_3:
        result->y = Newton(x, Pts.p_arr, Eps.max_eps_pow, Pts.p_arr.size(), n);
        break;
    case keys::Key_4:
        result->y = cubicSpline(x, Pts.p_arr, Eps.max_eps_pow, Pts.p_arr.size(), n);
        break;
    case keys::Key_5:
        break;

    default:
        return -1; // Err_code for wrong key
    }
    if(key == keys::Key_5) {
        double period = (Pts.p_arr[Pts.b].X - Pts.p_arr[Pts.a].X);
        return Trigonometric(&result->y, x, Pts.p_arr, Eps.max_eps_pow, Pts.p_arr.size(), period, n, true, sections);
        // Error code "1" - x not in sector
    }
    return 0;
}

inline bool exists (const std::string& name) 
{
    return ( access( name.c_str(), F_OK ) != -1 );
}

// uses N
int plot_record_data_menu(string *path, Buff *buff, PointsArray p, func_struct func_curr,  Epsilon Eps, float plot_density, InterpFlags *Flags)
{
    char c;
    string buf;
    istringstream iss;
    unsigned sections, n;
    keys key;
    if(!Flags->empty_file_flag) {
        if(Flags->new_params_flag) {
            *buff << "New parameters. Rewrite existing data? (N/y)" << right(0);
        } else {
            *buff << "Same parameters. Rewrite existing data? (N/y)" << right(0);
        }

        Flags->empty_file_flag = 1;
        if(!ask_not_agree()) {
            if(rewrite_data(*path)) return 1;
            if(record_function_data(*path, p, Eps, keys::Key_0, func_curr, plot_density)) return 1;
            *buff << "Rewrite success!" << right(0);
            EMPTY_COL_LINES_PTR(0,1);
        }
        else {
            // If not rewriting
            n = 2;
            do {
                // 3-step file naming dialog cycle
                switch(n) {
                    case 1:
                        *buff << "File already exists. Rewrite? (N/y)" << right(0);
                        if(!ask_not_agree()) {
                            if(rewrite_data(*path)) return 1;
                            *buff << "Rewrite success!" << right(0);
                            n = 0;
                            break;
                        }
                    case 2:
                        *buff << "Create a new file? (Y/n)" << right(0);
                        if( !ask_agree() ) {
                            Flags->empty_file_flag = 0;
                            if (buf != "" || Flags->new_params_flag) {
                                *buff << "Exiting..." << right(0);
                                return 0;
                            } else {
                                EMPTY_COL_LINES_PTR(0,1);
                                *buff << "Writing to the end of '" << *path << "'" << right(0);
                                n = 0;
                                break;
                            }
                        }
                    case 3:
                        *buff << "Enter new data file name: " << right_input(0);
                        getline(cin, buf);
                        *buff << buf << right(0);
                        *path = "plot/datafiles/" + buf + ".txt";
                        n = 1;
                        break;

                    default: break;
                } 
            } while( exists(*path) && (n != 0) ); // Exit if 'n = 0' or *path doesn't exist
            
            if(Flags->empty_file_flag) { // If rewrote a file
                if(record_function_data(*path, p, Eps, keys::Key_0, func_curr, plot_density)) return 1;
                *buff << "Function data record success!" << right(0);       
            }
        }

        
    } else {
        if(record_function_data(*path, p, Eps, keys::Key_0, func_curr, plot_density)) return 1;
    }
    // Printing buff->menu
    *buff << "Enter series of interpolation methods to use in a line:" << right(0); 
    *buff << "(example: 231)" << right(0); 
    EMPTY_COL_LINES_PTR(0,1)
    *buff << "1 - Lagrange polinomial;" << right(0);  
    *buff << "2 - Eitken;" << right(0); 
    *buff << "3 - Newton polinomial;" << right(0);  
    *buff << "4 - Cubic spline;" << right(0); 
    *buff << "5 - Trigonometric polinomial;" << right(0); 
    EMPTY_COL_LINES_PTR(0,1)
    *buff << "line: " << right_input(0);

   /* 
    * For each command sending user to the same 'interpolate' function to change interpolation values
    * and then use 'write mode' accordingly to the chosen settings. 
    *
    */
    getline(cin, buf);
    iss.str(buf);
    *buff << buf << right(0);
    while(iss >> c) {   
        EMPTY_COL_LINES_PTR(0,1);
        *buff << "* * * ";
        switch(c){ 
        case '1':
            key = keys::Key_1;
            *buff << "Lagrange * * *" << right(0);
        	break;

		case '2':
            key = keys::Key_2;
            *buff << "Eitken * * *" << right(0);
        	break;   

		case '3':
            key = keys::Key_3;
            *buff << "Newton * * *" << right(0);
        	break;
        
        case '4':
            key = keys::Key_4;
            *buff << "Cubic Spline * * *" << right(0);
            break;

        case '5':
            key = keys::Key_5;
            *buff << "Trigonometric * * *" << right(0);      
        	break;

		default:
            *buff << "ERROR: Unknown key! * * *" << right(0);
            c = '\0';
            break;
        }
        if(c != '\0') {
            EMPTY_COL_LINES_PTR(0,1);
            set_interpolation_vars(buff, key, p.p_arr.size(), &n, &sections);
            //*buff << p.p_arr[p.a].X << " " << p.p_arr[p.b].X << " " << plot_density << " " << " ";
            if(record_function_data(*path, p, Eps, key, func_curr, plot_density, n, sections)) return 1;
        }
    }
    Flags->empty_file_flag = 0;
    EMPTY_COL_LINES_PTR(0,1);
    *buff << "Record executed successfully!" << right(0);
    return 0;
}

// uses func_curr, x, a, b, N
int record_function_data(string path, PointsArray Pts, Epsilon Eps, keys key, func_struct func_curr, float h_loc, unsigned int n, unsigned int sect)
{
    fstream plot_fs;
    plot_fs.open(path);
    if(!plot_fs.is_open())
    {
        plot_fs.close();
        ofstream new_file (path);
        new_file.close();
        plot_fs.open(path);
        plot_fs << "# x y";
    }
    plot_fs.seekp(0, plot_fs.end);
    plot_fs << endl;
    plot_fs << fixed;
    plot_fs.precision(Eps.max_eps_pow);

    double x = Pts.p_arr[Pts.a].X;
    double x_end = Pts.p_arr[Pts.b].X;
    double period; // Period of 2n+1 points for trigonometric method

        double y = 0; 
    switch(key) {
        case keys::Key_0:
            plot_fs << "# Function " << func_curr.text << endl;
            plot_fs << "# Refering points: " << endl;
            for(unsigned int i = Pts.a; i <= Pts.b; i++)
            {
                plot_fs << " " << Pts.p_arr[i].X << " " << Pts.p_arr[i].Y << endl;
            }
            plot_fs << "\n# Interpolation data:\n\n";
            do
            {
                plot_fs << " " << x << " " << func_curr.f(x) << endl;
                x += h_loc;
            }   while(x <= x_end);
            break;
        case keys::Key_1:  
            plot_fs << "# Lagrange \n\n";
            do 
            {   
                plot_fs << x << " " << Lagrange(x, Pts.p_arr, Pts.p_arr.size(), Pts.a, Pts.b, false) << endl;
                x += h_loc;
            }   while(x <= x_end);
            break;

        case keys::Key_2:
            plot_fs << "# Eitken \n\n" ; 
            do 
            {   
                plot_fs << x << " " << Eitken(x, Pts.p_arr, Eps, Pts.p_arr.size(), false) << endl;
                x += h_loc;
            }   while(x <= x_end);
            break;

        case keys::Key_3:
            plot_fs << "# Newton, deg=" << n << "\n\n" ;
            do 
            {   
                plot_fs << x << " " << Newton(x, Pts.p_arr, Eps.max_eps_pow, Pts.p_arr.size(), n, false) << endl;
                x += h_loc;
            }   while(x <= x_end);
            break;

        case keys::Key_4:
            plot_fs << "# Cubic Spline, " << n << " points\n\n" ;
            do 
            {   
                plot_fs << x << " " << cubicSpline(x, Pts.p_arr, Eps.max_eps_pow, Pts.p_arr.size(), n, false) << endl;
                x += h_loc;
            }   while(x <= x_end);
            break;

        case keys::Key_5:
            plot_fs << "# Trigonometric, deg=" << n << ", sect=" << sect << " \n\n" ;
            if(2 * n + 1 > Pts.p_arr.size() - 1)
                return 2;
            period = (Pts.p_arr[2*n+1].X - Pts.p_arr[0].X);
            do 
            {   
                Trigonometric(&y, x, Pts.p_arr, Eps.max_eps_pow, Pts.p_arr.size(), period, n, false, sect);
                plot_fs << x << " " << y << endl;
                x += h_loc;
            }   while(x <= x_end);
            break;

        default:
            cout << "Wrong key!\n";
            return 2;
    }
    plot_fs.close();
    return 0;
}

int rewrite_data(string path)
{
    ofstream fs;
    fs.open(path, ios_base::in | ios_base::trunc);
    if(!fs.is_open())
    {
        return 1;
    }
    fs << "# x y";
    fs.close();
    return 0;
}

    ///////////       ///////////       ///////////       //////////       //////////      
    ///      ///      ///      ///      ///              ///              ///            
    ///////////       ///////////       ///////////       //////////       //////////   
    ///               ///   ///         ///                       ///              ///    
    ///               ///     ///       ///////////      ///////////      ///////////      

void press_continue_right(Buff *buff)
{
    *buff << "(press any key to continue)" << right(0);
    cout.flush();
    rk_mytermregime(0,0,1,0,1);
    char buf[5];
    read(1, buf, 5);
    rk_mytermrestore();
    buff->str_dq[0]->pop_back();
    *buff << "" << right(0);
}

void press_continue()
{
    cout << "\n\n(press any key to continue)";
    getchar();
    cout << "\033[1A";
    cout << "\033[K";
}

void enter_command(enum keys *key)
{
    cout.flush();
    rk_mytermregime(0,0,1,0,1);
    rk_readkey(key);
    rk_mytermrestore();
}


bool ask_agree()
{
    cout.flush();
    rk_mytermregime(0,0,1,0,1);
    bool ask = rk_agree();
    rk_mytermrestore();
    return ask;
}

bool ask_not_agree()
{
    cout.flush();
    rk_mytermregime(0,0,1,0,1);
    bool ask = rk_not_agree();
    rk_mytermrestore();
    return ask;
}

int enter_double(Buff *buff, double *x)
{
    string buf;
    cin >> *x;
    if(!cin)
    {
        *x = -65536.0;
        buff->content << "ERROR";
        *buff << right(0);
        cin.clear(); // clears the error flags
        cin.ignore(20, '\n'); //flush the buffer
        *buff <<  "So smart of you!" << right(0); 
        EMPTY_COL_LINES_PTR(0,1)
        return 1;
    }
    buff->content << *x;
    *buff << right(0);
    getchar();
    return 0;
}
int enter_int(Buff *buff, unsigned int *x)
{
    string buf;
    cin >> *x;
    if(!cin)
    {
        *x = ~0;
        buff->content << "ERROR";
        *buff << right(0);
        cin.clear(); // clears the error flags
        cin.ignore(20, '\n'); //flush the buffer
        *buff <<  "So smart of you!" << right(0); 
        EMPTY_COL_LINES_PTR(0,1)
        return 1;
    }
    buff->content << *x;
    *buff << right(0);
    getchar();
    return 0;
}

int enter_command(Buff *buff, unsigned int *x)
{
    string buf;
    if(scanf("%d", x) != 1)
    {
        buff->content << *x;
        *buff <<  "So smart of you!" << right(0); 
        getline(cin, buf);
        getline(cin, buf);
        EMPTY_COL_LINES_PTR(0,1)
        return 1;
    }
    buff->content << *x;
    EMPTY_COL_LINES_PTR(0,1)
    return 0;
}

void enter_a_b(Buff *buff, unsigned N, unsigned *a, unsigned *b)
{
    *buff <<  "a = " << right_input(0); 			
    enter_int(buff, a);
    while(*a > N - 3)
    {
        *buff <<  "Error! Try fitting it into this margin: {0,...," << N - 3 << "}" << right(0); 
        *buff <<  "a = " << right_input(0); 			
        enter_int(buff, a);
        EMPTY_COL_LINES_PTR(0,1)
    }
    *buff <<  "b = " << right_input(0); 			
    enter_int(buff, b);
    while(*b < *a || *b - *a < 2 || *b > N -1)
    {
        *buff <<  "Error! Try fitting it into this margin: {" << *a+2 << ",...," << N-1 << "}" << right(0); 
        *buff <<  "b = " << right_input(0); 			
        enter_int(buff, b);
        EMPTY_COL_LINES_PTR(0,1)
    }
}

void enter_x(Buff *buff, PointsArray Pts, double *x)
{
    *buff <<  "x should be in [" << Pts.p_arr[Pts.a].X << "..." << Pts.p_arr[Pts.b].X << "]" << right(0); 
    *buff <<  "x = " << right_input(0);
    buff->content << left;
    buff->content.width(buff->width_of_col(0) - 4);
    enter_double(buff, x);
    while(Pts.p_arr[Pts.a].X > *x || Pts.p_arr[Pts.b].X < *x)
    {
        *buff <<  "Error! This program is not designed for extrapolation :(" << right(0); 
        EMPTY_COL_LINES_PTR(0,1)
        *buff <<  "Try one more time" << right(0); 
        *buff <<  "X should be in x[" << Pts.p_arr[Pts.a].X << "..." << Pts.p_arr[Pts.b].X << "]" << right(0); 
        *buff <<  "x = " << right_input(0);
        buff->content << left;
        buff->content.width(buff->width_of_col(0) - 4);
        enter_double(buff, x);
    }
}

    ///////////    ///////////      ///////////     ///       ///     ///////////    
    ///      ///   ///      ///         ///         //////    ///         ///           
    ///////////    ///////////          ///         /// ///// ///         ///           
    ///            ///   ///            ///         ///   ///////         ///           
    ///            ///     ///      ///////////     ///       ///         ///   

void print_points(Buff *buff, vector<myPoint> p)
{
    unsigned width = 1;
    unsigned t = p.size();
    unsigned N = t;
    while(t/10 > 1){
        width++;
        t /= 10;
    }
    *buff << fixed;
    for(unsigned int i = 1; i <= N; i++)
    {
        *buff << "X[";
        buff->content.width(width);
        *buff << i-1 << "]=";
        buff->content.width(19);
        *buff << p[i-1].X << "  Y[";
        buff->content.width(width);
        *buff <<i-1<<"]=";
        buff->content.width(19);
        *buff << p[i-1].Y;
        *buff << right(0);
        if(i % buff->menu.left_height == 0)
            press_continue_right(buff);
    }
    *buff << defaultfloat;
    return;
}
// uses a,b, N
void print_points_ab(Buff *buff, PointsArray Pts)
{
    unsigned int width = 1;
    unsigned int t = Pts.b;
    while(t > 10){
        width++;
        t /= 10;
    }
    *buff << fixed;
    Pts.b += 1;
    for(unsigned int i = Pts.a + 1; i <= Pts.b; i++)
    {
        *buff << "X[";
        buff->content.width(width);
        *buff << i-1 << "]=";
        buff->content.width(19);
        *buff << Pts.p_arr[i-1].X << "  Y[";
        buff->content.width(width);
        *buff <<i-1<<"]=";
        buff->content.width(19);
        *buff << Pts.p_arr[i-1].Y;
        *buff << right(0); 
        if(i % buff->menu.left_height == 0)
            press_continue_right(buff);
    }
    Pts.b -= 1;
    *buff << defaultfloat;
    return;
}

void get_offset(Buff *buff, vector<myPoint> p, double x)
{
    unsigned int x_offset = 0;
    while(p[x_offset].X < x){x_offset++;};   
        x_offset--;
    *buff << "offset: " << fixed << "x[" << x_offset << "] < x" << right(1);
    *buff <<"(" << p[x_offset].X << " < " << x << ");" << right(1); 
}

/*void get_offset_old(Buff *buff, vector<myPoint> p)
{
    unsigned int x_offset = 0;
    while(p[x_offset].X < x){x_offset++;};   
        x_offset--;
    *buff << THIN_LINE << right; 
    EMPTY_COL_LINES_PTR(0,1)
    *buff << "offset: " << fixed << "x[" << x_offset << "] < x; (" 
        << p[x_offset].X << " < " << x << ")" << right; 
}*/

void print_result(Buff *buff, vector<Result> results, string buf)
{
    // Current array of found values:
    if( results.size() < 2 || (results.back().x != results[results.size() - 1].x))
    {
        *buff << "Found values: " << right(1);
        
    }
    else if(results.size() == 0)
    {
        *buff << "Empty" << right(1);
        return;
    }
    int i = 0;
    int size = results.size();

    if(results[size - 1].method_idx != results[size - 2].method_idx)
        *buff << "#" << buf << right(1);
    //*buff << results[size - i - 1].x << " " <<  results[size - i - 2].x << right(1);
    while(results[size - i - 1].x == results[size - i - 2].x ) 
        i++;
    
    *buff << "#" << buf << right(1);
    *buff << "y[" << i << "]=" << results.back().y << right(1); 
}
/*void print_results_old(Buff *buff, vector<Result> results)
{
    // Current array of found values:

    if(results.size() != 0)
    {
        *buff << "Found values: " << right; 
    }
    else 
    {
        *buff << "Empty" << right; 
    }
    {
        int i = 0;
        for (auto it = results.cbegin(); it != results.cend(); it++)
        {
            if(i == buff->menu.right_height)
            {
                i = 0;
                press_continue_right(buff);
            }
            *buff << "y[" << i++ << "]=" << results.front().y << right; 
        }
    }
}*/

void remove_results(Buff *buff, vector<Result> y, func_struct *func_mas )
{
    int i = 0;
    *buff << "Function: " << func_mas[y.front().func_idx].text << right(0);
    *buff << "Values:" << right(0);
    while(true)
    {
        *buff << "y[" << i << "] = " << y.front().y << right(0);

    }
    
}

void print_fun_error(Buff *buff)
{
    *buff << "Oups! Seems like we're off the function's limits..." << right(0); 
    *buff << "Let me try to fix it.." << right(0); 
    EMPTY_COL_LINES_PTR(0,1)
    cout.flush();
    usleep(2000000);
    *buff << "..." << right(0); 
    EMPTY_COL_LINES_PTR(0,1)
    cout.flush();
    usleep(2000000);
    *buff << "ERROR: FATAL!!!!!PREPARE FOR THE HUUUUUGE KABOOM! >_<" << right(0); 
    EMPTY_COL_LINES_PTR(0,1)
    cout.flush();
    usleep(3000000);
    print_shroom(buff);
    EMPTY_COL_LINES_PTR(0,2);
    *buff << " " << right(0); 
}

void print_one_millionth_chance_error(Buff *buff, int code)
{
    if(code == 2)
    {
        *buff <<  "This feeling when something with probability of 1/10000 hits!" << right(0); 
        *buff <<  "Now my programm won't work :( Bye bye!" << right(0);
        cout.flush();
        usleep(300000);
        //system("clear");
        *buff <<  "Nah, i'm kidding! :P" << right(0); 
        *buff <<  "I'll just set 'h' instead of 'flat 0' to be just a minimum allowed" << right(0);
        cout.flush();
        usleep(100000);
        *buff <<  "Here we go!" << right(0); 
        EMPTY_COL_LINES_PTR(0,1)
    }
    if (code == 1)
    {
        *buff <<  "How unlucky can this programm even be? It's been 10000 randomizing point_set() tries!" << right(0); 
        *buff <<  "Do you need more? Cmon.. " << right(0); 
    }
}
void print_shroom(Buff *buff)
{
    EMPTY_COL_LINES_PTR(0,5);
    *buff << "            _.-^^---....,,--.,     " << right(0); 
    *buff << "         _--                  `-_  " << right(0); 
    *buff << "        <                        > " << right(0); 
    *buff << "        |                         |" << right(0); 
    *buff << "        \\._                   _./ " << right(0); 
    *buff << "            ```--. . , ; .--'''    " << right(0); 
    *buff << "                  | |   |          " << right(0); 
    *buff << "               .-=||  | |=-.       " << right(0); 
    *buff << "               `-=#%%&%%#=-'       " << right(0); 
    *buff << "                  | ;  :|          " << right(0); 
    *buff << "         _____.,-#%%$@%%&#~,._____ " << right(0); 
    EMPTY_COL_LINES_PTR(0,3);
}

     ///////////      ///////       ///            ///       ///    ///       ///    ///       ///
    ///             ///     ///     ///            ///       ///    /////   /////    //////    ///
    ///            ///       ///    ///            ///       ///    /// ///// ///    /// ///// ///
    ///             ///     ///     ///             ///     ///     ///   //  ///    ///    //////
     ///////////      ///////       /////////////     ///////       ///       ///    ///       ///


void reset_right_column(deque<string> *str_dq, unsigned int rows, unsigned int first_line_offset)
{
    for(unsigned int i = 0; i < rows; i++)
    {
        str_dq->clear();
    }
    str_dq->resize(first_line_offset);
}

int arrange_plot(string data_path, string png_path, string func_name, double x_min, double x_max, double y_min, double y_max, short unsigned n_xtics, short unsigned n_ytics)
{
    fstream dat;
    fstream script;
    vector<string> word;
    string temp;
    if(n_ytics * n_xtics == 0)
    {
        return 1;
    }

    double period_x = (x_max - x_min) / n_xtics;
    double period_y = (y_max - y_min) / n_ytics;
    double period_tmp;
    short unsigned counter = 2;

    script.open("plot/plot_script.gnu", std::ofstream::out | std::ofstream::trunc);
    dat.open(data_path);
    getchar();
    if(!script.is_open())
    {
        return -1;
    }
    if(!dat.is_open())
    {
        return -2;
    }
    // Reading data file
    script << "load 'plot/plot_func.gnu'\n\n";
    script << "set output '" << png_path << "'\n\n";
    script << "# Legend\n";
    script << "set xrange[" << (x_min - period_x) << ":" << (x_max + period_x) << "]\n";
    script << "set yrange[" << (y_min - period_y) << ":" << (y_max + period_y) << "]\n\n";
    script << "# Axis labels\n";
    /*floor = static_cast<unsigned>(func.period / (PI/8));
    mod = func.period - PI/8 * floor;
    if(mod == 0)
    {
        char coeff[][3] = {"" , "/2", "/4", "/6", "/8"};
        script << "set format '$%g$' \n";
        script << "set xtics(";
        for(short unsigned i = 0; i < 10; i++)
        {
            script << "'" << coeff[i] << "П'";
        }
    } else script << "#set xtics 1\n"; */
    script << "#set format '$%g$'";
    script << "#set xtics ('$-2\\pi$' -2*pi, '$-\\pi$' -pi, 0, '$\\pi$' pi, '$2\\pi$' 2*pi)\n";
    script << "set xtics ";

    period_tmp = 1;
    if(period_x > 0.75 || period_x < 0.025) {
        while(period_tmp < period_x*0.5){
            period_tmp *= 10;
        }
        period_x = period_tmp ;
    }
    else if(period_x > 0.375)
        period_x = 0.5;
    else if(period_x > 0.125)
        period_x = 0.1;
    else if(period_x > 0.075)
        period_x = 0.05;
    script << period_x << "\n";

    script << "set ytics ";
    period_tmp = 1;
     if(period_y > 0.75 || period_y < 0.025) {
        while(period_tmp < period_y*0.5){
            period_tmp *= 10;
        }
        period_y = period_tmp;
    }
    else if(period_y > 0.375)
        period_y = 0.5;
    else if(period_y > 0.125)
        period_y = 0.1;
    else if(period_y > 0.075)
        period_y = 0.05;
    script << period_y << "\n";

    script << "set tics nomirror out scale 1\n";
    script << "set key outside\n";
    script << "\n";

    script << "# Function linestyles\n";
    script << "set style line 1 lc rgb 'black' pt 7 # Interpolation points linestyle\n";
    script << "set style line 2 linecolor rgb '#afafaf' linetype 1 linewidth 5 # Actual f(x) linestyle\n\n";
    script << "# Plot\n";
    script << "plot '"<< data_path << "' index 0 title 'Referring points' with points linestyle 1, \\\n";
    script << "'' index 1 title '" << func_name << "' with lines linestyle 2, \\\n";

    while(!dat.eof()) {
        getline(dat, temp);
        /*dat.read(word, 3);
        if(word[0] != '\0'){
            if( (word[2] == 'L') || (word[2] == 'E') || (word[2] == 'N') || (word[2] == 'C') || (word[2] == 'T') ) {
                short unsigned pos = dat.tellp();
                pos -= 3;
                dat.seekp(pos);
                getline(dat, temp);*/
                //
        if(temp[0] != '\0'){
            if( (temp[2] == 'L') || (temp[2] == 'E') || (temp[2] == 'N') || (temp[2] == 'C') || (temp[2] == 'T') ) {
                //
                unsigned length = temp.length() - 2;
                word.push_back(temp.substr(2, length));
                script << "'' index " << counter << " title '" << word[counter-2] << "' with lines linestyle " << counter + 1 << ", \\\n";
                counter++;
            }
        }
        //dat.ignore(150, '\n');
    }
    if(counter == 2)
        return 2;
    
    script << "\n# Interpolation linestyles:\n";

    script << "set style line 3 linecolor rgb '#dd181f' linetype 1 linewidth 1 \n";
    script << "set style line 4 linecolor rgb '#779922' linetype 1 linewidth 1\n";
    script << "set style line 5 linecolor rgb '#00ff1f' linetype 1 linewidth 1\n";
    script << "set style line 6 linecolor rgb '#ff1199' linetype 1 linewidth 1\n";
    script << "set style line 7 linecolor rgb '#cccc00' linetype 1 linewidth 1\n";
    return 0;
}

void extremum_from_points(vector<myPoint> p, unsigned a, unsigned b, double *y_max, double *y_min)
{
    *y_max = *y_min = p[0].Y;
    for(unsigned i = a; i < b; i++)
    {
        if(p[i].Y > *y_max) 
            *y_max = p[i].Y;
        else if(p[i].Y < *y_min) 
            *y_min = p[i].Y; 
    }
}