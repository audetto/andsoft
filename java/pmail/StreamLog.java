import javax.swing.*;

import java.io.*;

import java.awt.event.*;

class DummyOutputStream extends OutputStream {
    public void write(int b) throws IOException {
    }
}

class StreamLog extends JWindowFrame {
    private JTabbedPane pane;
    private static StreamLog one;
    
    public StreamLog(JMenu windowMenu) {
	super(windowMenu, "Debug Log", true, false, true, true);
	one = this;

	pane = new JTabbedPane();
	add(pane);

	JMenuBar jmb = new JMenuBar();
	setJMenuBar(jmb);

	JMenu mOptions = new JMenu("Options");
	jmb.add(mOptions);

	JMenuItem mRemove = new JMenuItem("Remove");
	mOptions.add(mRemove);
	mRemove.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    pane.remove(pane.getSelectedComponent());
		}
	    }
				);
    }
 
    public static PrintStream getPs(String name) {
	final JTextArea area = new JTextArea();
	area.setEditable(false);
	one.pane.add(new JScrollPane(area), name);

	PrintStream ps = new PrintStream(new DummyOutputStream()) {
		public void println(String x) {
		    area.append(x + "\n");
		}
	    };
	return ps;
    }
}
