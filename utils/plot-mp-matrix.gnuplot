set datafile separator ","
plot 'mp-match-matrix.csv' u 1:2:3 with image

set term png
set output "mp-match-matrix.png"
replot

set term qt
