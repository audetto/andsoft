/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

// the only header you need to use QuantLib
#define BOOST_LIB_DIAGNOSTIC
#  include <ql/quantlib.hpp>
#undef BOOST_LIB_DIAGNOSTIC

#include "MCTest.h"

#include <iostream>
#include <iomanip>

using namespace QuantLib;
using namespace AndSoft;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib
{

    Integer sessionId()
    {
        return 0;
    }

}
#endif

int main(int, char* [])
{

    QL_TRACE_ENABLE;
    
    try
    {
        mcTest();
        return 0;
    }
    catch (Error& err)
    {
        std::cout << "QuantLib::Error: " << err.what() << std::endl;
        return 1;
    }
    catch (std::exception& e)
    {
        std::cout << "std::exception: " << e.what() << std::endl;
        return 2;
    } 
    catch (...)
    {
        std::cout << "unknown error" << std::endl;
        return 3;
    }
}
