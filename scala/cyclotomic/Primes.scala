import scala.collection.mutable.ListBuffer

package asi.algebra
{
  object Primes
  {
    def primeFactors(num: Int) = 
      { 
	val factors = new ListBuffer[Int]
	var n = num

	while (n % 2 == 0)
	{
	  factors += 2
	  n /= 2
	}

	var i = 3
	while (i * i <= n)
	{
	  while (n % i == 0)
	  {
	    factors += i
	    n /= i
	  }
	  /* only check odd numbers */
	  i += 2
	}
	
	if (n > 1)
	  factors += n

	factors.toList
      }

    def isPrime(n: Int): Boolean =
      {
	if (n == 1)
	  return false

	if (n == 2)
	  return true

	if (n % 2 == 0)
	  return false

	var i = 3
	while (i * i <= n)
	{
	  if (n % i == 0)
	    return false

	  /* only check odd numbers */
	  i += 2
	}

	return true
      }
  }
}
