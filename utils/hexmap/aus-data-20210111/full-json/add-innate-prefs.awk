BEGIN {
    FS=","
}

{
    if (NF==10||NF==11) {
        ID=$1
        gsub(/[\[ ]/,"",ID)
        AID=$10
        gsub(/[\] ]/,"",AID)
        cmd="grep \"^" AID ",\" aus-giurfa-prefs.csv"
        while ( (cmd | getline x) > 0) {
            PREF=x
        };
        gsub(/.*,/,"",PREF)
        gsub(/ /,"",PREF)
        PREF=PREF/100
        $9=" "PREF
        close(cmd)
        if (ID!=81) {
            $10=$10","
        }
        gsub(/  /,", ",$0)
        print
    } 
    else {
        print
    }
}
