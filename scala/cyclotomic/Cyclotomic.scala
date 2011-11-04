import scala.collection.mutable.StringBuilder

package asi.algebra
{
  class Cyclotomic(d: Int, c: Array[BigInt])
  {
    require(BigInt(d).isProbablePrime(5))

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
     * I am not 100% sure here
     * We look for the smallest integer (< norm)
     * such that intMod() == 0
     * This is the theorem at page 90
     */
    def findK: BigInt =
      {
	val p = norm
	require(p.isProbablePrime(10))
	require((p mod dim) == BigInt(1))

	val expo = (p - 1) / dim

	// should we loop over a or k
	// a: is it enough to loop up to p?
	//    we know (?) k < p, what about a
	// k: should we check k.modPow(dim, p) == 1?
	//    which is automatic if we loop over a
	//    in that case, are we checking a lot of useless numbers?
	// moreover: is the smallest such a number the correct one?
	for (a <- BigInt(2) to p)
	  {
	    val k = a.modPow(expo, p)
	    if (intMod(k, p) == BigInt(0))
	      return k
	  }

	throw new AssertionError("Internal algorithm failure")
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
  }
}
