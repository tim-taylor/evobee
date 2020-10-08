set datafile separator ","
set yrange [-0.7:0.7]
set xrange [-0.7:0.7]
set style circle radius graph 0.01
set grid
plot "myhexmap_HKS92N_mp_u_b_g_x_y.csv" using 5:6 with circles title "HKS92N", "myhexmap_ALG_mp_u_b_g_x_y.csv" using 5:6 with circles title "ALG"
