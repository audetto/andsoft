import java.io.*;

import javax.swing.*;
import javax.swing.event.*;

class Eccezione {
    private String message;
    private String stack;

    public Eccezione(String message, String stack) {
	if (message == null)
	    message = "<NONE>";
	this.message = message;
	this.stack = stack;
    }
    
    public String toString() {
	return message;
    }

    public String getStack() {
	return stack;
    }
}

class LogFrame extends JWindowFrame {

    private JTextArea jta = new JTextArea();
    private JList jl;
    private DefaultListModel dlm = new DefaultListModel();

    private static LogFrame one;

    public LogFrame(JMenu windowMenu) {
	super(windowMenu, "Error log", true, false, true, true);
	one = this;

	jta.setEditable(false);

	jl = new JList(dlm);
	jl.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
	jl.addListSelectionListener(new ListSelectionListener() {
		public void valueChanged(ListSelectionEvent e) {
		    boolean adj = e.getValueIsAdjusting();
		    int idx = jl.getSelectedIndex();
		    if (adj == false && idx != -1) {
			Eccezione ecc = (Eccezione)dlm.elementAt(idx);
			String stack = ecc.getStack();
			jta.setText(stack);
		    }
		}
	    }
				    );

	JSplitPane jsp = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, new JScrollPane(jl), new JScrollPane(jta));
	getContentPane().add(jsp);
    }

    public static void log(Exception e) {
	StringWriter sw = new StringWriter();
	e.printStackTrace(new PrintWriter(sw));
	final String message = e.getMessage();
	final String stack = sw.toString();
	SwingUtilities.invokeLater(new Runnable() {
		public void run() {
		    Eccezione ecc = new Eccezione(message, stack);
		    one.dlm.addElement(ecc);
		}
	    }
				   );
    }
}
	
