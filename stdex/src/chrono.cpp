// stdex includes
#include "../include/chrono.hpp"
#include "../include/ctime.hpp"

// POSIX includes
//#include <time.h> // for clock_gettime
#include <pthread.h>


// std includes
#include <exception>
#include <cstdlib>
#include <numeric>
#include <cstring>

void timespec_add(timespec &result, const timespec &in)
{
    enum {BILLION = 1000000000};

    const stdex::time_t _ts_sec_max = 
        (std::numeric_limits<stdex::time_t>::max)();
    
    if(result.tv_sec == _ts_sec_max || result.tv_sec < 0 ||
        in.tv_sec == _ts_sec_max)
    {
        result.tv_sec = _ts_sec_max;
        result.tv_nsec = BILLION - 1;
        return;
    }

    timespec t2 = in;
    if (result.tv_nsec >= BILLION) {
        result.tv_nsec -= BILLION;
        result.tv_sec++;
    }
    if (t2.tv_nsec >= BILLION) {
        t2.tv_nsec -= BILLION;
        t2.tv_sec++;
    }
    result.tv_sec += t2.tv_sec;
    result.tv_nsec += t2.tv_nsec;
    if (result.tv_nsec >= BILLION) {
        result.tv_nsec -= BILLION;
        result.tv_sec++;
    }

    if(result.tv_sec < 0)
    {
        result.tv_sec = 0;
        result.tv_nsec = 0;
    }
    if(result.tv_nsec < 0)
    {
        result.tv_nsec = 0;
    }
}

#if defined(WIN32) || defined(_WIN32) // assuming we are on windows platform and have no realtime clock

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define _STDEX_UNDEF_LEAN_AND_MEAN
#endif
#include <windows.h>
#ifdef _STDEX_UNDEF_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef _STDEX_UNDEF_LEAN_AND_MEAN
#endif

#include <process.h>

typedef LONGLONG duration_long_long;
typedef ULONGLONG duration_ulong_long;

namespace WinAPI
{
    namespace kernel32_dll
    {
        static bool GetSystemTimePreciseAsFileTime(FILETIME* input);
        static bool GetSystemTimeAsFileTime(FILETIME* input);
    }
}

struct filetime_ptr
{
    FILETIME* value;
    filetime_ptr(FILETIME* input) :
        value(input) {}
};

filetime_ptr GetSystemTimeAsFileTime(filetime_ptr SystemTimeAsFileTimePtr)
{ // GetSystemTimeAsFileTime does not exist on WinCE
    
    if(!WinAPI::kernel32_dll::GetSystemTimeAsFileTime(SystemTimeAsFileTimePtr.value))
    {
        SYSTEMTIME st;

        GetSystemTime(&st);
        if (0 == SystemTimeToFileTime(&st, SystemTimeAsFileTimePtr.value))
        {
            SystemTimeAsFileTimePtr.value->dwLowDateTime = 0;
            SystemTimeAsFileTimePtr.value->dwHighDateTime = 0;
        }
    }
    return SystemTimeAsFileTimePtr;
}

filetime_ptr GetSystemTimePreciseAsFileTime(filetime_ptr SystemTimeAsFileTimePtr)
{ // GetSystemTimePreciseAsFileTime does not exist before Win8
    
    if(!WinAPI::kernel32_dll::GetSystemTimePreciseAsFileTime(SystemTimeAsFileTimePtr.value))
        GetSystemTimeAsFileTime(SystemTimeAsFileTimePtr.value);
    return SystemTimeAsFileTimePtr;
}

stdex::detail::_no_type operator,(filetime_ptr, stdex::detail::_yes_type);

namespace WinAPI
{
    namespace type_traits
    {
        struct _has_GetSystemTimePreciseAsFileTime
        {
            static const bool value =
                sizeof(::GetSystemTimePreciseAsFileTime((FILETIME*)(0)), stdex::detail::_yes_type()) == sizeof(stdex::detail::_yes_type);
        };

        struct _has_GetSystemTimeAsFileTime
        {
            static const bool value =
                sizeof(::GetSystemTimeAsFileTime((FILETIME*)(0)), stdex::detail::_yes_type()) == sizeof(stdex::detail::_yes_type);
        };
    }

#ifndef WINAPI
    #ifndef NTAPI
        #define _STDEX_WINAPI __stdcall
    #else
        #define _STDEX_WINAPI NTAPI
    #endif
#else
    #define _STDEX_WINAPI WINAPI
#endif

