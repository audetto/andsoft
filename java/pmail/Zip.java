import java.util.zip.*;

import java.io.*;

class Zip {
    private static void zipFile(File file) {
	try {
	    System.out.println(file.getAbsolutePath());
	    File outFile = new File(file.getAbsolutePath() + ".gz");
	    Utilities.copyStream(new BufferedInputStream(new FileInputStream(file)), new GZIPOutputStream(new BufferedOutputStream(new FileOutputStream(outFile))));
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
    public static void main(String args[]) {
	for (int i = 0; i < args.length; i++) {
	    File file = new File(args[i]);
	    zipFile(file);
	}
    }
}
