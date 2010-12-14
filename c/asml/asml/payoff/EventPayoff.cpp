/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/payoff/EventPayoff.h>
#include <asml/utils/error.h>
#include <fstream>

using namespace std;
using namespace QuantLib;

namespace
{
    using namespace ASI;

    void writeTempFile(const string                  & name,
                       const vector<Date>            & dates,
                       const vector<vector<string> > & events)
    {
        ofstream of(name.c_str());
        if (!of)
            THROW_EXCEPTION("Cannot open temporary file");

        const size_t numberOfDates = dates.size();

        if (events.size() != numberOfDates)
            THROW_EXCEPTION("Wrong number of events");

        const size_t numberOfLegs  = events[0].size();

        of << "function theDates()" << endl;
        of << "{" << endl;
        of << "    var d = Array(" << numberOfDates << ");" << endl;
        
        for (size_t i = 0; i < numberOfDates; ++i)
        {
            of << "    d[" << i << "] = " << dates[i].serialNumber() << ";" << endl;
        }

        of << "    return d;" << endl;
        of << "}" << endl << endl;

        of << "var d = theDates();" << endl << endl;

        of << "function dates()" << endl;
        of << "{" << endl;
        of << "    return d;" << endl;
        of << "}" << endl << endl;

        of << "function payoff(data, ts)" << endl;
        of << "{" << endl;
        of << "    var numOfAssets = data.length;" << endl;
        of << "    var numOfTimes  = data[0].length;" << endl;
        of << "    var payments    = Array(numOfTimes);" << endl << endl;

        of << "    var n;" << endl;
        of << "    function s(i)" << endl;
        of << "    {" << endl;
        of << "        return data[i][n];" << endl;
        of << "    }" << endl;
        of << "    function pay(v)" << endl;
        of << "    {" << endl;
        of << "        payments[n] = v;" << endl;
        of << "    }" << endl;
        of << "    function df(d)" << endl;
        of << "    {" << endl;
        of << "        return ts.df(n, d);" << endl;
        of << "    }" << endl << endl;

        for (size_t i = 0; i < numberOfDates; ++i)
        {
            of << "    n = " << i << endl;
            for (size_t j = 0; j < numberOfLegs; ++j)
            {
                of << "    " << events[i][j] << endl;
            }
            of << endl;
        }        

        of << "    return payments;" << endl;
        of << "}" << endl;
    }

}

namespace ASI
{
    boost::shared_ptr<JSPathOption> createEventBasedPayoff(const vector<string>          & includes,
                                                           const vector<Date>            & dates,
                                                           const vector<vector<string> > & events)
    {
        const string name = "/tmp/payoff.js";

        writeTempFile(name, dates, events);
        boost::shared_ptr<JSPathOption> jsPathOption(new JSPathOption(includes, name));

        return jsPathOption;
    }
    
}
