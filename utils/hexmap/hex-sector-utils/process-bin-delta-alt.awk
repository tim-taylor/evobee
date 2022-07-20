#for N in 3899 3999 4899 4999 5899 5999; do echo $N; cat binned-numflowers-across-runs-means-stds-w60-gen-$N.csv; done > all-bins-check.csv
#
#for N in 899 999 1899 1999 2899 2999 3899 3999 4899 4999 5899 5999; do echo $N; cat binned-numflowers-across-runs-means-stds-w60-gen-$N.csv; done > all-bins-check.csv    
#    
#gawk -f /home/tim/evobee/code/evobee/utils/hexmap/hex-sector-utils/process-bin-delta-alt.awk all-bins-check.csv |grep 999 > bin-deltas-check.csv
#

BEGIN {
    FS=","
}

NF==1 {
    G=int($1)
    T=0;
} 

NF==4 && G>0 {
    if ($3>P[$1]) {
        T+=($3-P[$1])
    } 
    else {
        T+=(P[$1]-$3)
    }
    P[$1]=$3
    if ($1==5) {
        print G", "T
    }
}
