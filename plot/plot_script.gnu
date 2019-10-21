load 'plot/plot_func.gnu'

set output 'plot/images/plot_img1.png'

# Legend
set xrange[1.64:3.56]
set yrange[-0.970123:6.74323]

# Axis labels
#set format '$%g$'#set xtics ('$-2\pi$' -2*pi, '$-\pi$' -pi, 0, '$\pi$' pi, '$2\pi$' 2*pi)
set xtics 0.1
set ytics 0.5
set tics nomirror out scale 1
set key outside

# Function linestyles
set style line 1 lc rgb 'black' pt 7 # Interpolation points linestyle
set style line 2 linecolor rgb '#afafaf' linetype 1 linewidth 5 # Actual f(x) linestyle

# Plot
plot 'plot/datafiles/plotdat.txt' index 0 title 'Referring points' with points linestyle 1, \
'' index 1 title 'f(x) = exp( 40/(10 + x)) * sin(x)' with lines linestyle 2, \
'' index 2 title 'Lagrange ' with lines linestyle 3, \
'' index 3 title 'Eitken ' with lines linestyle 4, \

# Interpolation linestyles:
set style line 3 linecolor rgb '#dd181f' linetype 1 linewidth 1 
set style line 4 linecolor rgb '#779922' linetype 1 linewidth 1
set style line 5 linecolor rgb '#00ff1f' linetype 1 linewidth 1
set style line 6 linecolor rgb '#ff1199' linetype 1 linewidth 1
set style line 7 linecolor rgb '#cccc00' linetype 1 linewidth 1
