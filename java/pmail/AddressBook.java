import java.util.*;
import javax.mail.*;
import javax.mail.internet.*;

import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;

class AddressBook extends JWindowFrame {
    static private AddressBook one;
    static public void showUnique(JDesktopPane desktop, JMenu windowMenu) {
	if (one == null) {
	    one = new AddressBook(windowMenu);
	    one.pack();
	    one.show();
	    desktop.add(one);
	}

	try {
	    one.setSelected(true);
	} catch(Exception ex) {
	    LogFrame.log(ex);
	}
	
    }

    private JTable jt = new JTable();
    private String[] header = {"Name", "Address"};

    public AddressBook(JMenu windowMenu) {
	super(windowMenu, "Address Book", true, true, true, true);

	addInternalFrameListener(new InternalFrameAdapter() {
		public void internalFrameClosed(InternalFrameEvent e) {
		    one = null;
		}
	    }
			  );
	refresh();

	JScrollPane jsp = new JScrollPane(jt);
	add(jsp);
    }

    public void refresh() {
	Enumeration keys = Options.addressBook.keys();
	int size = Options.addressBook.size();
	String[][] dati = new String[size][2];
	for (int i = 0; i < size; i++) {
	    dati[i][0] = (String)keys.nextElement();
	    dati[i][1] = Options.addressBook.getProperty(dati[i][0]);
	}	
	DefaultTableModel dtm = new DefaultTableModel(dati, header);
	jt.setModel(dtm);
    }

    public static void process(Address[] addresses) {
	for (int i = 0; i < addresses.length; i++) {
	    try {
		InternetAddress ia = (InternetAddress)addresses[i];
		String personal = ia.getPersonal();
		String address = ia.getAddress();
		if (personal == null)
		    personal = address;
		Options.addressBook.setProperty(personal, address);
		if (one != null)
		    one.refresh();
	    } catch(Exception ex) {
		LogFrame.log(ex);
	    } 
	}
	
    }
}
