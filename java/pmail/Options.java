import java.io.*;
import java.util.*;
import java.awt.*;

import javax.swing.*;

import javax.mail.*;

class Options {
    static File inboxDir = new File("/home/andrea/asi/mail/inbox");
    static File deleteDir = new File("/home/andrea/asi/mail/delete");
    static File queueDir = new File("/home/andrea/asi/mail/queue");
    static File sentDir = new File("/home/andrea/asi/mail/sent");
    static Properties addressBook = new Properties();
    static String smtpServer = "pentium";
    static String smtpUserID = null;
    static String smtpPasswd = null;
    static Hashtable accounts = new Hashtable();
    static JFileChooser saveFileChooser = new JFileChooser();
    static JFileChooser openFileChooser = new JFileChooser();
    static Font monoFont = new Font("Monospaced", Font.PLAIN, 12);
    static Session session = Session.getInstance(new Properties());

    public static void init()  {
	try {
	    ObjectInputStream ois = new ObjectInputStream(new FileInputStream(new File("/home/andrea/asi/mail/accounts.conf")));
	    accounts = (Hashtable)ois.readObject();
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
	try {
	    addressBook.load(new FileInputStream(new File("/home/andrea/asi/mail/book.conf")));
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
	try {
	    ObjectInputStream ois = new ObjectInputStream(new FileInputStream(new File("/home/andrea/asi/mail/options.conf")));
	    smtpServer = (String)ois.readObject();
	    smtpUserID = (String)ois.readObject();
	    smtpPasswd = (String)ois.readObject();
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
	inboxDir.mkdir();
	deleteDir.mkdir();
	queueDir.mkdir();
	sentDir.mkdir();
    }
    public static void close() {
	try {
	    ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(new File("/home/andrea/asi/mail/accounts.conf")));
	    oos.writeObject(accounts);
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
	try {
	    addressBook.store(new FileOutputStream(new File("/home/andrea/asi/mail/book.conf")), "POP3 mail AddressBook");
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
	try {
	    ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(new File("/home/andrea/asi/mail/options.conf")));
	    oos.writeObject(smtpServer);
	    oos.writeObject(smtpUserID);
	    oos.writeObject(smtpPasswd);
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
    }
}
