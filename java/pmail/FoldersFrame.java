import java.awt.*;
import java.awt.event.*;
import java.awt.datatransfer.*;

import java.io.*;
import java.util.*;
import java.util.zip.*;

import javax.mail.*;
import javax.mail.internet.*;

import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.event.*;
import javax.swing.table.*;

class MyTreeNode extends DefaultMutableTreeNode {
    public FilesTableModel ftm;
    public MyTreeNode(Object userObject, FilesTableModel ftm) {
	super(userObject);
	this.ftm = ftm;
    }
}

class DestTransferHandler extends TransferHandler {
    public boolean canImport(JComponent comp, DataFlavor[] targetFlavors) {
	return true;
    }
    public boolean importData(JComponent comp, Transferable t) {
	boolean ok = false;
	try {
	    JTree jtree = (JTree)comp;
	    MsgSelection msgsel = (MsgSelection)t.getTransferData(t.getTransferDataFlavors()[0]);
	    TreePath selPath = jtree.getSelectionPath();
	    MyTreeNode mtn = (MyTreeNode)selPath.getLastPathComponent();

	    FilesTableModel ftm = (FilesTableModel)msgsel.tm;

	    if (ftm != mtn.ftm) {

		File sourceDir = ftm.getDir();
		File destDir = mtn.ftm.getDir();

		for (int i = 0; i < msgsel.rows.length; i++) {
		    File destFile = File.createTempFile("msg", ".mail.gz", destDir);
		    String fileName = (String)ftm.getValueAt(msgsel.rows[i], 3);
		    File sourceFile = new File(sourceDir, fileName);

		    Utilities.copyStream(new BufferedInputStream(new FileInputStream(sourceFile)), new BufferedOutputStream(new FileOutputStream(destFile)));
		    MimeMessage msg = new MimeMessage(Options.session, new GZIPInputStream(new BufferedInputStream(new FileInputStream(destFile))));
		    Vector data = FilesTableModel.processMessage(msg, destFile.getName());
		    mtn.ftm.addRow(data);
		}
		return true;
	    }
	} catch(Exception ex) {
	    LogFrame.log(ex);
	}
	return ok;
    }
}

class FoldersFrame extends JWindowFrame {
    static private FoldersFrame one;
    static public void showUnique(JDesktopPane desktop, JMenu windowMenu) {
	if (one == null) {
	    one = new FoldersFrame(windowMenu);
	    one.pack();
	    one.setVisible(true);
	    desktop.add(one);
	}

	try {
	    one.setSelected(true);
	} catch(Exception ex) {
	    LogFrame.log(ex);
	}
	
    }
    public static void addIfExists(Vector dati) {
	if (one != null) {
	    one.inbox.addRow(dati);
	}
    }

    public static void queueIfExists(Vector dati) {
	if (one != null) {
	    one.queue.addRow(dati);
	}
    }

    private JTable jtab;
    private JTree jtre;
    private FilesTableModel inbox = new FilesTableModel(Options.inboxDir);
    private FilesTableModel delete = new FilesTableModel(Options.deleteDir);
    private FilesTableModel queue = new FilesTableModel(Options.queueDir);
    private FilesTableModel sent = new FilesTableModel(Options.sentDir);

    public FoldersFrame(final JMenu windowMenu) {
	super(windowMenu, "Folders", true, false, true, true);

	addInternalFrameListener(new InternalFrameAdapter() {
		public void internalFrameClosed(InternalFrameEvent e) {
		    one = null;
		}
	    }
				 );

	jtab = FilesTableModel.createFilesTable(windowMenu, this);
	
	final DefaultMutableTreeNode root = new DefaultMutableTreeNode("Mail");
	jtre = new JTree(root);

	jtre.setRootVisible(false);
	final MyTreeNode inboxNode = new MyTreeNode("inbox", inbox);
	root.add(inboxNode);
	MyTreeNode deleteNode = new MyTreeNode("delete", delete);
	root.add(deleteNode);
	MyTreeNode queueNode = new MyTreeNode("queue", queue);
	root.add(queueNode);
	MyTreeNode sentNode = new MyTreeNode("sent", sent);
	root.add(sentNode);
	TreePath tp = new TreePath(root);
	jtre.expandPath(tp);

	jtre.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
	jtre.addTreeSelectionListener(new TreeSelectionListener() {
		public void valueChanged(TreeSelectionEvent e) {
		    TreePath selPath = jtre.getSelectionPath();
		    if (selPath != null) {
			MyTreeNode mtn = (MyTreeNode)selPath.getLastPathComponent();
			jtab.setModel(mtn.ftm);
		    }
		}
	    }
			      );
	jtre.setTransferHandler(new DestTransferHandler());

      	JSplitPane jsp = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, new JScrollPane(jtre), new JScrollPane(jtab));
	add(jsp, BorderLayout.CENTER);

	JLabel progress = new JLabel();
	add(progress, BorderLayout.NORTH);

	JMenuBar jmb = new JMenuBar();
	setJMenuBar(jmb);

	JMenu mPath = new JMenu("Path");
	jmb.add(mPath);

	JMenuItem mNew = new JMenuItem("New folder...");
	mPath.add(mNew);
	mNew.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    TreePath selPath = jtre.getSelectionPath();
		    Object box = selPath.getPathComponent(1);
		    if (inboxNode.equals(box)) {
			String dirName = JOptionPane.showInternalInputDialog(FoldersFrame.this, "Folder name");
			if (dirName != null) {
			    MyTreeNode mtn = (MyTreeNode)selPath.getLastPathComponent();
			    File newDir = new File(mtn.ftm.getDir(), dirName);
			    if (newDir.mkdir()) {
				FilesTableModel ftm = new FilesTableModel(newDir);
				MyTreeNode new_node = new MyTreeNode(dirName, ftm);
				mtn.add(new_node);

				if (mtn.getChildCount() > 1) {  // bug workaround
				    ((DefaultTreeModel)jtre.getModel()).nodeStructureChanged(root);
				    jtre.setSelectionPath(selPath);
				}
			    }
			}
		    }
		}
	    }
			       );

	JMenu mQueue = new JMenu("Queue");
	jmb.add(mQueue);

	JMenuItem mSendAll = new JMenuItem("Send all...");
	mQueue.add(mSendAll);
	mSendAll.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    JPanel jp = new JPanel(new GridLayout(2, 1));
		    JProgressBar jpb = new JProgressBar();
		    jpb.setMaximum(queue.getRowCount());
		    jp.add(jpb);
		    JLabel jl = new JLabel("SMTP");
		    jl.setHorizontalAlignment(JLabel.CENTER);
		    jp.add(jl);
		    JDialog jd = new JDialog(JOptionPane.getFrameForComponent(FoldersFrame.this), "Sending mail...", true);
		    jd.setContentPane(jp);
		    jd.setResizable(true);
		    jd.setDefaultCloseOperation(JDialog.DO_NOTHING_ON_CLOSE);
		    jd.pack();
		    jd.setSize(400, jd.getHeight());
		    SendMail sm = new SendMail(sent, queue, jpb, jl, jd);
		    sm.start();
		    jd.setVisible(true);
		}
	    }
			       );

	MyTreeNode[] nodes = {inboxNode, deleteNode, queueNode, sentNode};
	setClosable(false);
	ReadFolders rf = new ReadFolders(nodes, progress, this);
	rf.start();
    }

}
