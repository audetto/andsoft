import java.io.*;

import java.util.*;
import java.util.zip.*;

import java.awt.*;

import javax.swing.*;

import javax.mail.*;
import javax.mail.internet.*;

import com.sun.mail.pop3.*;

class GetPOP3 extends SwingWorker {
    Message[] messages;
    boolean[] gets;
    boolean[] dels;
    JInternalFrame frame;
    JProgressBar progress;
    POP3Folder inbox;
    HashSet newUids = new HashSet();
    HashSet delUids = new HashSet();
    
    
    public GetPOP3(Message[] messages, boolean[] gets, boolean[] dels, JProgressBar progress, JInternalFrame frame, Folder inbox) {
	this.messages = messages;
	this.gets = gets;
	this.dels = dels;
	this.frame = frame;
	this.progress = progress;
	this.inbox = (POP3Folder)inbox;
    }

    public Object construct() {
	final int msgs = messages.length;
	
	Session session = Session.getInstance(new Properties());
	
	for (int i = 0; i < msgs; i++) {
	    try {
		Vector data = null;
		Address[] from = null;
		Address[] to = null;

		if (gets[i]) {
		    File tmpFile = File.createTempFile("msg", ".mail.gz", Options.inboxDir);
		    OutputStream os = new GZIPOutputStream(new BufferedOutputStream(new FileOutputStream(tmpFile)));
		    messages[i].writeTo(os);
		    os.close();

		    GZIPInputStream gzis = new GZIPInputStream(new BufferedInputStream(new FileInputStream(tmpFile)));
		    MimeMessage msg2 = new MimeMessage(session, gzis);
		    gzis.close();

		    from = msg2.getFrom();
		    to = msg2.getAllRecipients();
		    data = FilesTableModel.processMessage(msg2, tmpFile.getName());
		    
		    newUids.add(inbox.getUID(messages[i]));
		}

		if (dels[i]) {
		    messages[i].setFlag(Flags.Flag.DELETED, true);
		    newUids.add(inbox.getUID(messages[i]));
		}

		final int i2 = i + 1;
		final Vector data2 = data;
		final Address[] from2 = from;
		final Address[] to2 = to;
		EventQueue.invokeLater(new Runnable() {
			public void run() {
			    if (data2 != null)
				FoldersFrame.addIfExists(data2);
			    if (from2 != null)
				AddressBook.process(from2);
			    if (to2 != null)
				AddressBook.process(to2);

			    progress.setMaximum(msgs);
			    progress.setValue(i2);
			}
		    }
					   );
	    } catch(Exception ex) {
		LogFrame.log(ex);
	    }
	}

	Object[] res = {newUids, delUids};
	return res;
    }
        
    public void finished() {
	frame.dispose();
    }

}
