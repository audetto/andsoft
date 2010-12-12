function theDates()
{
    var d = Array(4);
    d[0] = 40523;
    d[1] = 40623;
    d[2] = 40723;
    d[3] = 40823;

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
