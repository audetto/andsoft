// References are from
// Harold M. Edwards
// Fermat's Last Theorem

import scala.collection.mutable.StringBuilder

package asi.algebra
{
  class Cyclotomic(d: Int, c: Array[BigInt])
  {
    require(BigInt(d).isProbablePrime(10))

    val dim: Int = d

    val coeffs: Array[BigInt] = createArray(dim)

    for (i <- 0 until c.length)
      {
	coeffs(i % dim) += c(i)
      }
    normalise
    
    private def normalise
    {
      /*
       * We adopt the convention that the highest power has always a zero coefficient
       */

      for (i <- 0 until dim)
	{
	  coeffs(i) -= coeffs(dim - 1)
	}
    }

    private def createArray(size: Int) = Array.fill(size) { BigInt(0) }
    
    override def toString =
      {
	var buf = new StringBuilder
	for (i <- 0 until dim)
	  {
	    buf ++= coeffs(i).toString
	    buf += '['
	    buf ++= i.toString
	    buf ++= "] "
	  }
	buf.drop(1)
	buf.toString
      }
    
    def + (that: Cyclotomic) =
      {
	require(that.dim == dim)
	val c = new Array[BigInt](dim)
	
	for (i <- 0 until dim)
	  {
	    c(i) = coeffs(i) + that.coeffs(i);
	  }
	
	new Cyclotomic(dim, c);
      }

    def - (that: Cyclotomic) =
      {
	this + (that * -1)
      }
    
    def * (that: Cyclotomic) = 
      {
	require(that.dim == dim)
	val c = createArray(2 * dim - 1)
	
	for (i <- 0 until dim)
	  {
	    for (j <- 0 until dim)
	      {
		c(i + j) += coeffs(i) * that.coeffs(j)
	      }
	  }
	
	new Cyclotomic(dim, c)
      }

    def * (that: BigInt) = 
      {
	val c = createArray(dim)
	
	for (i <- 0 until dim)
	  {
	    c(i) += coeffs(i) * that
	  }
	
	new Cyclotomic(dim, c)
      }

    def / (that: Cyclotomic): Option[Cyclotomic] =
      {
	if (that.isInteger)
	  {
	    this / that.asInteger
	  }
	else
	  {
	    val conj = that.conjugates
	    var c1 = this
	    var c2 = that
	    for (i <- 1 until conj.length)
	      {
		c1 *= conj(i)
		c2 *= conj(i)
	      }

	    c1 / c2.asInteger
	  }
      }

    def / (that: BigInt): Option[Cyclotomic] =
      {
	/*
	 * Since they are always normalised,
	 * there is one coeff = 0, then rem = 0
	 */

	val rem = coeffs(0) % that;

	val c =  Array.fill(dim) { BigInt(0) }

	for (i <- 0 until dim)
	  {
	    if (coeffs(i) % that != rem)
	      return None
	    c(i) = (coeffs(i) + rem) / that
	  }
	
	Some(new Cyclotomic(dim, c))
      }

    def rotate(pos: Int) =
      {
	val c = createArray(pos * dim)
	for (i <- 0 until dim)
	  {
	    c(i * pos) = coeffs(i)
	  }
	
	new Cyclotomic(dim, c)
      }
    
    def conjugates =
      {
	val conj = new Array[Cyclotomic](dim - 1)
	for (j <- 1 until dim)
	  {
	    conj(j - 1) = rotate(j)
	  }
	conj
      }
    
    def asInteger =
      {
	require(isInteger)
	coeffs(0)
      }
    
    def isInteger: Boolean =
      {
	for (i <- 1 until dim)
	  {
	    if (coeffs(i) != 0)
	      return false
	  }
	true
      }
    
    def norm =
      {
	val p = (Cyclotomic.one(dim) /: conjugates) (_ * _)
	p.asInteger
      }

    def factors =
      {
	val n = norm
	Primes.primeFactors(n)
      }

    /*
     * This is the theorem at page 90
     * 
     * k such that intMod() == 0
     *
     * k is such that it "divides" 0 and this
     * Read the top of page 94
     */
    def findK: BigInt =
      {
	val p = norm
	require(p.isProbablePrime(10))
	require((p mod dim) == BigInt(1))

	val mu = (p - 1) / dim

	val first = BigInt(2).modPow(mu, p)

	// First is always a solution (Fermat's little theorem)
	// We loop through all other solutions
	// until we find the only one that divides this
	// all the other solutions "divide" this->conjugates

	var k = first

	do
	  {
	    if (intMod(k, p) == BigInt(0))
	      return k
	    k = (k * first) mod p
	  }
	while (k != 1)

	throw new AssertionError("Internal algorithm failure")
      }

    def square: Cyclotomic =
      {
	this * this
      }

    // exponentiation by squaring
    def pow(k: Int): Cyclotomic =
      {
	require(k >= 0)
	k match
	{
	  case 0                 => Cyclotomic.one(dim)
	  case n if (n % 2 == 0) => pow(n / 2).square
	  case n                 => this * pow((n - 1) / 2).square
	}
      }

    def intMod(k: BigInt, p: BigInt): BigInt =
      {
	var sum: BigInt = 0
	for (i <- (dim - 1).to(0, -1))
	  {
	    sum *= k
	    sum += coeffs(i)
	    sum = sum.mod(p)
	  }
	sum
      }

    def intMod(c: Cyclotomic): BigInt =
      {
	val k = c.findK
	val p = c.norm
	intMod(k, p)
      }

    // we have many ways of achieving this
    // here, we simple adjust the constant
    def makeDivisible(c: Cyclotomic) =
      {
	val r = intMod(c)
	this - Cyclotomic.integer(dim, r)
      }

    // calculate the norm by successive factorizations
    // This is the algorithm at the bottom of page 103
    def quickNorm =
      {
	val h = Primes.primitiveRoot(dim)

	def job(g: Cyclotomic, total: Int, d: Int, factors: List[BigInt]): Cyclotomic =
	  {
	    val step = total / d
	    var m = g

	    for (i <- 1 to d - 1)
	      {
		val r = h.modPow(step * i, dim).intValue
		m *= g.rotate(r)
	      }

	    if (factors.isEmpty)
	      m
	    else
	      job(m, step, factors.head.intValue, factors.tail)
	  }

	val factors = Primes.primeFactors(dim - 1)

	val nrm = job(this, dim - 1, factors.head.intValue, factors.tail)
	nrm.asInteger
      }
  }
  
  object Cyclotomic
  {

    def integer(dim: Int, n: BigInt) =
      {
	new Cyclotomic(dim, Array(n))
      }

    def one(dim: Int) =
      {
	integer(dim, 1)
      }
    
    def zero(dim: Int) =
      {
	integer(dim, 0)
      }
    
    def binomial(dim: Int, x: BigInt, y: BigInt) =
      {
	new Cyclotomic(dim, Array(x, y))
      }

    def biNorm(dim: Int, x: BigInt, y: BigInt) =
      {
	if (x + y == 0)
	  dim * x.abs.pow(dim - 1)
	else
	  (x.pow(dim) + y.pow(dim)) / (x + y)
      }

    // find al ks relative to this prime norm
    def ks(dim: Int, p: BigInt) =
      {
	require(p.isProbablePrime(10))
	require((p mod dim) == BigInt(1))

	val mu = (p - 1) / dim

	val first = BigInt(2).modPow(mu, p)

	var ks: List[BigInt] = Nil
	var k = first

	do
	  {
	    ks = k :: ks
	    k = (k * first) mod p
	  }
	while (k != 1)

	ks
      }

    // we return a stream of all possible norms
    def norms(dim: Int): Stream[BigInt] =
      {
	def norm1(): Stream[BigInt] =
	  {
	    def n(A: Int, B: Int): Stream[BigInt] =
	      {
		val q = A * A - dim * B * B
		if (q < 0)
		  {
		    // start from the beginning with B = 0 or 1
		    n(A + 1, (A + 1) % 2)
		  }
		else if (q % 4 != 0)
		  {
		    // skip this one
		    n(A, B + 2)
		  }
		else
		  {
		    // accept and go to next
		    val w = q / 4
		    w #:: n(A, B + 2)
		  }
	      }
	    n(0, 0)
	  }
	def norm3(): Stream[BigInt] =
	  {
	    def n(A: Int, B: Int): Stream[BigInt] =
	      {
		val q = A * A + dim * B * B
		if (B > A)
		  {
		    // start from the beginning with B = 0 or 1
		    n(A + 1, (A + 1) % 2)
		  }
		else if (q % 4 != 0)
		  {
		    // skip this one
		    n(A, B + 2)
		  }
		else
		  {
		    // accept and go to next
		    val w = q / 4
		    w #:: n(A, B + 2)
		  }
	      }
	    n(0, 0)
	  }

	require(BigInt(dim).isProbablePrime(10))
	if (dim % 4 == 1)
	  {
	    // The norm is 1/4 (A^2 - lambda B^2)
	    norm1
	  }
	else
	  {
	    // The norm is 1/4 (A^2 + lambda B^2)
	    norm3
	  }
	// with A & B of the same parity
      }
  }
}
