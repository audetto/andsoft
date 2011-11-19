package asi.algebra
{

  object Algorithms
  {

    def sqrt(num: BigInt) =
      {
	def next(n: BigInt) : BigInt = (n + num / n) >> 1
	
	val one = BigInt(1)
	
	// start with the lower bound of power of 2
	var n = one << (num.bitLength / 2)
	var n1 = next(n)
	
	while ((n1 - n).abs > one)
	  {
	    n = n1
	    n1 = next(n)
	  }
	
	while (n1 * n1 > num)
	  {
	    n1 -= one
	  }

	// just to be sure
	require ((n1 * n1 <= num) && ((n1 + 1) * (n1 + 1) > num))
	
	n1
      }

    /*
     * Upper value of the sqrt.
     * sqrt * sqrt >= num
     */
    def sqrtU(num: BigInt) =
      {
	val k = sqrt(num)
	if (k * k < num)
	  k + 1
	else
	  k
      }
  }
}
