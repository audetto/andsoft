import java.io.*;
import java.text.*;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

import javax.mail.internet.*;

class MimeViewer extends JWindowFrame {
    
    public MimeViewer(JMenu windowMenu, InputStream input, String title) {
	super(windowMenu, title, true, true, true, true);

	try {

	    MimeBodyPart mbp = new MimeBodyPart(input);

	    JTabbedPane jtp = new JTabbedPane();
	    add(jtp, BorderLayout.CENTER);

	    String fileName = mbp.getFileName();
	
	    JPanel jp = new JPanel(new GridLayout(8, 2));
	    jp.add(new JLabel("Content Type:"));
	    jp.add(new JLabel(mbp.getContentType(), JLabel.RIGHT));

	    jp.add(new JLabel("Disposition:"));
	    jp.add(new JLabel(mbp.getDisposition(), JLabel.RIGHT));

	    jp.add(new JLabel("MD5:"));
	    jp.add(new JLabel(mbp.getContentMD5(), JLabel.RIGHT));

	    jp.add(new JLabel("FileName:"));
	    JButton jb;
	    if (fileName == null)
		jb = new JButton("Save");
	    else {
		fileName = MimeUtility.decodeText(fileName);
		jb = new JButton(fileName);
	    }
	    jb.addActionListener(new SaveStreamAction(fileName, mbp.getInputStream(), this));
	    jp.add(jb);

	    jp.add(new JLabel("Description:"));
	    jp.add(new JLabel(mbp.getDescription(), JLabel.RIGHT));

	    NumberFormat format = NumberFormat.getIntegerInstance();
	    jp.add(new JLabel("Size:"));
	    int size = mbp.getSize();
	    jp.add(new JLabel(new StringBuffer(format.format(size)).append(" bytes").toString(), JLabel.RIGHT));

	    jp.add(new JLabel("ContentID:"));
	    jp.add(new JLabel(mbp.getContentID(), JLabel.RIGHT));

	    jp.add(new JLabel("Encoding:"));
	    jp.add(new JLabel(mbp.getEncoding(), JLabel.RIGHT));

	    jtp.addTab("Mime", jp);

	    jp = new JPanel(new GridLayout(1, 1));
	    ByteArrayOutputStream baos = new ByteArrayOutputStream();
	    mbp.writeTo(baos);
	    String raw = baos.toString();
	    JTextArea jta = new JTextArea(raw);
	    jta.setFont(Options.monoFont);
	    jta.setEditable(false);
	    jp.add(new JScrollPane(jta));

	    jtp.addTab("Raw", jp);

	} catch(Exception ex) {
	    LogFrame.log(ex);
	}

    }

}
