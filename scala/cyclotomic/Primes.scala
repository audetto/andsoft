import scala.collection.mutable.ListBuffer
import java.math.BigInteger

package asi.algebra
{
  object Primes
  {
    // This is Algorithm A
    def primeFactors(num: BigInt): List[BigInt] = 
      { 
	def nextFactor(d: BigInt): BigInt =
	  {
	    if (d == 2)
	      3
	    else if (d == 3)
	      5
	    else if (d % 4 == 1) 
	      d + 2 
	    else d + 4
	  }

	var factors: List[BigInt] = Nil
	var d: BigInt = 2

	// Step A1
	var n = num

	if (n == 1)
	  return List(1)

	// Step A3
	do
	  {
	    var rem: Array[BigInteger] = n.underlying().divideAndRemainder(d.underlying())
	    // Step A4
	    while (rem(1).compareTo(BigInteger.ZERO) == 0)
	    {
	      // Step A5
	      factors = d :: factors
	      n = new BigInt(rem(0))

	      // Step A2
	      if (n == 1)
		return factors

	      // Step A6
	      if (n < d)
		return n :: factors

	      rem = n.underlying().divideAndRemainder(d.underlying())
	    }

	    // Step A6
	    if (rem(0).compareTo(d.underlying()) < 0)
	    // Step A7
	      return n :: factors
	    
	    // Step A6
	    d = nextFactor(d)
	  }
	while (true)

	factors
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

    def sqrt(num: BigInt) =
      {
	def next(n: BigInt, i: BigInt) : BigInt = (n + i / n) >> 1
	
	val one = BigInt(1)
	
	var n = one
	var n1 = next(n, num)
	
	while ((n1 - n).abs > one)
	  {
	    n = n1
	    n1 = next(n, num)
	  }
	
	while (n1 * n1 > num)
	  {
	    n1 -= one
	  }
	
	n1
      }

    def algoB(num: BigInt): List[BigInt] =
      {
	// Step B1
	var x:  BigInt = 5
	var xp: BigInt = 2
	var k:  BigInt = 1
	var l:  BigInt = 1
	var n:  BigInt = num
	
	var factors: List[BigInt] = Nil

	while (true)
	  {
	    // Step B2
	    if (n.isProbablePrime(8))
	      {
		factors = n :: factors
		return factors
	      }
	    
	    // Step B3
	    var g = n.gcd(xp - x)
	    while (g == 1)
	    {
	      // Step B4
	      k -= 1
	      if (k == 0)
		{
		  xp = x
		  l *= 2
		  k = l
		}
	      x = (x * x + 1) % n
	      g = n.gcd(xp - x)
	    }
	    
	    // Step B3
	    factors = g :: factors
	    if (g == n)
	      return factors

	    n = n / g
	    x = x % n
	    xp = xp % n
	  }
	Nil
      }

    def algoC(num: BigInt) =
      {
	require(num % 2 == 1)

	// Step C1
	val s: BigInt = sqrt(num)

	var a: BigInt = 2 * sqrt(num) + 1
	var b: BigInt = 1
	var r = s * s - num
	
	// Step C2
	while (r != 0)
	  {
	    // Step C3
	    r += a
	    a += 2

	    do
	      {
		// Step C4
		r -= b
		b += 2
	      }
	    // Step C5
	    while (r > 0)
	    
	  }

	// success
	// Step C2
	(a - b) / 2
      }

  }
}
