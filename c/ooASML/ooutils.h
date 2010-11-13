/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_OO_UTILS_H
#define ASI_OO_UTILS_H

#include <com/sun/star/uno/Reference.h>
#include <vector>
#include <string>

namespace ASI
{
    void ooThrow(const char * msg, const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >& context) __attribute__ ((__noreturn__));
    
    void getExceptions(std::vector<std::string> & e);
    
}

#define WRAP_BEGIN try {
#define WRAP_END   } catch (const std::exception & e) { ASI::ooThrow(e.what(), *this); }

#endif
