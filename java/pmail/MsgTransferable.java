import java.awt.datatransfer.*;

class MsgTransferable implements Transferable {
    public final static DataFlavor MSG_FLAVOR = new DataFlavor(MsgSelection.class, DataFlavor.javaJVMLocalObjectMimeType);
    private MsgSelection sel;

    MsgTransferable(MsgSelection sel) {
	this.sel = sel;
    }
    public DataFlavor[] getTransferDataFlavors() {
	DataFlavor[] df = {MSG_FLAVOR};
	return df;
    }
    public boolean isDataFlavorSupported(DataFlavor flavor) {
	return flavor.equals(MSG_FLAVOR);
    }
    public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException {
	return sel;
    }
}
