import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;

class SuDoku extends JFrame
{
    private JTextField[][] numbers;
    private Case[][] guesses;
    private JTextField textRepresentation;

    private Case[][] rows;
    private Case[][] cols;
    private Case[][] blocks;

    SuDoku()
    {
	super("Su Doku - (C)opyRight AndSoft Inc., 2005-09");

	Font font = new Font("Monospaced", Font.BOLD, 14);

	getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
	Border border = BorderFactory.createLineBorder(Color.BLUE, 2);

	numbers = new JTextField[9][];
	guesses = new Case[9][];
	rows    = new Case[9][];
	cols    = new Case[9][];
	blocks  = new Case[9][];
	for (int i = 0; i < 9; ++i)
	{
	    numbers[i] = new JTextField[9];
	    guesses[i] = new Case[9];

	    rows[i]   = new Case[9];
	    cols[i]   = new Case[9];
	    blocks[i] = new Case[9];
	}

	JPanel input = new JPanel(new GridLayout(3, 3));
	JPanel output = new JPanel(new GridLayout(3, 3));
	for (int i = 0; i < 3; ++i)
	{
	    for (int j = 0; j < 3; ++j)
	    {
		JPanel small_input = new JPanel(new GridLayout(3, 3));
		small_input.setBorder(border);

		JPanel small_output = new JPanel(new GridLayout(3, 3));
		small_output.setBorder(border);

		int block = i * 3 + j;
		int row_base = i * 3;
		int col_base = j * 3;
		
		int inBlock = 0;

		for (int k = row_base; k < row_base + 3; ++k)
		{
		    for (int h = col_base; h < col_base + 3; ++h)
		    {
			numbers[k][h] = new JTextField();
			numbers[k][h].setHorizontalAlignment(JTextField.CENTER);
			numbers[k][h].setFont(font);
			small_input.add(numbers[k][h]);

			Case thisCase = new Case();
			guesses[k][h] = thisCase;
			guesses[k][h].setHorizontalAlignment(JTextField.CENTER);
			guesses[k][h].setFont(font);
			small_output.add(guesses[k][h]);

			rows[k][h] = thisCase;
			cols[h][k] = thisCase;
			blocks[block][inBlock] = thisCase;
			++inBlock;
		    }
		}
		input.add(small_input);
		output.add(small_output);
	    }
	}
	
	getContentPane().add(input);

	JPanel cmds = new JPanel();
	cmds.setLayout(new BoxLayout(cmds, BoxLayout.X_AXIS));
	
	final JCheckBox forbidden  = new JCheckBox("forbidden",	 true);
	final JCheckBox compulsory = new JCheckBox("compulsory", true);
	final JCheckBox indirect   = new JCheckBox("indirect",	 true);
	final JCheckBox update	   = new JCheckBox("update",	 true);

	JButton clear = new JButton("Clear");
	clear.addActionListener(new ActionListener()
	    {
		public void actionPerformed(ActionEvent e)
		{
		    clear();
		}
	    }
	    );
	cmds.add(clear);

	JButton read = new JButton("Read");
	read.addActionListener(new ActionListener()
	    {
		public void actionPerformed(ActionEvent e)
		{
		    String str = JOptionPane.showInputDialog(SuDoku.this, "Initial position:");
		    if (str != null)
			readFrom(str);
		}
	    }
	    );
	cmds.add(read);

	JButton go = new JButton("Go");
	go.addActionListener(new ActionListener()
	    {
		public void actionPerformed(ActionEvent e)
		{
		    if (forbidden.isSelected())	 scanForForbiddens();
		    if (compulsory.isSelected()) goForTheGlory();
		    if (indirect.isSelected())	 secondPass();
		    if (update.isSelected())	 update();
		}
	    }
	    );
	cmds.add(go);

	JPanel flags = new JPanel();
	flags.setLayout(new BoxLayout(flags, BoxLayout.X_AXIS));

	flags.add(forbidden);
	flags.add(compulsory);
	flags.add(indirect);
	flags.add(update);
	
	getContentPane().add(cmds);
	getContentPane().add(flags);

	getContentPane().add(output);

	textRepresentation = new JTextField();
	textRepresentation.setFont(font);
	textRepresentation.setEditable(false);
	textRepresentation.setHorizontalAlignment(JTextField.CENTER);
	
	getContentPane().add(textRepresentation);

	numbers[0][2].setText("7");
	numbers[0][4].setText("9");
	numbers[1][1].setText("8");
	numbers[1][6].setText("1");
	numbers[1][8].setText("2");
	numbers[2][0].setText("6");
	numbers[2][5].setText("2");
	numbers[2][7].setText("4");
	numbers[3][0].setText("4");
	numbers[3][8].setText("9");
	numbers[4][3].setText("3");
	numbers[4][4].setText("4");
	numbers[4][5].setText("6");
	numbers[5][0].setText("5");
	numbers[5][8].setText("7");
	numbers[6][1].setText("5");
	numbers[6][3].setText("9");
	numbers[6][8].setText("3");
	numbers[7][0].setText("3");
	numbers[7][2].setText("1");
	numbers[7][7].setText("2");
	numbers[8][4].setText("6");
	numbers[8][6].setText("8");
	
    }