        extern "C"
        {
            typedef 
            void(_STDEX_WINAPI *GetSystemTimePreciseAsFileTimePtr)(FILETIME*);

            typedef 
            void(_STDEX_WINAPI *GetSystemTimeAsFileTimePtr)(FILETIME*);
        }

#undef _STDEX_WINAPI

    namespace kernel32_dll
    {
        bool GetSystemTimePreciseAsFileTime(FILETIME* input)
        {
            static HINSTANCE hKernel32 = GetModuleHandleW(L"kernel32.dll");
            if (hKernel32)
            {
                // GetSystemTimePreciseAsFileTime
                {
                    static FARPROC func = GetProcAddress(hKernel32, "GetSystemTimePreciseAsFileTime");
                    if (func)
                    {
                        reinterpret_cast<GetSystemTimePreciseAsFileTimePtr>(func)(input);
                        return true;
                    }
                }
            }
            return false;
        }

        bool GetSystemTimeAsFileTime(FILETIME* input)
        {
            static HINSTANCE hKernel32 = GetModuleHandleW(L"kernel32.dll");
            if (hKernel32)
            {
                // GetSystemTimeAsFileTime
                {
                    static FARPROC func = GetProcAddress(hKernel32, "GetSystemTimeAsFileTime");
                    if (func)
                    {
                        reinterpret_cast<GetSystemTimeAsFileTimePtr>(func)(input);
                        return true;
                    }
                }
            }
            return false;
        }
    }
    
    void GetSystemTimePreciseAsFileTime(FILETIME* input)
    {
        ::GetSystemTimePreciseAsFileTime(input);
    }
}

namespace clock_gettime_impl
{
    static LARGE_INTEGER get_abs_start_point_system()
    {
        LARGE_INTEGER jan_1_1970;

        // January 1, 1970 (start of Unix epoch) in "ticks"
        // 116444736000000000 in ULARGE_INTEGER (UTC)
        jan_1_1970.LowPart  = 0xD53E8000;
        jan_1_1970.HighPart = 0x019DB1DE;

        return jan_1_1970;
    }

    static LARGE_INTEGER get_abs_start_point_local()
    {
        struct lambdas
        {
            static LARGE_INTEGER get_current_date()
            {
                LARGE_INTEGER current_date;
                FILETIME ft;

                WinAPI::GetSystemTimePreciseAsFileTime(&ft);
                current_date.LowPart = ft.dwLowDateTime;
                current_date.HighPart = ft.dwHighDateTime;

                return current_date;
            }
        };

        static LARGE_INTEGER current_date = lambdas::get_current_date();

        return current_date;
    }

    template<bool>
    struct abs_start_point_impl
    {
        static LARGE_INTEGER get(){
            return get_abs_start_point_system();
        }


    };

    template<>
    struct abs_start_point_impl<false>
    {
        static LARGE_INTEGER get(){
            return get_abs_start_point_local();
        }
    };

    typedef abs_start_point_impl<sizeof(duration_long_long) * CHAR_BIT >= 64> abs_start_point;

#define _STDEX_CHRONO_CLOCK_REALTIME 0
#define _STDEX_CHRONO_CLOCK_MONOTONIC 1

    ::timespec system_time_to_timespec(const LARGE_INTEGER &stime)
    {
        timespec ts;

        LARGE_INTEGER ticks_per_sec_in_filetime;
        ticks_per_sec_in_filetime.QuadPart = 10000000; // a tick is 100ns

        LARGE_INTEGER delta_sec, delta_nsec;

        delta_sec.QuadPart = stime.QuadPart / ticks_per_sec_in_filetime.QuadPart;
        delta_nsec.QuadPart = (stime.QuadPart % ticks_per_sec_in_filetime.QuadPart) * 100; // ticks * 100ns

        while (delta_nsec.QuadPart > 999999999)
        {
            delta_nsec.QuadPart -= 999999999;
            delta_sec.QuadPart++;
        }

        LARGE_INTEGER _ts_sec_max;
        _ts_sec_max.QuadPart =
            (std::numeric_limits<stdex::time_t>::max)();

        if (delta_sec.QuadPart < _ts_sec_max.QuadPart)
        {
            ts.tv_sec =
                static_cast<stdex::time_t>(delta_sec.QuadPart);
            ts.tv_nsec =
                static_cast<long>(delta_nsec.QuadPart);
        }
        else
        {
            ts.tv_sec = (std::numeric_limits<stdex::time_t>::max)();
            ts.tv_nsec = 999999999;
        }

        return ts;
    }

