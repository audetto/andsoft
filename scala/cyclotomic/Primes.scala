import scala.collection.mutable.ListBuffer
import scala.collection.BitSet
import java.math.BigInteger

package asi.algebra
{
  object Primes
  {
    // we skip all multiples of the first 8 primes
    val sieve = Sieves.wheel(Array(2, 3, 5, 7, 11, 13, 17, 19))

    def primeFactors(num: BigInt): List[BigInt] =
      {
	// our best algorithm
	algoB(num)
      }

    def algoA(num: BigInt): List[BigInt] =
      { 
	algoA(num, sieve)
      }

    // This is Algorithm A
    def algoA(num: BigInt, sieve: Stream[BigInt]): List[BigInt] = 
      { 
	// try to facgtor n, with factors at least s.head
	def addFactor(n: BigInt, s: Stream[BigInt], sofar: List[BigInt]): List[BigInt] =
	  {
	    if (n == 1)
	      return sofar

	    var ss = s
	    var div = ss.head

	    if (n <= div || n.isProbablePrime(10))
	      return n :: sofar

	    do
	      {
		val rem = n /% div
		if (rem._2 == 0)
		  {
		    // new n, start from the beginning
		    return addFactor(rem._1, ss, div :: sofar);
		  }
		// just a new factor, don't check n again
		ss = ss.tail
		div = ss.head
	      }
	    while (div < n)

	    return n :: sofar
	  }

	addFactor(num, sieve, Nil)
      }

    def algoB(num: BigInt): List[BigInt] =
      {
	require(num > 1)
	
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
	    if (n.isProbablePrime(10))
	      {
		factors = n :: factors
		return factors.sortWith(_ > _)
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
	    
	    // this algorithm cannot factor perfect squares
	    // call into naive algorithm
	    factors = algoA(g) ++ factors
	    if (g == n)
	      return factors.sortWith(_ > _)

	    n = n / g
	    x = x % n
	    xp = xp % n
	  }
	Nil
      }

    def algoC(num: BigInt): BigInt =
      {
	require(num % 2 == 1)

	// Step C1
	val s: BigInt = Algorithms.sqrt(num)

	var a: BigInt = 2 * Algorithms.sqrt(num) + 1
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

    // find the smallest primitive root modulo n
    def primitiveRoot(n: BigInt) =
      {
	require(n.isProbablePrime(10))

	val factors = primeFactors(n - 1).distinct
	
	// check if r is a primitive root
	def check(r: BigInt) =
	  {
	    // since n is prime
	    // r.modPow(n - 1, n) == 1
	    !factors.exists(e => r.modPow((n - 1) / e, n) == BigInt(1))
	  }
	
	// we are looking for the smallest
	val root = (BigInt(2) to n - 1).find(check(_))
	
	// it cant fail
	root.get
      }

    def algoD(num: BigInt, r: Int): BigInt =
      {
	require(num % 2 == 1)

	val sieves = new Array[BitSet] (r)
	val p = new Array[Int] (r)
	val k = new Array[BigInt] (r)

	var x = Algorithms.sqrtU(num)

	for (i <- 0 until r)
	  {
	    // quadratic residues modulo 2 dont give any information
	    p(i) = if (i == 0) 3 else BigInt(p(i - 1)).bigInteger.nextProbablePrime.intValue

	    while (num.mod(p(i)) == 0)
	      p(i) = BigInt(p(i)).bigInteger.nextProbablePrime.intValue

	    sieves(i) = QuadraticResidues.create(p(i))
	    k(i) = (-x) mod p(i)
	  }
	
	def check(): Boolean =
	  {
	    for (i <- 0 until r)
	      {
		val kk = (k(i) * k(i) - num) mod p(i)
		if (!sieves(i)(kk.intValue))
		  return false
	      }
	    return true
	  }

	def advance()
	  {
	    x += 1
	    for (i <- 0 until r)
	      {
		k(i) = (k(i) - 1) mod p(i)
	      }
	  }

	while(true)
	  {
	    while (!check())
	      advance()

	    val y = Algorithms.sqrt(x * x - num)

	    if (y * y == x * x - num)
	      return x - y

	    advance()
	  }
	
	return 0
      }

    // call helper to find factors
    def factorHelper(num: BigInt, fact: BigInt => BigInt) =
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
		val f = fact(n)
		addFactor(n / f, addFactor(f, sofar))
	      }
	  }
	  
	addFactor(num, Nil).sortWith(_ > _)
      }

    def factorAlgoC(num: BigInt) =
      {
	factorHelper(num, x => algoC(x));
      }

    def factorAlgoD(num: BigInt) =
      {
	factorHelper(num, x => algoD(x, 30));
      }
    
  }
}
