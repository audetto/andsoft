abstract class SuDokuMethod
{
	protected Case[][] guesses;

	protected Case[][] rows;
	protected Case[][] cols;
	protected Case[][] blocks;

    SuDokuMethod(Case[][] guesses, Case[][] rows, Case[][] cols, Case[][] blocks)
    {
        this.guesses = guesses;
        this.rows = rows;
        this.cols = cols;
        this.blocks = blocks;
    }

    abstract void solve();
}
