#ifndef _STDEX_SSTREAM_H
#define _STDEX_SSTREAM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
/*none*/

// POSIX includes
/*none*/

#ifdef _STDEX_NATIVE_CPP_98_SUPPORT
// std includes
#include <sstream>

namespace stdex
{
    using std::basic_istringstream;
    using std::basic_ostringstream;
    using std::basic_stringstream;
    using std::basic_stringbuf;

    using std::istringstream;
    using std::ostringstream;
    using std::stringstream;
    using std::stringbuf;

    using std::wistringstream;
    using std::wostringstream;
    using std::wstringstream;
    using std::wstringbuf;
}
#else
// std includes
#include <strstream>

namespace stdex
{
    //typedef basic_istringstream;
    //typedef basic_ostringstream;
    //typedef basic_stringstream;
    //typedef basic_stringbuf;

    //typedef istringstream;
    //typedef ostringstream;
    typedef std::strstream stringstream;
    //typedef stringbuf;

    //typedef wistringstream;
    //typedef wostringstream;
    typedef stringstream wstringstream; // temporarily, need to be redone
    //typedef wstringbuf;
}


#endif // _STDEX_NATIVE_CPP_98_SUPPORT

#endif // _STDEX_SSTREAM_H
