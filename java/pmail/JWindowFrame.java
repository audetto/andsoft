import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.*;

class WindowMenuItem extends JMenuItem {
    private JInternalFrame jif;

    public WindowMenuItem(String title, JInternalFrame jif) {
	super(title);
	this.jif = jif;
	addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    try {
			WindowMenuItem.this.jif.setIcon(false);
			WindowMenuItem.this.jif.setSelected(true);
		    } catch(Exception ex) {
			LogFrame.log(ex);
		    }
		}
	    }
				);
    }
}

class JWindowFrame extends JInternalFrame {

    private JMenuItem jmi;
    private JMenu windowMenu;

    public JWindowFrame(JMenu windowMenu, String title, boolean resizable, boolean closable, boolean maximizable, boolean iconifiable) {
	super(title, resizable, closable, maximizable, iconifiable);
	
	this.windowMenu = windowMenu;

	jmi = new WindowMenuItem(title, this);
	windowMenu.add(jmi);

	addInternalFrameListener(new InternalFrameAdapter() {
		public void internalFrameClosed(InternalFrameEvent e) {
		    JWindowFrame.this.windowMenu.remove(JWindowFrame.this.jmi);
		    JWindowFrame.this.jmi = null;
		    JWindowFrame.this.windowMenu = null;
		}
	    }
			  );
    }

    public void setTitle(String title) {
	super.setTitle(title);
	jmi.setText(title);
    }
	
}
