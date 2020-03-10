set datafile separator ","
set xrange [0:199]
set yrange [655:295]
set xlabel "Generation"
set ylabel "Plant Type (marker point)"
unset key
set title "Plant evolution (#individuals), fickle-circumspect, 36 refuges ordered, 180^2+20x100"
plot "36refuges-180x180-20x100-fickle-circumspect-200gens-2020-03-10-11-24-19-671504-log.txt-all-summary.csv" with image

set term png
set output "plant-type-evol-fickle-circumspect-36refuges-ordered.png"
replot
# can resize output window and export to image (png) from window menu
set term qt


#plot for [i=300:650:10] "36refuges-180x180-20x100-stay-2020-03-09-17-57-33-063627-log.txt-".i.".csv" using 1:5 with lines
#plot 'total-mp-match-matrix-10000.csv' u 1:2:($3/10000) with image


