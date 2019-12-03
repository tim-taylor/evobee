#set terminal pngcairo dashed enhanced
set terminal png size 1000,1000
set output "all-pollen-distributions.png"
set datafile separator ","
set multiplot layout 36,1 margins 0.04, 0.995, 0.04, 0.995 spacing 0.0
set boxwidth 0.75 relative
set style fill solid 1.0
set xrange [290:660]
#set yrange [0:1250]
set yrange [0:5500]
unset xtics
unset ytics
unset xlabel
unset ylabel
unset key
set ylabel "300" rotate by 0
plot "all-pollen-distrib-totals-300.csv" using 1:2 with boxes
set ylabel "310" rotate by 0
plot "all-pollen-distrib-totals-310.csv" using 1:2 with boxes
set ylabel "320" rotate by 0
plot "all-pollen-distrib-totals-320.csv" using 1:2 with boxes
set ylabel "330" rotate by 0
plot "all-pollen-distrib-totals-330.csv" using 1:2 with boxes
set ylabel "340" rotate by 0
plot "all-pollen-distrib-totals-340.csv" using 1:2 with boxes
set ylabel "350" rotate by 0
plot "all-pollen-distrib-totals-350.csv" using 1:2 with boxes
set ylabel "360" rotate by 0
plot "all-pollen-distrib-totals-360.csv" using 1:2 with boxes
set ylabel "370" rotate by 0
plot "all-pollen-distrib-totals-370.csv" using 1:2 with boxes
set ylabel "380" rotate by 0
plot "all-pollen-distrib-totals-380.csv" using 1:2 with boxes
set ylabel "390" rotate by 0
plot "all-pollen-distrib-totals-390.csv" using 1:2 with boxes
set ylabel "400" rotate by 0
plot "all-pollen-distrib-totals-400.csv" using 1:2 with boxes
set ylabel "410" rotate by 0
plot "all-pollen-distrib-totals-410.csv" using 1:2 with boxes
set ylabel "420" rotate by 0
plot "all-pollen-distrib-totals-420.csv" using 1:2 with boxes
set ylabel "430" rotate by 0
plot "all-pollen-distrib-totals-430.csv" using 1:2 with boxes
set ylabel "440" rotate by 0
plot "all-pollen-distrib-totals-440.csv" using 1:2 with boxes
set ylabel "450" rotate by 0
plot "all-pollen-distrib-totals-450.csv" using 1:2 with boxes
set ylabel "460" rotate by 0
plot "all-pollen-distrib-totals-460.csv" using 1:2 with boxes
set ylabel "470" rotate by 0
plot "all-pollen-distrib-totals-470.csv" using 1:2 with boxes
set ylabel "480" rotate by 0
plot "all-pollen-distrib-totals-480.csv" using 1:2 with boxes
set ylabel "490" rotate by 0
plot "all-pollen-distrib-totals-490.csv" using 1:2 with boxes
set ylabel "500" rotate by 0
plot "all-pollen-distrib-totals-500.csv" using 1:2 with boxes
set ylabel "510" rotate by 0
plot "all-pollen-distrib-totals-510.csv" using 1:2 with boxes
set ylabel "520" rotate by 0
plot "all-pollen-distrib-totals-520.csv" using 1:2 with boxes
set ylabel "530" rotate by 0
plot "all-pollen-distrib-totals-530.csv" using 1:2 with boxes
set ylabel "540" rotate by 0
plot "all-pollen-distrib-totals-540.csv" using 1:2 with boxes
set ylabel "550" rotate by 0
plot "all-pollen-distrib-totals-550.csv" using 1:2 with boxes
set ylabel "560" rotate by 0
plot "all-pollen-distrib-totals-560.csv" using 1:2 with boxes
set ylabel "570" rotate by 0
plot "all-pollen-distrib-totals-570.csv" using 1:2 with boxes
set ylabel "580" rotate by 0
plot "all-pollen-distrib-totals-580.csv" using 1:2 with boxes
set ylabel "590" rotate by 0
plot "all-pollen-distrib-totals-590.csv" using 1:2 with boxes
set ylabel "600" rotate by 0
plot "all-pollen-distrib-totals-600.csv" using 1:2 with boxes
set ylabel "610" rotate by 0
plot "all-pollen-distrib-totals-610.csv" using 1:2 with boxes
set ylabel "620" rotate by 0
plot "all-pollen-distrib-totals-620.csv" using 1:2 with boxes
set ylabel "630" rotate by 0
plot "all-pollen-distrib-totals-630.csv" using 1:2 with boxes
set ylabel "640" rotate by 0
plot "all-pollen-distrib-totals-640.csv" using 1:2 with boxes
set ylabel "650" rotate by 0
set xtics nomirror out
plot "all-pollen-distrib-totals-650.csv" using 1:2 with boxes
unset multiplot
unset output
