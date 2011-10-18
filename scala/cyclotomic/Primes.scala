import scala.collection.mutable.ListBuffer

package asi.algebra
{
  object Primes
  {
    def primeFactors(num: BigInt) = 
      { 
	def nextFactor(n: BigInt, soFar: List[BigInt]): List[BigInt] =
	  {
	    if (n == 1)
	      {
		return soFar;
	      }
	    
	    if (n % 2 == 0)
	      {
		return nextFactor(n / 2, 2 :: soFar)
	      }
	    
	    var i = 3
	    while (i * i <= n)
	    {
	      if (n % i == 0)
		{
		  return nextFactor(n / i, i :: soFar)
		}
	      /* only check odd numbers */
	      i += 2
	    }
	    
	    return n :: soFar;
	  }

	val soFar = Nil
	nextFactor(num, soFar)
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
