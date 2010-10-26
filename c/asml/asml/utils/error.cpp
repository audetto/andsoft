/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
#include <asml/utils/error.h>
#include <stdexcept>

using namespace std;

namespace ASI
{
    void error(const string & msg)
    {
        throw std::runtime_error(msg);
    }
    
    void error(const ostringstream & msg)
    {
        error(msg.str());
    }
}
