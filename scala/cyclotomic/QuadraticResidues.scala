import scala.collection.mutable.BitSet

package asi.algebra
{
  object QuadraticResidues
    {
      /*
       * Returns a BitSet containing all the possible
       * quadratic residues mod p
       */
      def create(p: Int) =
	{
	  val sieve = new BitSet(p)
	  for (i <- 1 to p)
	    {
	      val residue = (i * i) % p
	      sieve.add(residue)
	    }
	  sieve
	}
    }
}
