import java.io.*;

import java.util.*;
import java.util.zip.*;

import java.awt.*;

import javax.swing.*;

import javax.mail.*;
import javax.mail.internet.*;

import com.sun.mail.smtp.*;

class SendMail extends SwingWorker {
    FilesTableModel sent;
    FilesTableModel queue;
    JProgressBar progress;
    JLabel label;
    JDialog dialog;

    SendMail(FilesTableModel sent, FilesTableModel queue, JProgressBar progress, JLabel label, JDialog dialog) {
	this.sent = sent;
	this.queue = queue;
	this.progress = progress;
	this.label = label;
	this.dialog = dialog;
    }

    public Object construct() {

	Properties props = new Properties();
	if (Options.smtpUserID != null) {
	    props.setProperty("mail.smtp.auth", "true");
	}

	Session session = Session.getInstance(props);
	if (Options.debug) {
	    session.setDebug(true);
	    session.setDebugOut(StreamLog.getPs(Options.smtpServer));
	}
	File destDir = sent.getDir();
	final int rows = queue.getRowCount();

	try {
	    SMTPTransport trans = (SMTPTransport)session.getTransport("smtp");
	    trans.setStartTLS(true);
	    trans.connect(Options.smtpServer, Options.smtpUserID, Options.smtpPasswd);

	    for (int i = rows - 1; i >= 0; i--) {
		try {
		    final int i2 = i;

		    String fileName = (String)queue.getValueAt(i, 3);
		    File file = new File(queue.getDir(), fileName);

		    GZIPInputStream gzis = new GZIPInputStream(new BufferedInputStream(new FileInputStream(file)));
		    final MimeMessage msg = new MimeMessage(session, gzis);
		    gzis.close();

		    final String sbj = msg.getSubject();

		    EventQueue.invokeLater(new Runnable() {
			    public void run() {
				progress.setValue(rows - i2);
				label.setText(sbj);
			    }
			}
					       );

		    trans.sendMessage(msg, msg.getAllRecipients());

		    final File destFile = File.createTempFile("msg", ".mail.gz", destDir);
		    Utilities.copyStream(new BufferedInputStream(new FileInputStream(file)), new BufferedOutputStream(new FileOutputStream(destFile)));
		    EventQueue.invokeLater(new Runnable() {
			    public void run() {
				try {
				    Vector data = FilesTableModel.processMessage(msg, destFile.getName());
				    queue.removeRow(i2);
				    sent.addRow(data);
				} catch(Exception ex) {
				    LogFrame.log(ex);
				}
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

	return null;
    }
    public void finished() {
	label.setText("DONE");
	dialog.setDefaultCloseOperation(JDialog.HIDE_ON_CLOSE);
    }
}