    int clock_gettime_realtime(timespec& ts) // unix time since January 1, 1970
    {
        FILETIME    filetime;
        LARGE_INTEGER today;
        const LARGE_INTEGER start_point = abs_start_point::get();;

        {
            WinAPI::GetSystemTimePreciseAsFileTime(&filetime);
        }

        today.LowPart = filetime.dwLowDateTime;
        today.HighPart = filetime.dwHighDateTime;

        if (today.QuadPart < start_point.QuadPart)
            return -1;

        today.QuadPart -= start_point.QuadPart;
        //today.QuadPart = ((static_cast<long long>(today.HighPart)) << 32) + static_cast<long long>(today.LowPart) - start_point.QuadPart;

        ts = system_time_to_timespec(today);

        return 0;
    }

    static const LARGE_INTEGER& cache_freq()
    {
        struct lambdas {
            static LARGE_INTEGER get_qpf()
            {
                LARGE_INTEGER freq;

                if (0 == QueryPerformanceFrequency(&freq))
                    freq.QuadPart = 0;

                return freq;
            }
        };

        static const LARGE_INTEGER freq_cached =
            lambdas::get_qpf();

        return freq_cached;
    }

    static const LARGE_INTEGER& get_rel_start_point() {
        cache_freq();

        struct lambdas {
            static LARGE_INTEGER get_qpc()
            {
                LARGE_INTEGER point;

                if (0 == QueryPerformanceCounter(&point) || point.QuadPart < 0)
                    point.QuadPart = 0;

                return point;
            }
        };

        static const LARGE_INTEGER point_cached =
            lambdas::get_qpc();

        return point_cached;
    }

    struct rel_start_point
    {
        static const LARGE_INTEGER& get()
        {
            return get_rel_start_point();
        }
    };

    static struct _init_inittime
    {
        _init_inittime()
        {
            rel_start_point::get();
            abs_start_point::get();
        }
    } init_inittime;

    int clock_gettime_monotonic(timespec& ts) // relative time since program start
    {
        const LARGE_INTEGER& sp = rel_start_point::get();

        if (sp.QuadPart == 0)
            return clock_gettime_realtime(ts);

        LARGE_INTEGER end_point;
        if (0 == QueryPerformanceCounter(&end_point) || end_point.QuadPart < sp.QuadPart)
            return clock_gettime_realtime(ts);

        const LARGE_INTEGER& cached_freq_sec = cache_freq();
        LARGE_INTEGER sec_to_ns_ratio;
        sec_to_ns_ratio.QuadPart = LONGLONG(1000) * 1000 * 1000;
        LARGE_INTEGER delta;
        delta.QuadPart = (end_point.QuadPart - sp.QuadPart);

        LARGE_INTEGER delta_sec, delta_nsec;

        delta_sec.QuadPart = delta.QuadPart / cached_freq_sec.QuadPart;
        delta_nsec.QuadPart = ( (delta.QuadPart % cached_freq_sec.QuadPart) * sec_to_ns_ratio.QuadPart ) / cached_freq_sec.QuadPart;

        while (delta_nsec.QuadPart > 999999999)
        {
            delta_nsec.QuadPart -= 999999999;
            delta_sec.QuadPart++;
        }

        const stdex::time_t _ts_sec_max =
            (std::numeric_limits<stdex::time_t>::max)();

        if (delta_sec.QuadPart < _ts_sec_max)
        {
            ts.tv_sec =
                static_cast<stdex::time_t>(delta_sec.QuadPart);
            ts.tv_nsec =
                static_cast<long>(delta_nsec.QuadPart);
        }
        else
        {
            ts.tv_sec = _ts_sec_max;
            ts.tv_nsec = 999999999;
        }

        return 0;
}

    int clock_gettime(int clk_id, stdex::timespec* _ts) {
        ::timespec ts;
        ts.tv_sec = _ts->tv_sec;
        ts.tv_nsec = _ts->tv_nsec;
        int err = -1;
        if (clk_id == _STDEX_CHRONO_CLOCK_MONOTONIC)
            err = clock_gettime_monotonic(ts);
        else if (clk_id == _STDEX_CHRONO_CLOCK_REALTIME)
            err = clock_gettime_realtime(ts);
        _ts->tv_nsec = ts.tv_nsec;
        _ts->tv_sec = ts.tv_sec;
        return err;

    }

