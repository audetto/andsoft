import java.util.*;
import java.text.*;

import java.awt.*;
import java.awt.event.*;

import javax.mail.*;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;

class POP3TableModel extends DefaultTableModel {
    Class[] classi = {String.class, String.class, Number.class, String.class, Date.class, Boolean.class, Boolean.class, String.class};
    String[] headers = {"From", "Subject", "Size", "UIDL", "Sent", "Get", "Del", "Msg"};
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
	return (col == 5 || col == 6);
    }
    public Message getMessage(int row) {
	return (Message)((Vector)dataVector.elementAt(row)).get(7);
    }
}

class POP3Frame extends JWindowFrame {

    SwingWorker sw;

    public POP3Frame(JMenu windowMenu, String account, String hostname, String username, String password, boolean ssl) {
	super(windowMenu, account, true, false, true, true);

	addInternalFrameListener(new InternalFrameAdapter() {
		public void internalFrameClosed(InternalFrameEvent e) {
		    Object[] res = (Object[])sw.get();
		    // controllare che res potrebbe essere null
		    Store store = (Store)res[0];
		    Folder inbox = (Folder)res[1];
		    try {
			if (inbox != null) inbox.close(true);
			if (store != null) store.close();
		    } catch(Exception ex) {
			LogFrame.log(ex);
		    }
		}
	    }
				 );

	final POP3TableModel dtm = new POP3TableModel();

	final JTable jt = new JTable(dtm);

	JScrollPane jsp = new JScrollPane(jt);
	add(jsp, BorderLayout.CENTER);

	final JProgressBar jpb_count = new JProgressBar();
	add(jpb_count, BorderLayout.NORTH);

	final JProgressBar jpb_size = new JProgressBar();
	jpb_size.setStringPainted(true);
	jpb_size.setString("0 Kbytes");
	add(jpb_size, BorderLayout.SOUTH);

	sw = new ReadPOP3(hostname, username, password, ssl, dtm, jpb_count, this);
	sw.start();

	final NumberFormat format = NumberFormat.getIntegerInstance();
	final TableModelListener tml = new TableModelListener() {
		public void tableChanged(TableModelEvent e) {
		    double totsize = 0;
		    double getsize = 0;
		    int n = dtm.getRowCount();
		    for (int i = 0; i < n; i++) {
			boolean get = ((Boolean)dtm.getValueAt(i, 5)).booleanValue();
			double size = ((Integer)dtm.getValueAt(i, 2)).intValue();
			totsize += size;
			if (get)
			    getsize += size;
		    }
		    jpb_size.setMaximum((int)totsize);
		    jpb_size.setValue((int)getsize);
		    jpb_size.setString(new StringBuffer(format.format(getsize / 1024)).append(" Kbytes").toString());
		}
	    };

	dtm.addTableModelListener(tml);

	JMenuBar jmb = new JMenuBar();
	setJMenuBar(jmb);

	JMenu mPOP3 = new JMenu("POP3");
	jmb.add(mPOP3);

	final JMenuItem mDoIt = new JMenuItem("Do it");
	mPOP3.add(mDoIt);
	mDoIt.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    Object[] res = (Object[])sw.get();
		    Store store = (Store)res[0];
		    Folder inbox = (Folder)res[1];
		    
		    mDoIt.setEnabled(false);
		    dtm.removeTableModelListener(tml);
		    
		    int n = dtm.getRowCount();
		    Message messages[] = new Message[n];
		    boolean gets[] = new boolean[n];
		    boolean dels[] = new boolean[n];
		    for (int i = 0; i < n; i++) {
			messages[i] = dtm.getMessage(i);
			gets[i] = ((Boolean)dtm.getValueAt(i, 5)).booleanValue();
			dels[i] = ((Boolean)dtm.getValueAt(i, 6)).booleanValue();
		    }
		    POP3Frame.this.setClosable(false);
		    GetPOP3 lh = new GetPOP3(messages, gets, dels, jpb_count, POP3Frame.this);
		    lh.start();		    
		}
	    }
				);

	final JMenuItem mStop = new JMenuItem("Stop");
	mPOP3.add(mStop);
	mStop.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if (sw != null) sw.interrupt();
		}
	    }
				);

	JMenu mAll = new JMenu("Selected");
	jmb.add(mAll);

	JMenuItem mGet = new JMenuItem("Get");
	mAll.add(mGet);
	mGet.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    int rows[] = jt.getSelectedRows();
		    for (int i = 0; i < rows.length; i++) {
			boolean old = ((Boolean)dtm.getValueAt(rows[i], 5)).booleanValue();
			dtm.setValueAt(new Boolean(!old), rows[i], 5);
		    }
		}
	    }
			       );

	JMenuItem mDel = new JMenuItem("Del");
	mAll.add(mDel);
	mDel.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    int rows[] = jt.getSelectedRows();
		    for (int i = 0; i < rows.length; i++) {
			boolean old = ((Boolean)dtm.getValueAt(rows[i], 6)).booleanValue();
			dtm.setValueAt(new Boolean(!old), rows[i], 6);
		    }
		}
	    }
			       );
    }
}
