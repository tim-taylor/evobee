# Gnuplot script to generate a visualisation of the distribution of flowers in the environment at a
# given point in time, using log data generated using the "F" flag (logFlowersInterPhaseFull).
#
# call this with, e.g.
# > gnuplot -e "gen=0;filebase='flower-positions';envsize=240" patch-visualisation.gnuplot
# or
# > for G in `seq 0 10 90`; do gnuplot -e "gen=$G;filebase='flower-positions'" patch-visualisation.gnuplot; done
#
# with a specified filename base "filebase" and a generation number "gen", the script expects to find
# an input file called "filebase"-g"gen".csv
#
# The output is saved to a file called "filebase"-g"gen".png
#
# The input files can be generated from the raw log files as follows:
#
# for G in `seq 0 10 99`; do gawk -F',' -vG=$G '$1=="F" {if ($2==G) {print $5 "," $6 "," $4}}' run-log.txt > flower-positions-g$G.csv; done
#

if (!exists("gen")) gen=0
if (!exists("filebase")) filebase="flower-positions"
if (!exists("envsize")) envsize=240

mytitle = sprintf("Flower distribution at generation %d", gen)
datafile = sprintf("%s-g%d.csv", filebase, gen)
outfile = sprintf("%s-g%d.png", filebase, gen)

set terminal png size 1000,1000
set output outfile

set size square
set datafile separator ","
set xrange [0:envsize]
set yrange [envsize:0]
set title mytitle
unset key
set xtics 20
set ytics 20

set linetype 1 lc rgb "#0000FF"
set linetype 2 lc rgb "#070FF8"
set linetype 3 lc rgb "#0F1EF0"
set linetype 4 lc rgb "#162DE9"
set linetype 5 lc rgb "#1D3CE2"
set linetype 6 lc rgb "#244BDB"
set linetype 7 lc rgb "#2C5AD3"
set linetype 8 lc rgb "#3369CC"
set linetype 9 lc rgb "#3A78C5"
set linetype 10 lc rgb "#4287BD"
set linetype 11 lc rgb "#4996B6"
set linetype 12 lc rgb "#50A5AF"
set linetype 13 lc rgb "#57B4A8"
set linetype 14 lc rgb "#5FC3A0"
set linetype 15 lc rgb "#66D299"
set linetype 16 lc rgb "#6DE192"
set linetype 17 lc rgb "#75F08A"
set linetype 18 lc rgb "#7CFF83"
set linetype 19 lc rgb "#83FF7C"
set linetype 20 lc rgb "#8AF075"
set linetype 21 lc rgb "#92E16D"
set linetype 22 lc rgb "#99D266"
set linetype 23 lc rgb "#A0C35F"
set linetype 24 lc rgb "#A8B457"
set linetype 25 lc rgb "#AFA550"
set linetype 26 lc rgb "#B69649"
set linetype 27 lc rgb "#BD8742"
set linetype 28 lc rgb "#C5783A"
set linetype 29 lc rgb "#CC6933"
set linetype 30 lc rgb "#D35A2C"
set linetype 31 lc rgb "#DB4B24"
set linetype 32 lc rgb "#E23C1D"
set linetype 33 lc rgb "#E92D16"
set linetype 34 lc rgb "#F01E0F"
set linetype 35 lc rgb "#F80F07"
set linetype 36 lc rgb "#FF0000"

plot datafile using 1:2:3 with points pt 7 ps 0.5 lc variable
