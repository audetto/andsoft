import java.io.*;
import java.util.*;
import java.awt.*;

import javax.swing.*;

import javax.mail.*;

class Options {
    static final String baseDir = "/home/andrea";

    static final File inboxDir = new File(baseDir + "/asi/mail/inbox");
    static final File deleteDir = new File(baseDir + "/asi/mail/delete");
    static final File queueDir = new File(baseDir + "/asi/mail/queue");
    static final File sentDir = new File(baseDir + "/asi/mail/sent");
    static Properties addressBook = new Properties();
    static String smtpServer = "pentium";
    static String smtpUserID = null;
    static String smtpPasswd = null;
    static String messageID = null;
    static Hashtable accounts = new Hashtable();
    static final JFileChooser saveFileChooser = new JFileChooser();
    static final JFileChooser openFileChooser = new JFileChooser();
    static final Font monoFont = new Font("Monospaced", Font.PLAIN, 12);
    static final Session session = Session.getInstance(new Properties());

    static final String accountsFileName = baseDir + "/asi/mail/accounts.conf";
    static final String addressBookFileName = baseDir + "/asi/mail/book.conf";
    static final String optionsFileName = baseDir + "/asi/mail/options.conf";

    public static void init()  {
	try {
	    ObjectInputStream ois = new ObjectInputStream(new FileInputStream(new File(accountsFileName)));
	    accounts = (Hashtable)ois.readObject();
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
	try {
	    addressBook.load(new FileInputStream(new File(addressBookFileName)));
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
	try {
	    ObjectInputStream ois = new ObjectInputStream(new FileInputStream(new File(optionsFileName)));
	    smtpServer = (String)ois.readObject();
	    smtpUserID = (String)ois.readObject();
	    smtpPasswd = (String)ois.readObject();
	    messageID = (String)ois.readObject();
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
	    ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(new File(accountsFileName)));
	    oos.writeObject(accounts);
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
	try {
	    addressBook.store(new FileOutputStream(new File(addressBookFileName)), "POP3 mail AddressBook");
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
	try {
	    ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(new File(optionsFileName)));
	    oos.writeObject(smtpServer);
	    oos.writeObject(smtpUserID);
	    oos.writeObject(smtpPasswd);
	    oos.writeObject(messageID);
	} catch(Exception ex) {
	    ex.printStackTrace();
	}
    }
}
