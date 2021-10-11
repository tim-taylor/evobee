# Check out the following great resources:
# http://www.gnuplotting.org/tag/palette/
# https://github.com/Gnuplotting/gnuplot-configs
# https://github.com/Gnuplotting/gnuplot-palettes

set datafile separator ","

xshift=5
set xlabel font ",11"
set xlabel "Hexagon sector"
set ylabel font ",11"
set ylabel "Plant count (normalised)"
set xrange[-85:275]
set yrange[0:0.55]

set title font ",12"
set title "Flower distributions run C01 (Aus data ex orchids)\nGiurfa prefs (honeybee), GC active, LC active)"
set title "Flower distributions run C17 (Aus data ex orchids)\nL-and-W prefs (hoverfly), GC inactive, LC active)"

#set term png size 800,600
#set output "distributions-C01.png"


# line styles for ColorBrewer Dark2
# for use with qualitative/categorical data
# provides 8 dark colors based on Set2
# compatible with gnuplot >=4.2
# author: Anna Schneider

# line styles
set style line 1 lt 1 lc rgb '#1B9E77' # dark teal
set style line 2 lt 1 lc rgb '#D95F02' # dark orange
set style line 3 lt 1 lc rgb '#7570B3' # dark lilac
set style line 4 lt 1 lc rgb '#E7298A' # dark magenta
set style line 5 lt 1 lc rgb '#66A61E' # dark lime green
set style line 6 lt 1 lc rgb '#E6AB02' # dark banana
set style line 7 lt 1 lc rgb '#A6761D' # dark tan
set style line 8 lt 1 lc rgb '#666666' # dark gray

# palette
set palette maxcolors 8
set palette defined ( 0 '#1B9E77', 1 '#D95F02', 2 '#7570B3', 3 '#E7298A', 4 '#66A61E', 5 '#E6AB02', 6 '#A6761D', 7 '#666666' )

plot "flower-distribution-data.csv" using (($1*10)+xshift)-(int($1/27)*360):4 w lp ls 1 lw 2 title "Burd et al 2014 ex orchids normalised", "flower-distribution-data.csv" using (($1*10)+xshift)-(int($1/27)*360):7 w lp ls 2 lw 2 title "Run C01 gen 1 normalised", "flower-distribution-data.csv" using (($1*10)+xshift)-(int($1/27)*360):10 w lp ls 3 lw 2 title "Run C01 gen 250 normalised"

plot "flower-distribution-data.csv" using (($1*10)+xshift)-(int($1/27)*360):19 w lp ls 1 lw 2 title "MI+SR19 normalised", "flower-distribution-data.csv" using (($1*10)+xshift)-(int($1/27)*360):13 w lp ls 2 lw 2 title "Run C17 gen 1 normalised", "flower-distribution-data.csv" using (($1*10)+xshift)-(int($1/27)*360):16 w lp ls 3 lw 2 title "Run C17 gen 250 normalised"
