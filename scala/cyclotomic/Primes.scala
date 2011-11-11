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
	    else
	      d + 4
	  }

	// try to facgtor n, with factors at least d
	def addFactor(n: BigInt, d: BigInt, sofar: List[BigInt]): List[BigInt] =
	  {
	    if (n == 1)
	      return sofar

	    if (n <= d || n.isProbablePrime(10))
	      return n :: sofar

	    var div = d

	    do
	      {
		var rem: Array[BigInteger] = n.underlying().divideAndRemainder(div.underlying())
		if (rem(1).compareTo(BigInteger.ZERO) == 0)
		  {
		    // new n, start from the beginning
		    return addFactor(new BigInt(rem(0)), div, div :: sofar);
		  }
		// just a new factor, don't check n again
		div = nextFactor(div)
	      }
	    while (div < n)

	    return n :: sofar
	  }

	addFactor(num, 2, Nil)
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

    // call algoC to find factors
    def factorC(num: BigInt) =
      {
	def addFactor(n: BigInt, sofar: List[BigInt]): List[BigInt] =
	  {
	    if (num % 2 == 0)
	      {
		addFactor(num / 2, 2 :: sofar)
	      }
	    else if (n.isProbablePrime(10))
	      {
		n :: sofar
	      }
	    else if (n == 1)
	      {
		sofar
	      }
	    else
	      {
		val f = algoC(n)
		addFactor(n / f, addFactor(f, sofar))
	      }
	  }
	  
	addFactor(num, Nil).sortWith(_ > _)
      }

    // find the smallest primitive root modulo n
    def primitiveRoot(n: BigInt) =
      {
	require(n.isProbablePrime(10))

	val factors = primeFactors(n - 1).distinct
	
	// check if r is a primitive root
	def check(r: BigInt) =
	  {
	    if (r.modPow(n - 1, n) != BigInt(1))
	      false
	    else
	      !factors.exists(e => r.modPow((n - 1) / e, n) == BigInt(1))
	  }
	
	// we are looking for the smallest
	val root = (BigInt(2) to n - 1).find(check(_))
	
	// it cant fail
	root.get
      }
  }
}