    stdex::timespec local_ts_to_system_ts(const stdex::timespec &_ts)
    {
        ::timespec ts;
        ts.tv_nsec = _ts.tv_nsec;
        ts.tv_sec = _ts.tv_sec;

        LARGE_INTEGER local_delta;
        local_delta.QuadPart = 
            abs_start_point::get().QuadPart - get_abs_start_point_system().QuadPart;

        ::timespec result = 
            system_time_to_timespec(local_delta);

        timespec_add(result, ts);

        stdex::timespec out;
        out.tv_sec = result.tv_sec;
        out.tv_nsec = result.tv_nsec;

        return out;
    }

}
int(*clock_gettime_func_pointer)(int, stdex::timespec*) = &clock_gettime_impl::clock_gettime;
using clock_gettime_impl::local_ts_to_system_ts;

#elif defined(__MACH__) && !defined(CLOCK_REALTIME)
#include <time.h>
#include <sys/time.h>       /* gettimeofday */
#include <mach/mach_time.h> /* mach_absolute_time */

typedef ::int64_t duration_long_long;
typedef ::uint64_t duration_ulong_long;

#define BILLION 1000000000L
#define MILLION 1000000L

#define NORMALISE_TIMESPEC( ts, uint_milli )            \
    {                                                \
        ts.tv_sec += uint_milli / 1000u;                \
        ts.tv_nsec += (uint_milli % 1000u) * MILLION;   \
        ts.tv_sec += ts.tv_nsec / BILLION;              \
        ts.tv_nsec = ts.tv_nsec % BILLION;              \
    }

static mach_timebase_info_data_t timebase = { 0, 0 }; /* numer = 0, denom = 0 */
static struct timespec           inittime = { 0, 0 }; /* nanoseconds since 1-Jan-1970 to init() */
static ::uint64_t                initclock;           /* ticks since boot to init() */

void init()
{
    struct timeval  micro;      /* microseconds since 1 Jan 1970 */

    if (mach_timebase_info(&timebase) != 0)
        std::abort();                            /* very unlikely error */

    if (gettimeofday(&micro, NULL) != 0)
        std::abort();                            /* very unlikely error */

    initclock = mach_absolute_time();

    inittime.tv_sec = micro.tv_sec;
    inittime.tv_nsec = micro.tv_usec * 1000;
}

static struct _init_inittime
{
    _init_inittime()
    {
        init();
    }
} init_inittime;

struct timespec get_abs_future_time_fine(unsigned milli)
{
    struct timespec future = { 0, 0 };     /* ns since 1 Jan 1970 to 1500 ms in future */
    ::uint64_t      clock;      /* ticks since init */
    ::uint64_t      nano;       /* nanoseconds since init */

    clock = mach_absolute_time() - initclock;
    nano = clock * (::uint64_t)timebase.numer / (uint64_t)timebase.denom;
    future = inittime;
    future.tv_sec += nano / BILLION;
    future.tv_nsec += nano % BILLION;
    NORMALISE_TIMESPEC( future, milli );
    return future;
}

int clock_gettime(int X, stdex::timespec *tv)
{
    ::timespec result = get_abs_future_time_fine(0);

    tv->tv_sec = result.tv_sec;
    tv->tv_nsec = result.tv_nsec;

    return (0);
}
int(*clock_gettime_func_pointer)(int, stdex::timespec*) = &clock_gettime;

stdex::timespec local_ts_to_system_ts(const stdex::timespec &ts)
{
    return ts;
}
#else

typedef long long duration_long_long;
typedef unsigned long long duration_ulong_long;

int clock_gettime(clockid_t, struct timespec*);
int my_clock_gettime(clockid_t X, stdex::timespec *tv)
{
    ::timespec result;
    result.tv_sec = tv->tv_sec;
    result.tv_nsec = tv->tv_nsec;

    int err = clock_gettime(X, &result);

    tv->tv_sec = result.tv_sec;
    tv->tv_nsec = result.tv_nsec;

    return err;
}
int(*clock_gettime_func_pointer)(clockid_t, stdex::timespec*) = &my_clock_gettime;

const stdex::timespec& local_ts_to_system_ts(const stdex::timespec &ts)
{
    return ts;
}

#endif

#ifdef CLOCK_MONOTONIC
    #undef _STDEX_CHRONO_CLOCK_MONOTONIC
    #define _STDEX_CHRONO_CLOCK_MONOTONIC CLOCK_MONOTONIC
    
    #ifndef _STDEX_CHRONO_CLOCK_MONOTONIC_EXISTS
        #define _STDEX_CHRONO_CLOCK_MONOTONIC_EXISTS
    #endif

