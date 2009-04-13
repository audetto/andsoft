import java.util.*;

class CaseInfo
{
    private boolean[] valids = new boolean[9];
	
    CaseInfo()
    {
        reset();
    }

    void reset()
    {
		for (int i = 0; i < 9; ++i)
			valids[i] = true;
    }
    
    void setFixed(int value)
    {
		for (int i = 0; i < 9; ++i)
			valids[i] = false;
		valids[value] = true;
    }
	
    void forbidValue(int value)
    {
		valids[value] = false;
    }
	
    boolean allowValue(int value)
    {
		return valids[value];
    }
	
    public String toString()
    {
		StringBuffer sb = new StringBuffer();
		boolean ok = false;
		for (int i = 0; i < 9; ++i)
		{
			if (valids[i])
			{
				sb.append(String.valueOf(i + 1));
				ok = true;
			} 
			else
			{
				sb.append(" ");
			}
		}
		
		if (ok)
			return sb.toString();
		else
			return "ERRORE";
    }
    
    int isFixed()
    {
		int count = 0;
		int value = -1;
		for (int i = 0; i < 9; ++i)
		{
			if (valids[i])
			{
				count++;
				value = i;
			}
		}
		
		if (count == 1)
			return value;
		else
			return -1;
    }
	
    Set<Integer> allowedValues()
    {
        Set<Integer> s = new HashSet<Integer> ();
        for (int i = 0; i < 9; ++i)
        {
            if (valids[i])
                s.add(i);
        }
		
        return s;
    }
	
}
