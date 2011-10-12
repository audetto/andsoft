import scala.collection.mutable.StringBuilder

package asi.algebra
{
  class Cyclotomic(d: Int, c: Array[Int])
  {
    require(Primes.isPrime(d))
    val dim: Int = d

    val coeffs: Array[Int] = new Array[Int](dim)
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
	val c = new Array[Int](dim)
	
	for (i <- 0 until dim)
	  {
	    c(i) = coeffs(i) + that.coeffs(i);
	  }
	
	new Cyclotomic(dim, c);
      }
    
    def * (that: Cyclotomic) = 
      {
	require(that.dim == dim)
	val c = new Array[Int](2 * dim - 1)
	
	for (i <- 0 until dim)
	  {
	    for (j <- 0 until dim)
	      {
		c(i + j) += coeffs(i) * that.coeffs(j)
	      }
	  }
	
	new Cyclotomic(dim, c)
      }

    def * (that: Int) = 
      {
	val c = new Array[Int](dim)
	
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

    def / (that: Int): Option[Cyclotomic] =
      {
	/*
	 * Since they are always normalised,
	 * there is one coeff = 0, then rem = 0
	 */

	val rem = coeffs(0) % that;

	val c = new Array[Int](dim)

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
	val c = new Array[Int](pos * dim)
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

  }
  
  object Cyclotomic
  {

    def integer(dim: Int, n: Int) =
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
    
    def binomial(dim: Int, x: Int, y: Int)
      {
	new Cyclotomic(dim, Array(x, y))
      }

    def biNorm(dim: Int, x: Int, y: Int)
      {
	if (x + y == 0)
	  dim * math.abs(x) * (dim - 1)
	else
	  (x * dim + y * dim) / (x + y)
      }
  }
}
