n=250 #number of intervals
max=50. #max value
min=0. #min value
width=(max-min)/n #interval width
hist(x,width)=width*floor(x/width)+width/2.0
set style fill solid 0.5 # fill style
set xrange [min:max]
set yrange [0:]
plot "levy.dat" u (hist($1,width)):(1.0) smooth freq w boxes lc rgb"green" notitle
