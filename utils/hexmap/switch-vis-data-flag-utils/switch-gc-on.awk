{
    if ($1 ~ /\[[0-9]+/) {
        if ($4>=0.4 && $4<=0.6) {
            $3="0,"
        }
    }
    print
}
