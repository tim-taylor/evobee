set datafile separator ","
set yrange [-0.7:0.7]
set xrange [-0.7:0.7]
set style circle radius graph 0.005
set grid
set size square
plot "myhexmap_HKS92N_mp_u_b_g_x_y.csv" using 5:6 with circles title "HKS92N", "myhexmap_ALG_mp_u_b_g_x_y.csv" using 5:6 with circles title "ALG"


plot "hexmap-dyer-param-R5-A375-B500.csv" using 5:6 with circles title "+0nm", "hexmap-dyer-param-R5-A400-B525.csv" using 5:6 with circles title "+25nm", "hexmap-dyer-param-R5-A425-B550.csv" using 5:6 with circles title "+50nm"

set term png
set output "hexmap-dyer-param.png"
replot

set term qt
