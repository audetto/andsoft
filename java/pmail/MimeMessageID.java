import javax.mail.*;
import javax.mail.internet.*;

class MimeMessageID extends MimeMessage {
    public MimeMessageID(MimeMessage source) throws MessagingException {
	super(source);
    }
    public MimeMessageID(Session session) {
	super(session);
    }
    protected void updateHeaders() throws MessagingException {
	super.updateHeaders();
	if (!(Options.messageID == null))
	    setHeader("Message-ID", Options.messageID);
    }
}
