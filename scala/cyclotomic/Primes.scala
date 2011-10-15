import scala.collection.mutable.ListBuffer

package asi.algebra
{
  object Primes
  {
    def primeFactors(num: BigInt) = 
      { 
	val factors = new ListBuffer[BigInt]
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

    def isPrime(n: BigInt): Boolean =
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

    /* Exponentiation by squaring
     */
    def ipow(base: BigInt, exp: BigInt) =
      {
	var res: BigInt = 1
	var b = base
	var e = exp
	while (e != 0)
	  {
	    if ((e & 1) != 0)
	      res *= b
	    e >>= 1
	    b *= b
	  }
	res
      }
  }
}