    void clear()
    {
	for (int i = 0; i < 9; ++i)
	    for (int j = 0; j < 9; ++j)
	    {
		numbers[i][j].setText(null);
		guesses[i][j].reset();
	    }
	textRepresentation.setText(null);
    }

    void scanForForbiddens()
    {
	for (int i = 0; i < 9; ++i)
	    for (int j = 0; j < 9; ++j)
		doOneCase(i, j);
    }

    void readFrom(String str)
    {
        clear();

        try
        {
            int pos = 0;

            for (int i = 0; i < 9; ++i)
            {
                for (int j = 0; j < 9; ++j)
                {
                    char ch;
                    do
                    {
                        ch = str.charAt(pos);
                        ++pos;
                    }
                    while (!(ch == '.' || (ch >= '1' && ch <= '9')));
                    if (ch != '.')
                        numbers[i][j].setText(String.valueOf(ch));
                }
            }
        }
        catch (RuntimeException e)
        {
            clear();
            e.printStackTrace();
        }

    }

    int getBlock(int row, int col)
    {
	int r = row / 3;
	int c = col / 3;
	int b = r * 3 + c;
	return b;
    }

    void doOneCase(int row, int col)
    {
	String s = numbers[row][col].getText();
	if (s.length() > 0)
	{
	    int value = Integer.parseInt(s) - 1;

	    guesses[row][col].setFixed(value);

	    int b = getBlock(row, col);

	    for (int i = 0; i < 9; ++i)
	    {
		rows[row][i].forbidValue(value, Color.YELLOW);
		cols[col][i].forbidValue(value, Color.YELLOW);
		blocks[b][i].forbidValue(value, Color.YELLOW);
	    }
	}
    }

    void goForTheGlory()
    {
	for (int j = 0; j < 9; ++j)
	{
	    lookForOneBlock(rows[j]);
	    lookForOneBlock(cols[j]);
	    lookForOneBlock(blocks[j]);
	}
    }

    void lookForOneBlock(Case[] block)
    {
	for (int number = 0; number < 9; ++number)
	{
	    int count = 0;
	    int first = -1;
	    for (int pos = 0; pos < 9; ++pos)
	    {
		if (block[pos].allowValue(number))
		{
		    ++count;
		    first = pos;
		}
	    }
	    if (count == 1)
		block[first].newFixed(number);
	    if (count == 0)
		System.out.println("Error " + String.valueOf(number + 1));
	}
    }

    void update()
    {
	textRepresentation.setText(null);
	StringBuilder str = new StringBuilder();

	for (int i = 0; i < 9; ++i)
	    for (int j = 0; j < 9; ++j)
	    {
		int isFixed = guesses[i][j].isFixed();
		if (isFixed != -1)
		{
		    String thisStr = String.valueOf(isFixed + 1);
		    str.append(thisStr);
		    numbers[i][j].setText(thisStr);
		}
		else
		{
		    str.append(".");
		}

	    }
	textRepresentation.setText(str.toString());
    }

    void secondPass()
    {
	for (int r = 0; r < 3; ++r)
	    for (int c = 0; c < 3; ++c)
		lookForDoublesInOneSquare(r, c);
    }

    void lookForDoublesInOneSquare(int r, int c)
    {
	for (int number = 0; number < 9; ++number)
	{
	    int count = 0;
	    int first_r = -1;
	    int first_c = -1;
	    int row_base = r * 3;
	    int col_base = c * 3;
	    for (int row = row_base; row < row_base + 3; ++row)
	    {
		for (int col = col_base; col < col_base + 3; ++col)
		{
		    if (guesses[row][col].allowValue(number))
		    {
			++count;
			if (count == 1)
			{
			    first_r = row;
			    first_c = col;
			}
			else
			{
			    if (first_r == row)
			    {
				first_c = -1;
			    }
			    else
			    {
				if (first_c == col)
				{
				    first_r = -1;
				}
				else
				{
				    first_r = -1;
				    first_c = -1;
				}
			    }
			}
		    }
		}
	    }
	    if (first_r != -1)
	    {
		for (int col = 0; col < 9; ++col)
		{
		    int c2 = col / 3;
		    if (c2 != c)
			guesses[first_r][col].forbidValue(number, Color.WHITE);
		}
	    }
	    if (first_c != -1)
	    {
		for (int row = 0; row < 9; ++row)
		{
		    int r2 = row / 3;
		    if (r2 != r)
			guesses[row][first_c].forbidValue(number, Color.WHITE);
		}
	    }
	}
    }

    public static void main(String[] args)
    {
	EventQueue.invokeLater(new Runnable()
	    {
		public void run()
		{
		    JFrame frame = new SuDoku();
		    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		    frame.pack();
		    frame.setVisible(true);
		}
	    }
	    );
    }
}
