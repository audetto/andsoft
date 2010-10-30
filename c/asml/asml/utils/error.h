/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_ERROR_H
#define ASI_ERROR_H

#include <sstream>

#define THROW_EXCEPTION(MSG) do { std::ostringstream out; out << MSG; ASI::error(out); } while (false)

namespace ASI
{
    void __attribute__ ((__noreturn__)) error(const std::string & msg);
    void __attribute__ ((__noreturn__)) error(const std::ostringstream & msg);
}

#endif
