$0 !~ /int build/
/int build/ { print "    static private int build = " $6 + 1 " ;" }
