import javax.swing.table.*;

class MsgSelection {
    public int[] rows;
    public TableModel tm;
    MsgSelection(int[] rows, TableModel tm) {
	this.rows = rows;
	this.tm = tm;
    }
}
