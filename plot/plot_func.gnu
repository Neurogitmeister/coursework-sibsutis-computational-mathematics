reset
# png
set terminal pngcairo size 1280, 720 enhanced font 'Verdana,10'
#set terminal epslatex size 3.5,2.62 standalone color colortext 10

#set output 'terminal_epslatex.tex'

set border linewidth 1.5
# Axes label 
set xlabel 'x'
set ylabel 'y'

# grid
set tics nomirror out scale 1
set style line 12 lc rgb'#808080' lt 0 lw 1
set grid back ls 12
set grid xtics ytics mxtics
