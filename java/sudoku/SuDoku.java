import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;

class SuDoku extends JFrame {
    private JTextField[][] numbers;
    private Case[][] guesses;

    SuDoku() {
	super("Su Doku - (C)opyRight AndSoft Inc., 2005");

	addWindowListener(new WindowAdapter() {
		public void windowClosing(WindowEvent e) {
		    System.exit(0);
		}
	    }
			  );

	Font font = new Font("Monospaced", Font.BOLD, 14);

	getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
	Border border = BorderFactory.createLineBorder(Color.BLUE, 2);

	numbers = new JTextField[9][];
	for (int i = 0; i < 9; ++i)
	    numbers[i] = new JTextField[9];

	JPanel input = new JPanel(new GridLayout(3, 3));
	for (int i = 0; i < 3; ++i) {
	    for (int j = 0; j < 3; ++j) {
		JPanel small = new JPanel(new GridLayout(3, 3));
		small.setBorder(border);
		int row_base = i * 3;
		int col_base = j * 3;
		for (int k = row_base; k < row_base + 3; ++k) {
		    for (int l = col_base; l < col_base + 3; ++l) {
			numbers[k][l] = new JTextField();
			numbers[k][l].setHorizontalAlignment(JTextField.CENTER);
			numbers[k][l].setFont(font);
			small.add(numbers[k][l]);
		    }
		}
		input.add(small);
	    }
	}
	
	getContentPane().add(input);

	JPanel cmds = new JPanel();
	cmds.setLayout(new BoxLayout(cmds, BoxLayout.X_AXIS));
	
	final JCheckBox forbidden  = new JCheckBox("forbidden",	 true);
	final JCheckBox compulsory = new JCheckBox("compulsory", true);
	final JCheckBox indirect   = new JCheckBox("indirect",	 true);
	final JCheckBox update	   = new JCheckBox("update",	 true);

	JButton go = new JButton("Go");
	go.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if (forbidden.isSelected())	 scanForForbiddens();
		    if (compulsory.isSelected()) goForTheGlory();
		    if (indirect.isSelected())	 secondPass();
		    if (update.isSelected())	 update();
		}
	    }
			     );
	cmds.add(go);
	cmds.add(forbidden);
	cmds.add(compulsory);
	cmds.add(indirect);
	cmds.add(update);

	getContentPane().add(cmds);

	guesses = new Case[9][];
	for (int i = 0; i < 9; ++i)
	    guesses[i] = new Case[9];

	JPanel output = new JPanel(new GridLayout(3, 3));
	for (int i = 0; i < 3; ++i) {
	    for (int j = 0; j < 3; ++j) {
		JPanel small = new JPanel(new GridLayout(3, 3));
		small.setBorder(border);
		int row_base = i * 3;
		int col_base = j * 3;
		for (int k = row_base; k < row_base + 3; ++k) {
		    for (int l = col_base; l < col_base + 3; ++l) {
			guesses[k][l] = new Case();
			guesses[k][l].setHorizontalAlignment(JTextField.CENTER);
			guesses[k][l].setFont(font);
			small.add(guesses[k][l]);
		    }
		}
		output.add(small);
	    }
	}

	getContentPane().add(output);
	
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
	
	pack();
	setVisible(true);
    }

    void scanForForbiddens() {
	for (int i = 0; i < 9; ++i)
	    for (int j = 0; j < 9; ++j)
		doOneCase(i, j);
    }

    void doOneCase(int row, int col) {
	String s = numbers[row][col].getText();
	if (s.length() > 0) {
	    int value = Integer.parseInt(s) - 1;

	    guesses[row][col].setFixed(value);

	    // same row
	    for (int i = 0; i < 9; ++i)
		guesses[row][i].forbidValue(value, Color.YELLOW);

	    // same col
	    for (int j = 0; j < 9; ++j)
		guesses[j][col].forbidValue(value, Color.YELLOW);

	    int hsq = row / 3;
	    hsq = hsq * 3;
	    int vsq = col / 3;
	    vsq = vsq * 3;

	    for (int i = hsq; i < hsq + 3; ++i) {
		for (int j = vsq; j < vsq + 3; ++j)
		    guesses[i][j].forbidValue(value, Color.YELLOW);
	    }
	}
    }

    void goForTheGlory() {
	for (int j = 0; j < 9; ++j)
	    lookForOneColumn(j);
	for (int i = 0; i < 9; ++i)
	    lookForOneRow(i);
	for (int r = 0; r < 3; ++r)
	    for (int c = 0; c < 3; ++c)
		lookForOneSquare(r, c);
    }

    void lookForOneColumn(int col) {
	for (int number = 0; number < 9; ++number) {
	    int count = 0;
	    int first = -1;
	    for (int row = 0; row < 9; ++row) {
		if (guesses[row][col].allowValue(number)) {
		    ++count;
		    first = row;
		}
	    }
	    if (count == 1)
		guesses[first][col].newFixed(number);
	    if (count == 0)
		System.out.println("Error " + String.valueOf(number + 1) + " in column " + String.valueOf(col));
	}
    }

    void lookForOneRow(int row) {
	for (int number = 0; number < 9; ++number) {
	    int count = 0;
	    int first = -1;
	    for (int col = 0; col < 9; ++col) {
		if (guesses[row][col].allowValue(number)) {
		    ++count;
		    first = col;
		}
	    }
	    if (count == 1)
		guesses[row][first].newFixed(number);
	    if (count == 0)
		System.out.println("Error " + String.valueOf(number + 1) + " in row " + String.valueOf(row));
	}
    }

    void lookForOneSquare(int r, int c) {
	for (int number = 0; number < 9; ++number) {
	    int count = 0;
	    int first_r = -1;
	    int first_c = -1;
	    int row_base = r * 3;
	    int col_base = c * 3;
	    for (int row = row_base; row < row_base + 3; ++row) {
		for (int col = col_base; col < col_base + 3; ++col) {
		    if (guesses[row][col].allowValue(number)) {
			++count;
			first_r = row;
			first_c = col;
		    }
		}
	    }
	    if (count == 1)
		guesses[first_r][first_c].newFixed(number);
	    if (count == 0)
		System.out.println("Error " + String.valueOf(number + 1) + " in square " + 
				   String.valueOf(r * 10 + c));
	}
    }

    void update() {
	for (int i = 0; i < 9; ++i)
	    for (int j = 0; j < 9; ++j) {
		int isFixed = guesses[i][j].isFixed();
		if (isFixed != -1)
		    numbers[i][j].setText(String.valueOf(isFixed + 1));
	    }
    }

    void secondPass() {
	for (int r = 0; r < 3; ++r)
	    for (int c = 0; c < 3; ++c)
		lookForDoublesInOneSquare(r, c);
    }

    void lookForDoublesInOneSquare(int r, int c) {
	for (int number = 0; number < 9; ++number) {
	    int count = 0;
	    int first_r = -1;
	    int first_c = -1;
	    int row_base = r * 3;
	    int col_base = c * 3;
	    for (int row = row_base; row < row_base + 3; ++row) {
		for (int col = col_base; col < col_base + 3; ++col) {
		    if (guesses[row][col].allowValue(number)) {
			++count;
			if (count == 1) {
			    first_r = row;
			    first_c = col;
			} else {
			    if (first_r == row)
				first_c = -1;
			    else
				if (first_c == col)
				    first_r = -1;
				else {
				    first_r = -1;
				    first_c = -1;
				}
			}
		    }
		}
	    }
	    if (first_r != -1) {
		for (int col = 0; col < 9; ++col) {
		    int c2 = col / 3;
		    if (c2 != c)
			guesses[first_r][col].forbidValue(number, Color.WHITE);
		}
	    }
	    if (first_c != -1) {
		for (int row = 0; row < 9; ++row) {
		    int r2 = row / 3;
		    if (r2 != r)
			guesses[row][first_c].forbidValue(number, Color.WHITE);
		}
	    }
	}
    }

    public static void main(String[] args) {
	new SuDoku();
    }
}
