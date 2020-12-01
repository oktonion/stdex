#ifndef _STDEX_CTIME_H
#define _STDEX_CTIME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
/*none*/

// POSIX includes
/*none*/

// std includes
#include <ctime>

#if !defined(NULL)
    #error "stdex:: non-conforming <ctime> header: no NULL"
#endif

#if !defined(CLOCKS_PER_SEC)
    #error "stdex:: non-conforming <ctime> header: no CLOCKS_PER_SEC"
#endif

#if !defined(STDEX_NULL)
    #define STDEX_NULL NULL
#endif

#if !defined(STDEX_CLOCKS_PER_SEC)
    #define STDEX_CLOCKS_PER_SEC CLOCKS_PER_SEC
#endif

#if !defined(TIME_UTC)
    #define TIME_UTC 1
#endif

#define STDEX_TIME_UTC TIME_UTC

struct timespec;

namespace stdex
{
    namespace ctime_cpp17
    {
#ifndef STDEX_DO_NOT_ADD_CPP17_STD // define to exclude std implementations
        using namespace std;
#endif
    }

    // Types
    using std::clock_t;
    using std::size_t;
    using std::time_t;
    using std::tm;

    namespace detail
    {
        namespace ctime_detail
        {
            namespace std_dummy
            {
                using namespace std;

                struct _dummy_2int
                {
                    int _dummy[2];
                };

                _dummy_2int timespec_get(...);
            }

            namespace ctime_std_dummy
            {
                using namespace std_dummy;

                struct _has_timespec
                {
                    static const bool value = 
                        sizeof(timespec_get(0, 0)) != sizeof(_dummy_2int);
                };
            }

            using ctime_std_dummy::_has_timespec;
        }

        template<bool>
        struct _timespec_impl
        {
            typedef ::timespec timespec;
        };

        template<>
        struct _timespec_impl<false>
        {
            struct timespec
            {
                stdex::time_t tv_sec;
                long tv_nsec;
            };
        };
    }
    
    typedef
    detail::_timespec_impl<
        detail::ctime_detail::_has_timespec::value>::timespec
    timespec;

    // Time manipulation
    using std::clock;
    using std::time;
    using std::difftime;

    namespace ctime_cpp17
    {
        int timespec_get(stdex::timespec* ts, int base); // C++ 17
    }

    using ctime_cpp17::timespec_get;

    // Format conversions
    using std::ctime;
    using std::asctime;
    using std::strftime;
    //using std::wcsftime; // Defined in header <cwchar>
    using std::gmtime;
    using std::localtime;
    using std::mktime;
}

#endif // _STDEX_CTIME_H
