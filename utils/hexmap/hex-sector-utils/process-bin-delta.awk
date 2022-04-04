# for N in 0 {99..2499..100}; do echo $N; cat binned-numflowers-across-runs-means-stds-w60-gen-$N.csv; done > all-bins.csv
#
# gawk -f process-bin-delta.awk all-bins.csv > bin-deltas.csv



BEGIN {
    FS=","
}

NF==1 {
    G=int($1)
    if (G>99) {
        print (G-100)", "T
    }
    T=0;
} 

NF==4 && G==0 {
    P[$1]=$3
} 

NF==4 && G>0 {
    if ($3>P[$1]) {
        T+=($3-P[$1])
    } 
    else {
        T+=(P[$1]-$3)
    }
    P[$1]=$3
}

END {
    print G", "T
}
