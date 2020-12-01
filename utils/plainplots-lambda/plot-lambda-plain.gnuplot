# Gnuplot scipt to generate a graph of the distribution of dominant frequencies for a given run
# at a given point in time
#
# Call like this, e.g.
# > gnuplot -e "filebase='myfilenamebase'" plot-lambda-plain.gnuplot
#
# The script looks for an input data file named [filebase].csv
#
# The output is saved in a file named [filebase].png

infile = sprintf("%s.csv", filebase)
outfile = sprintf("%s.png", filebase)

set datafile separator ","
set size square
set title font ",9"
set title infile
set xrange [355:565]
set yrange [0:23250]
set xtics nomirror out 20
set ytics nomirror out
unset key
set term png
set output outfile
plot infile using 1:2 with points pointtype 7