#endif

#ifdef CLOCK_BOOTTIME 
    #undef _STDEX_CHRONO_CLOCK_MONOTONIC
    #define _STDEX_CHRONO_CLOCK_MONOTONIC CLOCK_BOOTTIME

    #ifndef _STDEX_CHRONO_CLOCK_MONOTONIC_EXISTS
        #define _STDEX_CHRONO_CLOCK_MONOTONIC_EXISTS
    #endif
#endif

#ifdef CLOCK_MONOTONIC_RAW  
    #undef _STDEX_CHRONO_CLOCK_MONOTONIC
    #define _STDEX_CHRONO_CLOCK_MONOTONIC CLOCK_MONOTONIC_RAW

    #ifndef _STDEX_CHRONO_CLOCK_MONOTONIC_EXISTS
        #define _STDEX_CHRONO_CLOCK_MONOTONIC_EXISTS
    #endif
#endif

#ifdef CLOCK_REALTIME
    #undef _STDEX_CHRONO_CLOCK_REALTIME
    #define _STDEX_CHRONO_CLOCK_REALTIME CLOCK_REALTIME

    #ifndef _STDEX_CHRONO_CLOCK_MONOTONIC_EXISTS
        #define _STDEX_CHRONO_CLOCK_MONOTONIC CLOCK_REALTIME
    #endif
#endif

#if defined(WIN32) || defined(_WIN32)
    LARGE_INTEGER performanceFrequency;

    const bool stdex::chrono::steady_clock::is_steady = QueryPerformanceFrequency(&performanceFrequency) != 0;
    const bool stdex::chrono::system_clock::is_steady = false;

#else

#ifdef _STDEX_CHRONO_CLOCK_MONOTONIC_EXISTS
    const bool stdex::chrono::steady_clock::is_steady = true;
    const bool stdex::chrono::system_clock::is_steady = 
        (_STDEX_CHRONO_CLOCK_MONOTONIC == _STDEX_CHRONO_CLOCK_REALTIME);
#else
    const bool stdex::chrono::steady_clock::is_steady = false;
    const bool stdex::chrono::system_clock::is_steady = false;
#endif

#endif // not windows

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_NOEXCEPT_FUNCTION noexcept

#else

#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif

namespace stdex {
    namespace chrono {
        namespace detail {

            using stdex::detail::_declptr;

            template<class _LhsT, class _RhsT,
                bool _BothAreSigned,
                bool _LhsIsSigned>
            struct cmp_less_impl;

            template<class _LhsT, class _RhsT>
            struct cmp_less_impl<_LhsT, _RhsT, 
                false, false>
            {
                bool operator()(_LhsT lhs, _RhsT rhs) const throw()
                {
                    typedef typename stdex::make_unsigned<_RhsT>::type type;

                    return rhs < 0 ? false : lhs < type(rhs);
                }
            };

            template<class _LhsT, class _RhsT>
            struct cmp_less_impl<_LhsT, _RhsT, 
                false, true>
            {
                bool operator()(_LhsT lhs, _RhsT rhs) const throw()
                {
                    typedef typename stdex::make_unsigned<_LhsT>::type type;

                    return lhs < 0 ? true : type(lhs) < rhs;
                }
            };

            template<class _LhsT, class _RhsT, bool _LhsIsSigned>
            struct cmp_less_impl<_LhsT, _RhsT, 
                true, _LhsIsSigned>
            {
                bool operator()(_LhsT lhs, _RhsT rhs) const throw()
                {
                    return lhs < rhs;
                }
            };

            template<class _LhsT, class _RhsT>
            struct cmp_less_functor:
                cmp_less_impl<_LhsT, _RhsT,
                    (stdex::is_signed<_LhsT>::value == stdex::is_signed<_RhsT> ::value),
                    stdex::is_signed<_LhsT>::value>
            { };

            template<class _LhsT, class _RhsT>
            static bool cmp_less(_LhsT lhs, _RhsT rhs) throw()
            {
                cmp_less_functor<_LhsT, _RhsT> impl;
                return impl(lhs, rhs);
            }

            template<class _LhsT, class _RhsT>
            static bool cmp_greater(_LhsT lhs, _RhsT rhs) throw()
            {
                return cmp_less(rhs, lhs);
            }

            duration_long_long convert(const _big_int &value)
            {
                duration_long_long result;
                std::memcpy(&result, value.least64_value, sizeof(duration_long_long));
                return result;
            }

