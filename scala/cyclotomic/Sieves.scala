import scala.collection.mutable.BitSet

package asi.algebra
{
  object Sieves
  {
    // returns a stream of all the given prime numbers
    // plus all the non multiples of any of them
    def wheel(primes: Array[Int]) =
      {
	val p = (1 /: primes) (_ * _)
	val bits = new BitSet(p)
	
	for (i <- 1 to p)
	  {
	    bits.add(i)
	  }
	
	for (i <- primes)
	  {
	    // remove all multiples of i
	    var ii = i
	    
	    do
	      {
		bits.remove(ii)
		ii += i
	      }
	    while (ii <= p)
	  }
	
	// the first numbers come from the primes
	def str1(n: Int): Stream[BigInt] =
	  {
	    if (n < primes.length)
	      {
		primes(n) #:: str1(n + 1)
	      }
	    else
	      {
		// switch to the sieve
		val it = bits.iterator
		it.next // skip 1
		str2(it, 0)
	      }
	  }
	
	// afterwards we go with the sieve
	def str2(it: Iterator[Int], b: BigInt): Stream[BigInt] =
	  {
	    if (it.hasNext)
	      {
		(b + it.next()) #:: str2(it, b)
	      }
	    else
	      {
		// restart sieve for next batch
		str2(bits.iterator, b + p)
	      }
	  }
	
	str1(0)
	
      }
  }

}
