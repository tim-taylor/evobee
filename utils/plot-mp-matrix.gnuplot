set datafile separator ","
set xrange [290:660]
set yrange [660:290]
set title "Pollinator's confidence of MP match (HKS92N background)"
set size square
unset key
plot "mp-confidence-of-match-matrix-HKS92N.csv" u 1:2:3 with image

set term png
set output "mp-match-matrix.png"
replot

#############################


set datafile separator ","
set xrange [290:660]
set yrange [660:290]
set size square
set title "Pollinator's confidence that two MPs match (ALG background)"
unset key
plot 'mp-match-matrix.csv' u 1:2:3 with image

set term png
set output "mp-match-matrix.png"
replot

set term qt

#############################

set datafile separator ","
set xrange [290:660]
set yrange [660:290]
set title "Probability that two marker points are deemed to match (N=10,000)"
plot 'total-mp-match-matrix-10000.csv' u 1:2:($3/10000) with image

# can resize output window and export to image (png) from window menu
