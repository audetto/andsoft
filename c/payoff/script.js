function theDates()
{
    var d = Array(4);
    d[0] = 40823;
    d[1] = 40923;
    d[2] = 41023;
    d[3] = 41123;

    return d;
}

var d = theDates();

function dates()
{
    return d;
}

function payoff(data, ts)
{
    var numOfAssets = data.length;
    var numOfTimes  = data[0].length;

    var payments = Array(numOfTimes);

    var n2;

    function s(i)
    {
	return data[i][n];
    }

    function pay(val)
    {
	payments[n] = val;
    }

    function df(mat)
    {
	return ts.df(n, mat);
    }

    var runningMaximum = 1.0;
    var thisSum;

    for (i = 0; i < numOfTimes; ++i)
    {
	thisSum = 0.0;
        for (j = 0; j < numOfAssets; ++j)
	    thisSum += data[j][i];
	thisSum /= numOfAssets;

	if (thisSum > runningMaximum)
	    runningMaximum = thisSum;
    }

    var payoff = runningMaximum - thisSum;

    payments[numOfTimes - 1] = payoff;

    return payments;
}
