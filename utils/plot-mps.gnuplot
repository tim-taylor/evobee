set datafile separator ","
set yrange [0:1250]

set xlabel "Marker Point"
set ylabel "Flower Count"


plot "mp-counts-gen-0.csv" using 1:2 with lines, "mp-counts-gen-1.csv" using 1:2 with lines, "mp-counts-gen-2.csv" using 1:2 with lines, "mp-counts-gen-3.csv" using 1:2 with lines, "mp-counts-gen-4.csv" using 1:2 with lines


plot "mp-counts-gen-0.csv" using 1:2 with lines, "mp-counts-gen-10.csv" using 1:2 with lines, "mp-counts-gen-20.csv" using 1:2 with lines, "mp-counts-gen-30.csv" using 1:2 with lines, "mp-counts-gen-40.csv" using 1:2 with lines


set term png
set output "mp-count-evolution-graph.png"
replot

set term qt
