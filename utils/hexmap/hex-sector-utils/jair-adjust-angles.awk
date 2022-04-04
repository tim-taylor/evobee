BEGIN {
    FS=","
}

{
    for (i=1; i<=NF; i++) {
        TB=$i; 
        if (TB<90.0) {
            TO=120.0-TB
        } else {
            TO=480.0-TB
        }
        if (TO>360.0) {
            TO-=360.0
        }
        printf "%.3f", TO
        if (i<NF) {
            printf ","
        } else {
            printf "\n"
        }
    }
}
