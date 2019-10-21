#include "makePlot.h"


void makeplot()
{
    // ready to make a plot
    pid_t childpid=fork();
    if(childpid==0) {
        // child process makes plot
        std::FILE* pipehandle=popen("gnuplot -persist","w");
        // make some plot. You can send multiple commands to the pipe each ending in \n
        std::fprintf(pipehandle,"plot \"results.txt\" using 1:2 with lines\n");
        std::fprintf(pipehandle,"quit\n");
        std::fflush(pipehandle);
        std::fclose(pipehandle);
        // child process exits
        exit(0);
    }
    // parent process waits for child process to exit
    waitpid(childpid,NULL,0);

    // you can now repeat to make other gnuplots; all will appear simultaneously in the 
    // terminal and are persistent after the parent process has finished.
}
