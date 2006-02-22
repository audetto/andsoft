import java.io.*;
import java.awt.*;
import java.text.*;

import java.util.*;
import java.util.zip.*;

import javax.mail.*;
import javax.mail.internet.*;

import javax.swing.*;
import javax.swing.table.*;

class ReadFolders extends SwingWorker {
    DefaultTableModel tablemodel;
    JInternalFrame frame;
    JLabel progress;
    File dir;
    MyTreeNode[] nodes;
    
    int file_count = 0;
    int size_count = 0;

    Session session = Session.getInstance(new Properties());

    public ReadFolders(MyTreeNode[] nodes, JLabel progress, JInternalFrame frame) {
	this.frame = frame;
	this.progress = progress;
	this.dir = dir;
	this.nodes = nodes;
    }

    private void processDirectory(final MyTreeNode node) {

	File dir = node.ftm.getDir();

	File[] files = dir.listFiles(new FileFilter() {
		public boolean accept(File pathname) {
		    if (pathname.isDirectory())
			return true;
		    else {
			String name = pathname.getName();
			return name.matches("msg.*\\.mail\\.gz");
		    }
		}
	    }
						  );

	int msgs = files.length;

	for (int i = 0; i < msgs; i++) {
	    if (files[i].isFile()) {
		try {
		    GZIPInputStream gzis = new GZIPInputStream(new BufferedInputStream(new FileInputStream(files[i])));
		    MimeMessage msg = new MimeMessage(session, gzis);
		    gzis.close();

		    final Vector dati = FilesTableModel.processMessage(msg, files[i].getName());

		    file_count++;
		    size_count += msg.getSize();
		    Object[] data = {new Integer(file_count), new Integer(size_count)};
		    final String label = MessageFormat.format("{0, number, integer} messages ({1, number, integer} bytes)", data);

		    EventQueue.invokeLater(new Runnable() {
			    public void run() {
				node.ftm.addRow(dati);	
				progress.setText(label);
			    }
			}
					       );
		    
		} catch(Exception ex) {
		    LogFrame.log(ex);
		}
	    } else {
		FilesTableModel ftm = new FilesTableModel(files[i]);
		final MyTreeNode new_node = new MyTreeNode(files[i].getName(), ftm);
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
			    node.add(new_node);
			}
		    }
					   );
		processDirectory(new_node);
	    }
	}
    }

    public Object construct() {
	for (int i = 0; i < nodes.length; i++) {
	    processDirectory(nodes[i]);
	}
	return null;
    }

    public void finished() {
	frame.setClosable(true);
    }

}
