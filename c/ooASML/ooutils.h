#ifndef ASI_OO_UTILS_H
#define ASI_OO_UTILS_H

#include <rtl/ustring.hxx>

#include <string>

namespace ASI
{


}

#define WRAP_BEGIN try {
#define WRAP_END   } catch (const std::exception & e) { throw lang::IllegalArgumentException(); }

#endif
