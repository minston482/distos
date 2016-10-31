#!/usr/bin/gnuplot
set key bottom
#set terminal wxt persist
set ylab "Execution time" font ", 20"
set xlab "Threads" font ", 20"
set term postscript enhanced eps 
set style fill solid
set xtics("2"0, "3"1, "4"2, "5"3, "6"4, "7"5, "8"6, "9"7, "10"8) font ", 15"
#set yrange[0:100]
set size 1.4, 1
set output "result_comp.eps"
plot "result_comp" u 1 t "Michael Q" w linespoints lt rgb 'black', ""u 2 t "Sandard Q" w linespoints lt rgb 'blue'
#plot "fig8_varmail" u 1 t "WB" w linespoints lt rgb 'gray', ""u 2 t "PDC" w linespoints lt rgb 'blue'

