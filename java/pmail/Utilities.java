import java.io.*;

class Utilities {

    public static void copyStream(InputStream in, OutputStream out) throws IOException {
	byte[] buf = new byte[4096];
	int nch;
	while ((nch = in.read(buf)) != -1) {
	    out.write(buf, 0 , nch);
	}
	out.close();
    }

}
