import java.io.*;
import java.util.*;
import java.awt.*;

import javax.mail.*;
import javax.mail.internet.*;

import javax.swing.*;
import javax.swing.table.*;

class ReadFiles extends SwingWorker {
    DefaultTableModel tablemodel;
    Component frame;
    JProgressBar progress;

    public ReadFiles(JProgressBar progress, Component frame) {
	this.frame = frame;
	this.progress = progress;
    }

    public Object construct() {

	    Session session = Session.getInstance(new Properties());
	    
	    File[] files = Options.inboxDir.listFiles(new FilenameFilter() {
		    public boolean accept(File dir, String name) {
			return name.matches("msg.*\\.mail");
		    }
		}
				       );

	    int msgs = files.length;

	    for (int i = 0; i < msgs; i++) {
		try {

		    BufferedInputStream bis = new BufferedInputStream(new FileInputStream(files[i]));
		    MimeMessage msg = new MimeMessage(session, bis);
		    bis.close();
		
		    final Object[] data2 = FilesTableModel.processMessage(msg, files[i].getName());
		    final int i2 = i + 1;
		    final int msgs2 = msgs;
		    SwingUtilities.invokeLater(new Runnable() {
			    public void run() {
				FilesFrame.addIfExists(data2);
				progress.setMinimum(0);
				progress.setMaximum(msgs2);
				progress.setValue(i2);
			    }
			}
					       );
		} catch(Exception ex) {
		    LogFrame.log(ex);
		}
	    }

	    return null;
    
    }

}