            _big_int convert(const duration_long_long& value)
            {
                _big_int result;
                std::memcpy(result.least64_value, &value, sizeof(duration_long_long));
                return result;
            }

            _big_int::_big_int(stdex::intmax_t _value)
            {
                STATIC_ASSERT(sizeof(duration_long_long) <= sizeof(_declptr<_big_int>()->least64_value), platform_specific_int64_should_fit_in_8_bytes);
                duration_long_long value = _value;
                std::memcpy(least64_value, &value, sizeof(duration_long_long));
            }

            _big_int::_big_int(const _big_int& other)
            {
                std::memcpy(least64_value, other.least64_value, sizeof(least64_value));
            }

            _big_int& _big_int::operator=(const _big_int& other)
            {
                std::memcpy(least64_value, other.least64_value, sizeof(least64_value));
                return *this;
            }

            stdex::intmax_t _big_int::to_integer() const
            {
                duration_long_long result = convert(*this);

                if (cmp_greater( result, (std::numeric_limits<stdex::intmax_t>::max)() ))
                    throw(std::out_of_range("overflow in stdex::chrono::duration cast to stdex::intmax_t"));

                return static_cast<stdex::intmax_t>(result);
            }

            long double _big_int::to_floating_point() const
            {
                duration_long_long result = convert(*this);
                // long double range is always wider than long long range 
                return static_cast<long double>(result);
            }

            _big_int _big_int::operator+() const
            {
                return (*this);
            }

            _big_int _big_int::operator-() const
            {
                return convert(-convert(*this));
            }

            _big_int& _big_int::operator++()
            {
                *this = convert(convert(*this) + 1);
                return *this;
            }

            _big_int _big_int::operator++(int)
            {
                _big_int tmp(*this);
                operator++();
                return tmp;
            }

            _big_int& _big_int::operator--()
            {
                *this = convert(convert(*this) - 1);
                return *this;
            }

            _big_int _big_int::operator--(int)
            {
                _big_int tmp(*this);
                operator--();
                return tmp;
            }

            _big_int& _big_int::operator+=(const _big_int& other)
            {
                *this = convert(convert(*this) + convert(other));
                return *this;
            }

            _big_int& _big_int::operator-=(const _big_int& other)
            {
                *this = convert(convert(*this) - convert(other));
                return *this;
            }

            _big_int& _big_int::operator*=(const _big_int& other)
            {
                *this = convert(convert(*this) * convert(other));
                return *this;
            }

            _big_int& _big_int::operator/=(const _big_int& other)
            {
                *this = convert(convert(*this) / convert(other));
                return *this;
            }

            _big_int& _big_int::operator%=(const _big_int& other)
            {
                *this = convert(convert(*this) % convert(other));
                return *this;
            }

            _big_int operator+(_big_int a, const _big_int& b)
            {
                return a += b;
            }

            _big_int operator-(_big_int a, const _big_int& b)
            {
                return a -= b;
            }

            _big_int operator*(_big_int a, const _big_int& b)
            {
                return a *= b;
            }

            _big_int operator/(_big_int a, const _big_int& b)
            {
                return a /= b;
            }

            _big_int operator%(_big_int a, const _big_int& b)
            {
                return a %= b;
            }
            
            bool operator< (const _big_int& a, const _big_int& b)
            {
                return convert(a) < convert(b);
            }

            bool operator> (const _big_int& a, const _big_int& b)
            {
                return convert(a) > convert(b);
            }

            bool operator==(const _big_int& a, const _big_int& b)
            {
                return convert(a) == convert(b);
            }

            bool operator!=(const _big_int& a, const _big_int& b)
            {
                return convert(a) != convert(b);
            }

            bool operator>=(const _big_int& a, const _big_int& b)
            {
                return convert(a) >= convert(b);
            }

            bool operator<=(const _big_int& a, const _big_int& b)
            {
                return convert(a) <= convert(b);
            }
        } // namespace detail
    } // namespace chrono
} // namespace stdex

stdex::timespec
    stdex::chrono::system_clock::to_timespec(const time_point &_t) _STDEX_NOEXCEPT_FUNCTION
{
    chrono::time_point<system_clock, chrono::seconds> _s = 
        chrono::time_point_cast<chrono::seconds>(_t);
    chrono::nanoseconds _ns = 
        chrono::duration_cast<chrono::nanoseconds>(_t - _s);

    stdex::time_t _s_count = 
        detail::_chrono_convert<stdex::time_t>(
            detail::duration_count(_s.time_since_epoch()), detail::chrono_detail::_priority_tag<4>() );

    stdex::timespec _ts;

    _ts.tv_sec = _s_count;
    _ts.tv_nsec = static_cast<long>(_ns.count());

    return local_ts_to_system_ts(_ts);

    return _ts;
}


