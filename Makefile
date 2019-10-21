#GDB version
#main: obj bin main.o functions.o methods.o term.o gauss.o plot.o \
	g++ -ggdb3 -Wall -o ./bin/main  ./obj/functions.o ./obj/interpolationMethods.o ./obj/termCommands.o ./obj/twoColMain.o ./obj/gauss.o ./obj/makePlot.o \
\
main.o: ./src/twoColMain.cpp \
	g++ -ggdb3 -c -Wall -o ./obj/twoColMain.o ./src/twoColMain.cpp \
\
functions.o: ./src/functions.cpp \
	g++ -ggdb3 -c -Wall -o ./obj/functions.o ./src/functions.cpp \
 \
methods.o: ./src/interpolationMethods.cpp \
	g++ -ggdb3 -c -Wall -o ./obj/interpolationMethods.o ./src/interpolationMethods.cpp \
\
term.o: ./src/termCommands.cpp \
	g++ -ggdb3 -c -Wall -o ./obj/termCommands.o ./src/termCommands.cpp \
\
gauss.o: ./src/gauss.cpp \
	g++ -ggdb3 -c -Wall -o ./obj/gauss.o ./src/gauss.cpp \
\
plot.o: ./src/makePlot.cpp \
	g++ -ggdb3 -c -Wall -o ./obj/makePlot.o ./src/makePlot.cpp
# Normal version
main: obj bin main.o functions.o methods.o term.o gauss.o plot.o 
	g++ -Wall -o ./bin/main  ./obj/functions.o ./obj/interpolationMethods.o ./obj/termCommands.o \
	 ./obj/main.o ./obj/gauss.o ./obj/makePlot.o 
main.o: ./src/InterpolationMain.cpp
	g++ -c -Wall -o ./obj/main.o ./src/InterpolationMain.cpp 

functions.o: ./src/functions.cpp
	g++ -c -Wall -o ./obj/functions.o ./src/functions.cpp

methods.o: ./src/interpolationMethods.cpp
	g++ -c -Wall -o ./obj/interpolationMethods.o ./src/interpolationMethods.cpp

term.o: ./src/termCommands.cpp
	g++ -c -Wall -o ./obj/termCommands.o ./src/termCommands.cpp

gauss.o: ./src/gauss.cpp
	g++ -c -Wall -o ./obj/gauss.o ./src/gauss.cpp

plot.o: ./src/makePlot.cpp
	g++ -c -Wall -o ./obj/makePlot.o ./src/makePlot.cpp

bin:
	mkdir ./bin

obj:
	mkdir ./obj	

.PHONY: clean
clean:
	rm -rf bin/ obj/
.PHONY: run
run:
	./bin/main
