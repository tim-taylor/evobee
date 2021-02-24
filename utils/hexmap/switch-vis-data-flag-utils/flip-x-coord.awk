{
    if ($1 ~ /\[[0-9]+/) {
        oldx=$5
        if (substr(oldx,1,1) == "-") {
            $5=substr(oldx,2)
        }
        else {
            $5="-"oldx
        }
    }
    print
}
