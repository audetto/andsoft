import java.io.*;

import java.util.*;
import java.util.zip.*;

import java.awt.event.*;
import java.awt.datatransfer.*;

import javax.mail.*;
import javax.mail.internet.*;

import javax.swing.*;
import javax.swing.table.*;

class SourceTransferHandler extends TransferHandler {
    protected Transferable createTransferable(JComponent c) {
	JTable jtable = (JTable)c;
	TableModel tm = jtable.getModel();
	int[] rows = jtable.getSelectedRows();
	Arrays.sort(rows); // forse inutile
	return new MsgTransferable(new MsgSelection(rows, tm));
    }
    public int getSourceActions(JComponent c) {
	return TransferHandler.MOVE;
    }
    public void exportDone(JComponent source, Transferable data, int action) {
	if (data != null) {
	    try {
		MsgSelection msgsel = (MsgSelection)data.getTransferData(MsgTransferable.MSG_FLAVOR);
		FilesTableModel ftm = (FilesTableModel)msgsel.tm;
		File dir = ftm.getDir();
		for (int i = msgsel.rows.length - 1; i >= 0; i--) {
		    ftm.removeRow(msgsel.rows[i]);
		}
	    } catch(Exception ex) {
		LogFrame.log(ex);
	    }
	}
    }
    public boolean canImport(JComponent comp, DataFlavor[] targetFlavors) {
	return false;
    }
    public boolean importData(JComponent comp, Transferable t) {
	return false;
    }
}

class FilesTableModel extends DefaultTableModel {
    private Class[] classi = {String.class, String.class, Number.class, String.class, Date.class};
    private String[] headers = {"From", "Subject", "Size", "Filename", "Sent"};
    private TreeSet[] ts = new TreeSet[5];
    private File dir;

    private int cur_col = 10;

    public FilesTableModel(File dir) {
	this.dir = dir;
	for (int i = 0; i < ts.length; i++)
	    ts[i] = new TreeSet(new ArrayCompare(i));
    }

    public File getDir() {
	return dir;
    }
    public int getColumnCount() {
	return headers.length;
    }
    public String getColumnName(int col) {
	return headers[col];
    }
    public Class getColumnClass(int col) {
	return classi[col];
    }
    public boolean isCellEditable(int row, int col) {
	return false;
    }
    public void addRow(Vector rowData) {
	for (int i = 0; i < ts.length; i++)
	    ts[i].add(rowData);
	super.addRow(rowData);
    }
    public void removeRow(int row) {
	String fileName = (String)getValueAt(row, 3);
	File sourceFile = new File(dir, fileName);
	sourceFile.delete();
	
	Object rowData = dataVector.elementAt(row);
	for (int i = 0; i < ts.length; i++)
	    ts[i].remove(rowData);
	super.removeRow(row);	
    }
    public void sort(int col) {
	if (col < ts.length) {
	    if (cur_col != col) {
		Object[] rowData = ts[col].toArray();
		dataVector = convertToVector(rowData);
		cur_col = col;
	    } else {
		Collections.reverse(dataVector);
	    }
	    fireTableRowsUpdated(0, getRowCount() - 1);
	}
    } 

    public static Vector processMessage(Message msg, String fileName) throws MessagingException, IOException {
	Vector dati = new Vector(5);

	Address[] from = msg.getFrom();
	if (from != null && from.length > 0) {
	    InternetAddress ia = (InternetAddress)from[0];
	    dati.add(ia.toUnicodeString());
	}

	dati.add(msg.getSubject());
   	dati.add(new Integer(msg.getSize()));
	dati.add(fileName);
	dati.add(msg.getSentDate());

	return dati;
    }

    public static JTable createFilesTable(final JMenu windowMenu, final JInternalFrame parent) {
	final JTable jtab = new JTable();
	
	jtab.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
	jtab.setTransferHandler(new SourceTransferHandler());
	jtab.setDragEnabled(true);
	jtab.addMouseListener(new MouseAdapter() {
		private MouseEvent me = null;

		public void mouseClicked(MouseEvent e) {
		    if (e.getClickCount() == 2) {
			int row = jtab.rowAtPoint(e.getPoint());
			if (row != -1) {
			    try {
				FilesTableModel ftm = (FilesTableModel)jtab.getModel();
				String fileName = (String)ftm.getValueAt(row, 3);
				File file = new File(ftm.dir, fileName);
				MimeMessage msg = new MimeMessage(Options.session, new GZIPInputStream(new BufferedInputStream(new FileInputStream(file))));
				MessageViewer msgView = new MessageViewer(msg, windowMenu);
				msgView.setSize(640, 480);
				msgView.show();
				parent.getDesktopPane().add(msgView);
				msgView.setSelected(true);
			    } catch(Exception ex) {
				LogFrame.log(ex);
			    }
			}
		    }
		}
	    }
			    );

	jtab.getTableHeader().addMouseListener(new MouseAdapter() {
		public void mouseClicked(MouseEvent e) {
		    if (e.getClickCount() == 1) {
			int col = jtab.columnAtPoint(e.getPoint());
			if (col != -1)
			    ((FilesTableModel)jtab.getModel()).sort(col);
		    }
		}
	    }
					      );
	
	return jtab;
    }

}
