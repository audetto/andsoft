import java.util.*;

class ArrayCompare implements Comparator {
    private int col;
    public ArrayCompare(int col) {
	this.col = col;
    }
    public int compare(Object o1, Object o2) {
	Vector v1 = (Vector)o1;
	Vector v2 = (Vector)o2;
	Comparable c1 = (Comparable)v1.elementAt(col);
	Comparable c2 = (Comparable)v2.elementAt(col);

	int res;
	if (c1 == null) {
	    if (c2 == null)
		res = 0;
	    else
		res = -1;
	} else {
	    if (c2 == null)
		res = 1;
	    else
		res = c1.compareTo(c2);
	}
	
	if (res == 0) {
	    Comparable f1 = (Comparable)v1.elementAt(3);
	    Comparable f2 = (Comparable)v2.elementAt(3);
	    res = f1.compareTo(f2);
	}
	
	return res;
   }
}