static
duration_long_long
days_from_civil(duration_long_long y, duration_ulong_long m, duration_ulong_long d) _STDEX_NOEXCEPT_FUNCTION
{
    y -= m <= 2;
    const duration_long_long era = (y >= 0 ? y : y-399) / 400;
    const duration_ulong_long yoe = static_cast<duration_ulong_long>(y - era * 400);      // [0, 399]
    const duration_ulong_long doy = (153*(m + (m > 2 ? -3 : 9)) + 2)/5 + d-1;  // [0, 365]
    const duration_ulong_long doe = yoe * 365 + yoe/4 - yoe/100 + doy;         // [0, 146096]
    return era * 146097 + static_cast<duration_long_long>(doe) - 719468;
}

struct civil_date
{
    duration_long_long year;
    duration_ulong_long month;
    duration_ulong_long day;
};

static
civil_date
civil_from_days(duration_long_long z) _STDEX_NOEXCEPT_FUNCTION
{
    z += 719468;
    const duration_long_long era = (z >= 0 ? z : z - 146096) / 146097;
    const duration_ulong_long doe = static_cast<duration_ulong_long>(z - era * 146097);          // [0, 146096]
    const duration_ulong_long yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
    const duration_long_long y = static_cast<duration_long_long>(yoe) + era * 400;
    const duration_ulong_long doy = doe - (365*yoe + yoe/4 - yoe/100);                // [0, 365]
    const duration_ulong_long mp = (5*doy + 2)/153;                                   // [0, 11]
    const duration_ulong_long d = doy - (153*mp+2)/5 + 1;                             // [1, 31]
    const duration_ulong_long m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]

    civil_date result;
    result.year = y + (m <= 2);
    result.month = m;
    result.day = d;

    return result;
}

static
duration_ulong_long
weekday_from_days(duration_long_long z) _STDEX_NOEXCEPT_FUNCTION
{
    return static_cast<duration_ulong_long>(z >= -4 ? (z+4) % 7 : (z+5) % 7 + 6);
}

template <class To, class Rep, class Period>
To
round_down(const stdex::chrono::duration<Rep, Period>& d)
{
    To t = stdex::chrono::duration_cast<To>(d);
    if (t > d)
        --t;
    return t;
}

static
stdex::tm
make_utc_tm(stdex::chrono::system_clock::time_point tp)
{
    using namespace stdex::chrono;


    typedef         
    stdex::ratio_multiply<
        hours::period,
        stdex::ratio<24>
    >::type days_period;

    typedef 
    stdex::chrono::duration<
        stdex::chrono::system_clock::rep,
        days_period
    > days;

    // t is time duration since 1970-01-01
    stdex::chrono::system_clock::duration t = tp.time_since_epoch();
    // d is days since 1970-01-01
    days d = round_down<days>(t);
    // t is now time duration since midnight of day d
    t -= d;
    // break d down into year/month/day
    civil_date civ_date = civil_from_days(d.count());
    // start filling in the tm with calendar info
    std::tm tm;
    tm.tm_isdst = 0;
    tm.tm_year = civ_date.year - 1900;
    tm.tm_mon = civ_date.month - 1;
    tm.tm_mday = civ_date.day;
    tm.tm_wday = weekday_from_days(d.count());
    tm.tm_yday = d.count() - days_from_civil(civ_date.year, 1, 1);
    // Fill in the time
    tm.tm_hour = duration_cast<hours>(t).count();
    t -= hours(tm.tm_hour);
    tm.tm_min = duration_cast<minutes>(t).count();
    t -= minutes(tm.tm_min);
    tm.tm_sec = duration_cast<seconds>(t).count();
    return tm;
}

