import java.util.*;
import java.util.zip.*;

import java.io.*;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

import javax.mail.*;
import javax.mail.internet.*;

class Forward extends JWindowFrame {
    public Forward(final MimeMessage msg, JMenu windowMenu) {
	super(windowMenu, "<NONE>", true, true, true, true);

	getContentPane().setLayout(new GridLayout(3, 2));

	JButton jb = new JButton("To:");
	jb.setHorizontalAlignment(SwingConstants.LEFT);
	getContentPane().add(jb);
	final JList to = new JList(new DefaultComboBoxModel());
	to.addKeyListener(new KeyAdapter() {
		public void keyTyped(KeyEvent e) {
		    // this is the char code of delete as seen in a debug session
		    if (e.getKeyChar() == '\u007f') {
			Object[] sel = to.getSelectedValues();
			if (sel.length > 0) {
			    DefaultComboBoxModel dcbm = (DefaultComboBoxModel)to.getModel();
			    for (int i = sel.length - 1; i >= 0; i++) {
				dcbm.removeElement(sel[i]);
			    }
			}
		    }
		}
	    }
			  );
	jb.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    String address = JOptionPane.showInputDialog(Forward.this, "To:");
		    if (address != null)
			((DefaultComboBoxModel)to.getModel()).addElement(address);
		}
	    }
			    );
	getContentPane().add(new JScrollPane(to));

	getContentPane().add(new JLabel("From:"));
	final JComboBox from = new JComboBox();
	for (Enumeration en = Options.accounts.keys(); en.hasMoreElements(); ) {
	    from.addItem(en.nextElement());
	}
	getContentPane().add(from);

	getContentPane().add(new JLabel("Subject:"));
	final JTextField subject = new JTextField();
	try {
	    String sbj = "FWD: " + msg.getSubject();
	    subject.setText(sbj);
	    setTitle(sbj);
	} catch(Exception ex) {
	    LogFrame.log(ex);
	}
	getContentPane().add(subject);

	JMenuBar jmb = new JMenuBar();
	setJMenuBar(jmb);
	JMenu mMessage = new JMenu("Message");
	jmb.add(mMessage);
	JMenuItem mSend = new JMenuItem("Send");
	mMessage.add(mSend);

	mSend.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    try {
			MimeMessage message = new MimeMessageID(msg);
			int not = to.getModel().getSize();
			message.setRecipients(Message.RecipientType.TO, (Address[])null);
			for (int i = 0; i < not; i++) {
			    String tos = (String)to.getModel().getElementAt(i);
			    message.addRecipient(Message.RecipientType.TO, new InternetAddress(tos));
			}
			String name = (String)from.getSelectedItem();
			Account account = (Account)Options.accounts.get(name);
			message.setFrom(new InternetAddress(account.address));
			message.setSubject(subject.getText());

			File tmpFile = File.createTempFile("msg", ".mail.gz", Options.queueDir);
			OutputStream os = new GZIPOutputStream(new BufferedOutputStream(new FileOutputStream(tmpFile)));
			message.writeTo(os);
			os.close();
			
			Vector data = FilesTableModel.processMessage(message, tmpFile.getName());
			FoldersFrame.queueIfExists(data);

			dispose();

		    } catch(Exception ex) {
			LogFrame.log(ex);
		    }

		}
	    }
			     );

    }
}
