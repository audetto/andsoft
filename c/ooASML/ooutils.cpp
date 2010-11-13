/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "ooutils.h"

#include <rtl/ustring.hxx>
#include <com/sun/star/uno/RuntimeException.hpp>
#include <boost/circular_buffer.hpp>

namespace
{
    boost::circular_buffer<std::string> & getBuffer()
    {
        static boost::circular_buffer<std::string> buffer(10);
        return buffer;
    }
}

namespace ASI
{
    void ooThrow(const char * msg, const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >& context)
    {
        getBuffer().push_back(msg);
        throw ::com::sun::star::uno::RuntimeException( ::rtl::OUString::createFromAscii(msg), context);
    }

    void getExceptions(std::vector<std::string> & e)
    {
        e.assign(getBuffer().rbegin(), getBuffer().rend());
    }

}
