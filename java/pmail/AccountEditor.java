import java.awt.*;
import java.awt.event.*;

import java.util.*;

import javax.swing.*;
import javax.swing.event.*;

class AccountEditor extends JWindowFrame {
    static private JInternalFrame one;
    static public void showUnique(JDesktopPane desktop, JMenu windowMenu) {
	if (one == null) {
	    one = new AccountEditor(windowMenu);
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

    private DefaultListModel dlm = new DefaultListModel();

    public AccountEditor(JMenu windowMenu) {
	super(windowMenu, "POP3 Accounts", true, true, false, true);

	addInternalFrameListener(new InternalFrameAdapter() {
		public void internalFrameClosed(InternalFrameEvent e) {
		    one = null;
		}
	    }
				 );

	getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.X_AXIS));

	final JList accountsList = new JList(dlm);
	final JTextField server = new JTextField();
	final JTextField username = new JTextField();
	final JPasswordField password = new JPasswordField();
	final JTextField address = new JTextField();

	accountsList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
	accountsList.addListSelectionListener(new ListSelectionListener() {
		public void valueChanged(ListSelectionEvent e) {
		    boolean adj = e.getValueIsAdjusting();
		    int idx = accountsList.getSelectedIndex();
		    if (adj == false && idx != -1) {
			String name = (String)dlm.elementAt(idx);
			Account account = (Account)Options.accounts.get(name);
			server.setText(account.server);
			username.setText(account.username);
			password.setText(account.password);
			address.setText(account.address);
		    }
		}
	    }
					      );

	refresh();

	getContentPane().add(new JScrollPane(accountsList));

	JPanel jp = new JPanel(new GridLayout(4, 2));

	jp.add(new JLabel("Server:"));
	jp.add(server);
	jp.add(new JLabel("Username:"));
	jp.add(username);
	jp.add(new JLabel("Password:"));
	jp.add(password);
	jp.add(new JLabel("Address:"));
	jp.add(address);

	getContentPane().add(jp);

	jp = new JPanel(new GridLayout(3, 1));
	JButton jb = new JButton("Add");
	jb.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    String name = JOptionPane.showInternalInputDialog(AccountEditor.this, "Account name");
		    if (name != null) {
			Object old = Options.accounts.get(name);
			if (old == null) {
			    Options.accounts.put(name, new Account(name));
			    refresh();
			}
		    }
		    
		}
	    }
			     );
	jp.add(jb);

	jb = new JButton("Update");
	jb.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    int idx = accountsList.getSelectedIndex();
		    if (idx != -1) {
			String name = (String)dlm.elementAt(idx);
			Account account = (Account)Options.accounts.get(name);
			account.server = server.getText();
			account.username = username.getText();
			account.password = new String(password.getPassword());
			account.address = address.getText();
		    }
		}
	    }
			     );
	jp.add(jb);

	jb = new JButton("Delete");
	jb.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    int idx = accountsList.getSelectedIndex();
		    if (idx != -1) {
			String name = (String)dlm.elementAt(idx);
			Options.accounts.remove(name);
			refresh();
		    }
		}
	    }
			     );
	jp.add(jb);

	getContentPane().add(jp);
	
    }

    private void refresh() {
	dlm.removeAllElements();
	for (Enumeration e = Options.accounts.keys(); e.hasMoreElements(); ) {
	    dlm.addElement(e.nextElement());
	}

    }

}
