function payoff(data)
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
