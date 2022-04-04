infile="ausdata-minus-orchids-gc-detectability-by-hex-sector-inc-0p225.csv"

set datafile separator ","

set size square
set title font ",13"
set xlabel font ",12"
set xlabel "Hexagon sector"
set ylabel font ",12"
set ylabel "Plant species count"
set xtics font ",9"
set xtics nomirror out 20 rotate by -60
set ytics nomirror out
set style data histograms
set style histogram rowstacked
set style fill solid 1.0 border -1

set boxwidth 0.8 relative
#set boxwidth 10 absolute

set auto x
#set xrange[-85:275]
set yrange[0:40]

# the following function is from https://groups.google.com/g/comp.graphics.apps.gnuplot/c/M_ldKLSFXwg
nth(countCol,labelCol,n) = ((int(column(countCol)) % n == 0) ? stringcolumn(labelCol) : "")

set lt 1 lc rgb 'blue'
set lt 2 lc rgb 'red'

# set term qt

set term png size 800,800

set output "detectability-gcTH-0p10.png"
set title "Green Contrast threshold 0.10"
plot infile using 4 t "Detectable", infile using 5:xtic(nth(2,3,10)) t "Undetectable"

set output "detectability-gcTH-0p15.png"
set title "Green Contrast threshold 0.15"
plot infile using 8 t "Detectable", infile using 9:xtic(nth(2,3,10)) t "Undetectable"

set output "detectability-gcTH-0p20.png"
set title "Green Contrast threshold 0.20"
plot infile using 12 t "Detectable", infile using 13:xtic(nth(2,3,10)) t "Undetectable"

set output "detectability-gcTH-0p225.png"
set title "Green Contrast threshold 0.225"
plot infile using 16 t "Detectable", infile using 17:xtic(nth(2,3,10)) t "Undetectable"

set output "detectability-gcTH-0p25.png"
set title "Green Contrast threshold 0.25"
plot infile using 20 t "Detectable", infile using 21:xtic(nth(2,3,10)) t "Undetectable"
