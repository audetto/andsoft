import java.util.*;
import java.util.zip.*;

import java.io.*;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

import javax.mail.*;
import javax.mail.internet.*;

import javax.activation.*;

class Composer extends JWindowFrame {
    public Composer(Message msg, JMenu windowMenu) {
	super(windowMenu, "New Message", true, true, true, true);

	JPanel jp = new JPanel(new GridLayout(3, 2));

	JButton jb = new JButton("To:");
	jb.setHorizontalAlignment(SwingConstants.LEFT);
	jp.add(jb);
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
		    String address = JOptionPane.showInputDialog(Composer.this, "To:");
		    if (address != null)
			((DefaultComboBoxModel)to.getModel()).addElement(address);
		}
	    }
			    );
	jp.add(new JScrollPane(to));

	jp.add(new JLabel("From:"));
	final JComboBox from = new JComboBox();
	for (Enumeration en = Options.accounts.keys(); en.hasMoreElements(); ) {
	    from.addItem(en.nextElement());
	}
	jp.add(from);

	jp.add(new JLabel("Subject:"));
	final JTextField subject = new JTextField();
	jp.add(subject);

	final JTextArea text = new JTextArea();
	getContentPane().add(new JScrollPane(text));

	if (msg != null) {
	    try {
		subject.setText(msg.getSubject());
		Address[] froms = msg.getFrom();
		if (froms != null && froms.length > 0)
		    ((DefaultComboBoxModel)to.getModel()).addElement(((InternetAddress)froms[0]).getAddress());
		Object cont = msg.getContent();
		if (cont instanceof String) {
		    LineNumberReader lnr = new LineNumberReader(new StringReader((String)cont));
		    String str;
		    do {
			str = lnr.readLine();
			if (str != null) {
			    StringBuffer sb = new StringBuffer("> ").append(str).append("\n");
			    text.append(sb.toString());
			}
		    } while (str != null);
		}
	    } catch(Exception ex) {
		LogFrame.log(ex);
	    }

	}

	JPanel attachments = new JPanel();
	attachments.setLayout(new BoxLayout(attachments, BoxLayout.X_AXIS));
	final JList jl = new JList(new DefaultComboBoxModel());
	jl.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
	attachments.add(new JScrollPane(jl));
	JPanel buttons = new JPanel();
	jb = new JButton("Add");
	jb.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    Options.openFileChooser.setSelectedFile(null);
		    int returnVal = Options.openFileChooser.showOpenDialog(Composer.this);
		    if (returnVal == JFileChooser.APPROVE_OPTION) {
			File file = Options.openFileChooser.getSelectedFile();
			((DefaultComboBoxModel)jl.getModel()).addElement(file);
		    }
		}
	    }
			     );

	buttons.add(jb);
	jb = new JButton("Remove");
	jb.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    Object[] sel = jl.getSelectedValues();
		    if (sel.length > 0) {
			DefaultComboBoxModel dcbm = (DefaultComboBoxModel)jl.getModel();
			for (int i = 0; i < sel.length; i++) {
			    dcbm.removeElement(sel[i]);
			}
		    }
		}
	    }
			     );
	buttons.add(jb);
	attachments.add(buttons);

	JTabbedPane jtp = new JTabbedPane();
	jtp.addTab("Text", text);
	jtp.addTab("Attachments", attachments);
	jtp.setSelectedIndex(0);

	JSplitPane jsp = new JSplitPane(JSplitPane.VERTICAL_SPLIT, jp, jtp);
	getContentPane().add(jsp);

	JMenuBar jmb = new JMenuBar();
	setJMenuBar(jmb);
	JMenu mMessage = new JMenu("Message");
	jmb.add(mMessage);
	JMenuItem mSend = new JMenuItem("Send");
	mMessage.add(mSend);

	mSend.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    try {
			MimeMessage message = new MimeMessage(Options.session);
			int not = to.getModel().getSize();
			for (int i = 0; i < not; i++) {
			    String tos = (String)to.getModel().getElementAt(i);
			    message.addRecipient(Message.RecipientType.TO, new InternetAddress(tos));
			}
			String name = (String)from.getSelectedItem();
			Account account = (Account)Options.accounts.get(name);
			message.setFrom(new InternetAddress(account.address));
			message.setSubject(subject.getText());
			ListModel lm = jl.getModel();
			if (lm.getSize() == 0)
			    message.setText(text.getText());
			else {
			    message.setSentDate(new Date());
			    Multipart multipart = new MimeMultipart();
			    BodyPart messageBodyPart = new MimeBodyPart();
			    messageBodyPart.setText(text.getText());
			    multipart.addBodyPart(messageBodyPart);

			    for (int i = 0; i < lm.getSize(); i++) {
				messageBodyPart = new MimeBodyPart();
				File att = (File)lm.getElementAt(i);
				DataSource source = new FileDataSource(att.toString());
				messageBodyPart.setDataHandler(new DataHandler(source));
				messageBodyPart.setFileName(att.getName());
				multipart.addBodyPart(messageBodyPart);
			    }

			    message.setContent(multipart);
			}

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
