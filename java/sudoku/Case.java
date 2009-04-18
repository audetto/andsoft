import javax.swing.*;
import java.awt.*;
import java.util.*;

class Case extends JTextField
{
    private CaseInfo ci = new CaseInfo();
    private boolean fixed = false;
    private Color defaultColor;
	
    Case()
    {
		super("123456789");
		setEditable(false);
		defaultColor = getBackground();
    }
	
    void reset()
    {
		setText("123456789");
		setBackground(defaultColor);
		ci.reset();
		fixed = false;
    }
	
    boolean forbidValue(int value, Color color)
    {
		int wasFixed = ci.isFixed();

        // returns true is anything has changed
		boolean result = false;

        if (!fixed)
		{
			result = ci.forbidValue(value);
			setText(ci.toString());
		}
	    
		if (wasFixed == -1)
		{
			int isFixed = ci.isFixed();
			if (isFixed != -1)
				setBackground(color);
        }

        return result;
    }
	
    void setFixed(int value)
    {
		ci.setFixed(value);
		setText(ci.toString());
		fixed = true;
		setBackground(Color.GREEN);
    }
	
    void newFixed(int value)
    {
		int isFixed = ci.isFixed();
		if (isFixed == -1)
		{
			ci.setFixed(value);
			setText(ci.toString());
			fixed = true;
			setBackground(Color.RED);
		}
    }
	
    boolean isValid(int value)
    {
		return ci.isValid(value);
    }
	
    int isFixed()
    {
		return ci.isFixed();
    }
	
    Set<Integer> validValues()
    {
        return ci.validValues();
    }
}
