import scala.collection.mutable.ListBuffer

package asi.algebra
{
  object Primes
  {
    def primeFactors(num: Int) = 
      { 
	var n = num
	var factors = new ListBuffer[Int]
	for (i <- 2 to n
	     if i * i <= n)
	  {
	    while (n % i == 0)
	    {
	      factors += i
	      n /= i
	    }
	  }

	if (n > 1)
	  factors += n

	factors.toList
      }
  }
}
