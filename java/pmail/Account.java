import java.io.*;

class Account implements Serializable {
    public String name;
    public String server;
    public String username;
    public String password;
    public String address;
    public boolean ssl;

    public Account(String name) {
	this.name = name;
    }
}
