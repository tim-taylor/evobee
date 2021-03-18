set datafile separator ","
set size square
set title font ",8"
set title "XXX"
set xtics nomirror out 20
set xtics font ",7"
set ytics nomirror out
set boxwidth 6 absolute
set style fill solid 1.0 border lc "gray30"
unset key
set xrange[0:365]
set yrange[0:3000]
plot "hex-sector-bin-means-stds-gen-49.csv" using ($1+10):3:4 with boxerrorbars fillcolor "gray70"
