BEGIN {
    FS=","
}

{
    if ($1 ~ /\[/ && NF>=10) {
        cmd="/home/tim/evobee-hexmap/calc-dom-lambda -v "$5" "$6
        cmd | getline CDL
        close(cmd)
        split(CDL, CDLARY)
        DLR=CDLARY[1]
        DLR=DLR+0.5
        split(DLR,DLA,".")
        DL=DLA[1]
        if (DL>0) {
            printf("%s, %d,%s,%s,%s,%s,%s,%s,%s,%s",$1,DL,$3,$4,$5,$6,$7,$8,$9,$10)
            if (N<80) printf(",")
            printf("\n")
        }
        N++
    }
    else {
        print
    }
}