static
stdex::tm get_time_t_epoch()
{
    const stdex::chrono::system_clock::time_point stdex_sys_clock_tp_epoch = 
        stdex::chrono::system_clock::time_point(0);
    stdex::tm stdex_tm_time_point = 
        make_utc_tm(stdex_sys_clock_tp_epoch);
    const stdex::time_t time_t_epoch(0);
    const stdex::time_t invalid_time_t(-1);
    stdex::time_t time_t_time_point = invalid_time_t;
    do{   
        stdex_tm_time_point.tm_year++;
        time_t_time_point = stdex::mktime(&stdex_tm_time_point);
    }
    while(invalid_time_t == time_t_time_point);

    const double secs_since_time_t_epoch = 
        stdex::difftime(time_t_time_point, time_t_epoch);
    
    stdex::chrono::seconds
        secs_since_tm_epoch = // since 1 jan 1900
            stdex::chrono::hours(stdex_tm_time_point.tm_yday * 24 * stdex_tm_time_point.tm_year) +
            stdex::chrono::hours(stdex_tm_time_point.tm_hour) +
            stdex::chrono::minutes(stdex_tm_time_point.tm_min) +
            stdex::chrono::seconds(stdex_tm_time_point.tm_sec);
    stdex::chrono::seconds secs_since_tm_epoch_to_tt_epoch =
        secs_since_tm_epoch -
        stdex::chrono::seconds(stdex::chrono::seconds::rep(secs_since_time_t_epoch));

    stdex_tm_time_point =
        make_utc_tm(stdex_sys_clock_tp_epoch + secs_since_tm_epoch);

    
    return stdex_tm_time_point;
}

static
int get_UTC_shift()
{
    static stdex::tm time_t_epoch = get_time_t_epoch();

    stdex::tm stdex_tm_time_point = time_t_epoch;
    stdex::time_t time_t_time_point = stdex::mktime(&time_t_epoch);
    const stdex::time_t invalid_time_t(-1);

    int UTC_shift = 0;

    while (invalid_time_t == time_t_time_point)
    {
        stdex_tm_time_point.tm_hour++;
        UTC_shift++;
        time_t_time_point = stdex::mktime(&stdex_tm_time_point);
    }

    return UTC_shift;
}

stdex::time_t
    stdex::chrono::system_clock::to_time_t(const time_point &_t) _STDEX_NOEXCEPT_FUNCTION
{
    int UTC_shift = get_UTC_shift();
    stdex::tm stdex_sys_clock_epoch = make_utc_tm(time_point(0));

    stdex::tm UTC_result = make_utc_tm(_t + hours(UTC_shift));
    return 
        stdex::mktime(&UTC_result);
}


stdex::chrono::system_clock::time_point
    stdex::chrono::system_clock::from_time_t(stdex::time_t _t) _STDEX_NOEXCEPT_FUNCTION
{
    typedef chrono::time_point<system_clock, seconds>    _from;

    int UTC_shift = get_UTC_shift();
    stdex::tm stdex_sys_clock_epoch = make_utc_tm(time_point(0) + hours(UTC_shift));
    stdex_sys_clock_epoch.tm_mon++;
    stdex_sys_clock_epoch.tm_yday = 31;
    stdex::time_t stdex_sys_clock_epoch_tt = stdex::mktime(&stdex_sys_clock_epoch);

    double seconds_from_epoch = 
        stdex::difftime(_t, stdex_sys_clock_epoch_tt);
    const duration_long_long sec_in_jan = (31 * 24 * 60 * 60);
      
    stdex::chrono::detail::_big_int result = 
        stdex::chrono::detail::convert(
            static_cast<duration_long_long>(duration_long_long(0) + duration_long_long(seconds_from_epoch) + sec_in_jan)
        );

    return time_point_cast<system_clock::duration>
        (_from(chrono::seconds(result)));
}

stdex::chrono::system_clock::time_point stdex::chrono::system_clock::now() _STDEX_NOEXCEPT_FUNCTION
{    // get current time
    {
        stdex::timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 0;

        if ((*clock_gettime_func_pointer)(_STDEX_CHRONO_CLOCK_REALTIME, &ts) != 0)
        {
            std::terminate();
        }
        
        return
        time_point( 
            seconds(ts.tv_sec) + duration_cast<duration>(nanoseconds(ts.tv_nsec)) );
    }
}

stdex::chrono::steady_clock::time_point stdex::chrono::steady_clock::now() _STDEX_NOEXCEPT_FUNCTION
{    // get current time
    {
        stdex::timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 0;

        if ((*clock_gettime_func_pointer)(_STDEX_CHRONO_CLOCK_MONOTONIC, &ts) != 0)
        {
            std::terminate();
        }

        return
        time_point(
            seconds(ts.tv_sec) + duration_cast<duration>(nanoseconds(ts.tv_nsec)));
    }
}
