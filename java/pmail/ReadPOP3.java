import java.io.*;
import java.util.*;
import java.awt.*;
import java.text.*;

import javax.mail.*;
import javax.mail.event.*;
import javax.mail.internet.*;

import javax.swing.*;
import javax.swing.table.*;

import com.sun.mail.pop3.*;

class ReadPOP3 extends SwingWorker {
    DefaultTableModel tablemodel;
    String hostname;
    String username;
    String password;
    Component frame;
    JProgressBar progress;

    String finalmessage = null;

    public ReadPOP3(String hostname, String username, String password, DefaultTableModel tablemodel, JProgressBar progress, Component frame) {
	this.tablemodel = tablemodel;
	this.hostname = hostname;
	this.username = username;
	this.password = password;
	this.frame = frame;
	this.progress = progress;
    }

    public Object construct() {
	Store store = null;
	Folder inbox = null;

	try {

	    Properties props = new Properties();
	    //	    props.setProperty("mail.debug", "true");
	    Session session = Session.getInstance(props);
	    store = session.getStore("pop3");
	    store.connect(hostname, username, password);

	    inbox = store.getFolder("INBOX");
	    inbox.open(Folder.READ_WRITE);
	    final int msgs = inbox.getMessageCount();

	    Message[] messages = inbox.getMessages();
	    POP3Folder popfld = (POP3Folder)inbox;

	    Object[] data = {new Integer(msgs), new Integer(popfld.getSize())};
	    finalmessage = MessageFormat.format("{0, number, integer} messages ({1, number, integer} bytes)", data);

	    for (int i = msgs - 1; i >= 0; i--) {
		
		if (Thread.interrupted())
		    break;

		try {

		    final Object[] dati = new Object[8];

		    Address[] from = messages[i].getFrom();
		    if (from != null && from.length > 0) {
			InternetAddress ia = (InternetAddress)from[0];
			dati[0] = ia.toUnicodeString();
		    }

		    dati[1] = messages[i].getSubject();
	      
		    MimeMessage mimemsg = (MimeMessage)messages[i];
		    dati[2] = new Integer(mimemsg.getSize());
	    
		    dati[3] = popfld.getUID(messages[i]);

		    dati[4] = messages[i].getSentDate();

		    dati[5] = Boolean.FALSE;
		    dati[6] = Boolean.FALSE;

		    dati[7] = messages[i];

		    final int i2 = msgs - i;
		    SwingUtilities.invokeLater(new Runnable() {
			    public void run() {
				tablemodel.addRow(dati);
				progress.setMaximum(msgs);
				progress.setValue(i2);
			    }
			}
					       );
		} catch(Exception ex) {
		    LogFrame.log(ex);
		}
	    }
	} catch(Exception ex) {
	    LogFrame.log(ex);
	}

	Object[] res = {store, inbox};
	return res;    
    }

    public void finished() {
	JOptionPane.showInternalMessageDialog(frame, finalmessage);
    }

}
