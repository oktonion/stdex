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
#include <stdexcept>

void timespec_add(stdex::timespec &result, const stdex::timespec &in)
{
    enum {BILLION = 1000000000};

    const stdex::time_t _ts_sec_max = 
    #ifdef max
        (std::numeric_limits<stdex::time_t>::max)();
    #else
        std::numeric_limits<stdex::time_t>::max();
    #endif
    
    if(result.tv_sec == _ts_sec_max || result.tv_sec < 0 ||
        in.tv_sec == _ts_sec_max)
    {
        result.tv_sec = _ts_sec_max;
        result.tv_nsec = BILLION - 1;
        return;
    }

    stdex::timespec t2 = in;
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

//from <sdkddkver.h> SDK Win10:
#define _STDEX_WIN32_WINNT_NT4                    0x0400 // Windows NT 4.0
#define _STDEX_WIN32_WINNT_WIN2K                  0x0500 // Windows 2000
#define _STDEX_WIN32_WINNT_WINXP                  0x0501 // Windows XP
#define _STDEX_WIN32_WINNT_WS03                   0x0502 // Windows Server 2003
#define _STDEX_WIN32_WINNT_VISTA                  0x0600 // Windows Vista
#define _STDEX_WIN32_WINNT_WS08                   0x0600 // Windows Server 2008
#define _STDEX_WIN32_WINNT_WIN7                   0x0601 // Windows 7
#define _STDEX_WIN32_WINNT_WIN8                   0x0602 // Windows 8
#define _STDEX_WIN32_WINNT_WINBLUE                0x0603 // Windows 8.1
#define _STDEX_WIN32_WINNT_WIN10                  0x0A00 // Windows 10

#ifdef WINVER
#define _STDEX_WINVER WINVER
#else
#define _STDEX_WINVER 0x0
#endif

namespace WinAPI
{
    namespace type_traits
    {
        typedef stdex::bool_constant< (_STDEX_WINVER >= _STDEX_WIN32_WINNT_NT4) > target_is_windows_NT_or_later;
        typedef stdex::bool_constant< (_STDEX_WINVER >= _STDEX_WIN32_WINNT_WIN2K) > target_is_windows_2000_or_later;
        typedef stdex::bool_constant< (_STDEX_WINVER >= _STDEX_WIN32_WINNT_WINXP) > target_is_windows_XP_or_later;
        typedef stdex::bool_constant< (_STDEX_WINVER >= _STDEX_WIN32_WINNT_WS03) > target_is_windows_server_2003_or_later;
        typedef stdex::bool_constant< (_STDEX_WINVER >= _STDEX_WIN32_WINNT_VISTA) > target_is_windows_vista_or_later;
        typedef stdex::bool_constant< (_STDEX_WINVER >= _STDEX_WIN32_WINNT_WS08) > target_is_windows_server_2008_or_later;
        typedef stdex::bool_constant< (_STDEX_WINVER >= _STDEX_WIN32_WINNT_WIN7) > target_is_windows_7_or_later;
        typedef stdex::bool_constant< (_STDEX_WINVER >= _STDEX_WIN32_WINNT_WIN8) > target_is_windows_8_or_later;
        typedef stdex::bool_constant< (_STDEX_WINVER >= _STDEX_WIN32_WINNT_WINBLUE) > target_is_windows_8_1_or_later;
        typedef stdex::bool_constant< (_STDEX_WINVER >= _STDEX_WIN32_WINNT_WIN10) > target_is_windows_10_or_later;
    }

} // namespace WinAPI

#undef _STDEX_WIN32_WINNT_NT4         
#undef _STDEX_WIN32_WINNT_WIN2K       
#undef _STDEX_WIN32_WINNT_WINXP       
#undef _STDEX_WIN32_WINNT_WS03               
#undef _STDEX_WIN32_WINNT_VISTA       
#undef _STDEX_WIN32_WINNT_WS08         
#undef _STDEX_WIN32_WINNT_WIN7        
#undef _STDEX_WIN32_WINNT_WIN8        
#undef _STDEX_WIN32_WINNT_WINBLUE     
#undef _STDEX_WIN32_WINNT_WIN10   

#undef _STDEX_WINVER

typedef LONGLONG duration_long_long;
typedef ULONGLONG duration_ulong_long;

namespace WinAPI
{
    namespace kernel32_dll
    {
        static bool GetSystemTimePreciseAsFileTime(FILETIME* input);
        static bool GetSystemTimeAsFileTime(FILETIME* input);
    }

    void GetSystemTimePreciseAsFileTime(FILETIME* input);
    void GetSystemTimeAsFileTime(FILETIME* input);
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
    
    if (!WinAPI::kernel32_dll::GetSystemTimePreciseAsFileTime(SystemTimeAsFileTimePtr.value))
    {
        WinAPI::GetSystemTimeAsFileTime(SystemTimeAsFileTimePtr.value);
    }
    return SystemTimeAsFileTimePtr;
}

stdex::detail::_no_type operator,(filetime_ptr, stdex::detail::_yes_type);

namespace WinAPI
{
    /*namespace type_traits
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
    }*/

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
            } else
            {
                hKernel32 = GetModuleHandleW(L"kernel32.dll");
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
            } else
            {
                hKernel32 = GetModuleHandleW(L"kernel32.dll");
            }
            return false;
        }
    }

    namespace detail
    {
        template<bool>
        struct GetSystemTimePreciseAsFileTime_impl
        {
            // Windows8.1 or grater
            // NOTE: attempt to dynamically load GetSystemTimePreciseAsFileTime from kernel32.dll would be done anyway
            template<class _FileTime>
            static void call(_FileTime input)
            {
                ::GetSystemTimePreciseAsFileTime(input);
            }
        };

        template<>
        struct GetSystemTimePreciseAsFileTime_impl<false>
        {
            static void call(filetime_ptr input) // Windows 7 or less
            {
                ::GetSystemTimePreciseAsFileTime(input);
            }
        };

        template<bool>
        struct GetSystemTimeAsFileTime_impl
        {
            // Windows2000 or grater
            // NOTE: attempt to dynamically load GetSystemTimeAsFileTime from kernel32.dll would be done anyway
            template<class _FileTime>
            static void call(_FileTime input)
            {
                ::GetSystemTimeAsFileTime(input);
            }
        };

        template<>
        struct GetSystemTimeAsFileTime_impl<false>
        {
            static void call(filetime_ptr input) // Windows CE or less
            {
                ::GetSystemTimeAsFileTime(input);
            }
        };
    }
    
    void GetSystemTimePreciseAsFileTime(FILETIME* input)
    {
        detail::GetSystemTimePreciseAsFileTime_impl<type_traits::target_is_windows_10_or_later::value>::call(input);
    }

    void GetSystemTimeAsFileTime(FILETIME* input)
    {
        detail::GetSystemTimeAsFileTime_impl<type_traits::target_is_windows_2000_or_later::value>::call(input);
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

    stdex::timespec system_time_to_timespec(const LARGE_INTEGER &stime)
    {
        stdex::timespec ts;

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
        #ifdef max
            (std::numeric_limits<stdex::time_t>::max)();
        #else
            std::numeric_limits<stdex::time_t>::max();
        #endif

        if (delta_sec.QuadPart < _ts_sec_max.QuadPart)
        {
            ts.tv_sec =
                static_cast<stdex::time_t>(delta_sec.QuadPart);
            ts.tv_nsec =
                static_cast<long>(delta_nsec.QuadPart);
        }
        else
        {
            ts.tv_sec = 
            #ifdef max
                (std::numeric_limits<stdex::time_t>::max)();
            #else
                std::numeric_limits<stdex::time_t>::max();
            #endif
            ts.tv_nsec = 999999999;
        }

        return ts;
    }

    int clock_gettime_realtime(stdex::timespec& ts) // unix time since January 1, 1970
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

    int clock_gettime_monotonic(stdex::timespec& ts) // relative time since program start
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
        #ifdef max
            (std::numeric_limits<stdex::time_t>::max)();
        #else
            std::numeric_limits<stdex::time_t>::max();
        #endif

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
        stdex::timespec ts;
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
        stdex::timespec ts;
        ts.tv_nsec = _ts.tv_nsec;
        ts.tv_sec = _ts.tv_sec;

        LARGE_INTEGER local_delta;
        local_delta.QuadPart = 
            abs_start_point::get().QuadPart - get_abs_start_point_system().QuadPart;

        stdex::timespec result =
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
                const stdex::intmax_t intmax_max =
                #ifdef max
                    (std::numeric_limits<stdex::intmax_t>::max)();
                #else
                    std::numeric_limits<stdex::intmax_t>::max();
                #endif

                if (cmp_greater( result,  intmax_max))
                    throw(std::range_error("overflow in stdex::chrono::duration cast to stdex::intmax_t"));

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

            _big_int operator+(const _big_int& a, const _big_int& b)
            {
                return _big_int(a) += b;
            }

            _big_int operator-(const _big_int& a, const _big_int& b)
            {
                return _big_int(a) -= b;
            }

            _big_int operator*(const _big_int& a, const _big_int& b)
            {
                return _big_int(a) *= b;
            }

            _big_int operator/(const _big_int& a, const _big_int& b)
            {
                return _big_int(a) /= b;
            }

            _big_int operator%(const _big_int& a, const _big_int& b)
            {
                return _big_int(a) %= b;
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

namespace stdex 
{
    namespace chrono
    {
        template<class _Dur>
        static
        stdex::intmax_t _get_duration_count(
            typename conditional<
                detail::_is_duration<_Dur>::value,
                const _Dur&,
                seconds&
            >::type dur)
        {
            return dur.count();
        }

        template<class _Dur>
        static
        stdex::intmax_t _get_duration_count(
            typename conditional<
                detail::_is_duration<_Dur>::value,
                int,
                const _Dur&
            >::type)
        {
            return 0;
        }

        inline
        static void _get_ns(const nanoseconds &ns, stdex::timespec &out)
        {
            out.tv_nsec = static_cast<long>(_get_duration_count<nanoseconds>(ns));
        }

        inline
        static void _get_ns(const microseconds &mcs, stdex::timespec &out)
        {
            out.tv_nsec = static_cast<long>(_get_duration_count<microseconds>(mcs) * 1000);
        }

        inline
        static void _get_ns(const milliseconds &ms, stdex::timespec &out)
        {
            out.tv_nsec = static_cast<long>(_get_duration_count<milliseconds>(ms) * 1000 * 1000);
        }

        inline
        static void _get_ns(const seconds &sec, stdex::timespec &out)
        {
            out.tv_nsec = static_cast<long>(_get_duration_count<seconds>(sec) * 1000 * 1000 * 1000);
        }
    }
}

stdex::timespec
    stdex::chrono::system_clock::to_timespec(const time_point &_t) _STDEX_NOEXCEPT_FUNCTION
{
    chrono::time_point<system_clock, chrono::seconds> _s = 
        chrono::time_point_cast<chrono::seconds>(_t);

    stdex::timespec _ts;
    _ts.tv_nsec = 0;

    _ts.tv_sec = 
        detail::_chrono_convert_func::call(
            detail::_duration_count_func::call(_s.time_since_epoch()),
            stdex::detail::_chrono_force_tmpl_param<stdex::time_t>());

    _get_ns(_t - _s, _ts);

    return local_ts_to_system_ts(_ts);

    //return _ts;
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

namespace stdex
{
    namespace chrono
    {
        struct _duration_cast
        {
            template<class _To, class _From>
            static _To call(const _From &_from, 
                const stdex::detail::_chrono_force_tmpl_param<_To>&)
            {
                return duration_cast<_To>(_from);
            }
        };
    }
}

template <class To, class Rep, class Period>
static
To round_down(const stdex::chrono::duration<Rep, Period>& d)
{
    To t = 
        stdex::chrono::_duration_cast::call(
            d, 
            stdex::detail::_chrono_force_tmpl_param<To>()
        );
    if (t > d)
        --t;
    return t;
}

typedef         
stdex::ratio_multiply<
    stdex::chrono::hours::period,
    stdex::ratio<24>
>::type days_period;

typedef 
stdex::chrono::duration<
    stdex::chrono::system_clock::rep,
    days_period
> days;

static
stdex::tm
make_utc_tm(stdex::chrono::system_clock::time_point tp)
{
    using namespace stdex::chrono;

    // t is time duration since 1970-01-01
    system_clock::duration t = tp.time_since_epoch();
    // d is days since 1970-01-01
    days d = round_down<days>(t);
    // t is now time duration since midnight of day d
    t -= d;
    // break d down into year/month/day
    civil_date civ_date = civil_from_days(d.count());
    // start filling in the tm with calendar info
    stdex::tm result;
    result.tm_isdst = 0;
    result.tm_year = static_cast<int>(civ_date.year) - 1900;
    result.tm_mon = static_cast<int>(civ_date.month) - 1;
    result.tm_mday = static_cast<int>(civ_date.day);
    result.tm_wday = static_cast<int>( weekday_from_days(d.count()) );
    result.tm_yday = static_cast<int>( d.count() - days_from_civil(civ_date.year, 1, 1) );
    // Fill in the time
    result.tm_hour = static_cast<int>( duration_cast<hours>(t).count() );
    t -= hours(result.tm_hour);
    result.tm_min = static_cast<int>( duration_cast<minutes>(t).count() );
    t -= minutes(result.tm_min);
    result.tm_sec = static_cast<int>( duration_cast<seconds>(t).count() );

    return result;
}

static
stdex::tm get_time_t_epoch_with_UTC()
{
    const stdex::chrono::system_clock::time_point stdex_sys_clock_tp_epoch; // should be 1970-1-1
    stdex::tm stdex_tm_time_point = 
        make_utc_tm(stdex_sys_clock_tp_epoch);
    
    if(
        stdex_tm_time_point.tm_year != 70 ||
        stdex_tm_time_point.tm_mon != 0 ||
        stdex_tm_time_point.tm_mday != 1 ||
        stdex_tm_time_point.tm_hour != 0 ||
        stdex_tm_time_point.tm_min != 0 ||
        stdex_tm_time_point.tm_sec != 0
      )
    {
        std::abort();
    }

    const stdex::time_t time_t_epoch(0);
    const stdex::time_t invalid_time_t(-1);
    stdex::time_t time_t_time_point = invalid_time_t;
    short d_shift = -299;
    do{ 
        d_shift += 30;
        if(stdex_tm_time_point.tm_year > 70 + 100) 
            break; 
        stdex_tm_time_point = make_utc_tm(stdex_sys_clock_tp_epoch + days(d_shift));
        
        stdex_tm_time_point.tm_isdst = 0;
        stdex::tm tmp = stdex_tm_time_point;
        time_t_time_point = stdex::mktime(&tmp);
        if(invalid_time_t == time_t_time_point)
        {
            stdex_tm_time_point.tm_isdst = 1;
            tmp = stdex_tm_time_point;
            time_t_time_point = stdex::mktime(&tmp);
        }
    }
    while(invalid_time_t == time_t_time_point);

    if(invalid_time_t != time_t_time_point)
    {
        const double secs_since_time_t_epoch = 
            stdex::difftime(time_t_time_point, time_t_epoch) - ( (1 == stdex_tm_time_point.tm_isdst) ? (1.0 * 60.0 * 60.0) : 0.0 );
        
        stdex::chrono::seconds 
            stdex_secs_since_time_t_epoch = duration_long_long(secs_since_time_t_epoch), // with GMT shift
            stdex_d_shift_secs_delta = days(d_shift); // without GMT shift
        
        stdex::chrono::seconds
            stdex_secs_delta = stdex_secs_since_time_t_epoch - stdex_d_shift_secs_delta; // sys_clock and time_t epoches delta with GMT shift

        stdex::chrono::system_clock::time_point stdex_sys_clock_time_point = stdex_sys_clock_tp_epoch + stdex_secs_delta;

        stdex_sys_clock_time_point -= stdex::chrono::hours(24);
        time_t_time_point = invalid_time_t;
        for(std::size_t i = 0; i < 24 * 2; ++i)
        {
            stdex_tm_time_point = make_utc_tm(stdex_sys_clock_time_point);
            stdex::tm tmp = stdex_tm_time_point;
            time_t_time_point = stdex::mktime(&tmp);

            if(invalid_time_t == time_t_time_point)
            {
                tmp = make_utc_tm(stdex_sys_clock_time_point + stdex::chrono::seconds(1));
                time_t_time_point = stdex::mktime(&tmp);
                if(invalid_time_t != time_t_time_point)
                {
                    tmp = stdex_tm_time_point;
                    time_t_time_point = time_t_epoch;
                }
            }

            if(time_t_epoch == time_t_time_point)
            {
                stdex_tm_time_point = tmp;
                break;
            }

            stdex_sys_clock_time_point += stdex::chrono::hours(1);
        }

        if(time_t_epoch != time_t_time_point)
            stdex_tm_time_point.tm_year = -987654321;
    }
    else
    {
        stdex_tm_time_point.tm_year = -987654321;
    }

    return stdex_tm_time_point;
}

static
stdex::chrono::system_clock::time_point tm_to_time_point(const stdex::tm &value)
{
    using stdex::chrono::seconds;

    days d1 = 
        days_from_civil(value.tm_year, value.tm_mon + 1, 1) - days_from_civil(70, 1, 1);
    seconds result =
        d1 +
        days(value.tm_mday - 1) +
        seconds(value.tm_hour * 3600 + value.tm_min * 60 + value.tm_sec);
    
    return stdex::chrono::system_clock::time_point(result);
}

static 
int get_GM_offset()
{
    static stdex::tm time_t_epoch_with_UTC = get_time_t_epoch_with_UTC();
    
    if(-987654321 == time_t_epoch_with_UTC.tm_year)
    {
        time_t_epoch_with_UTC = get_time_t_epoch_with_UTC();
        if(-987654321 == time_t_epoch_with_UTC.tm_year)
            return 0;
    }

    return 
        time_t_epoch_with_UTC.tm_hour > 12 ? 
            (time_t_epoch_with_UTC.tm_hour - 24) : time_t_epoch_with_UTC.tm_hour; // assuming that time_t epoch starts from 00:00:00
}

stdex::time_t
    stdex::chrono::system_clock::to_time_t(const time_point &_t) _STDEX_NOEXCEPT_FUNCTION
{
    const stdex::time_t invalid_tt(-1);

    static stdex::tm time_t_epoch_with_UTC = get_time_t_epoch_with_UTC();
    
    if(-987654321 == time_t_epoch_with_UTC.tm_year)
    {
        time_t_epoch_with_UTC = get_time_t_epoch_with_UTC();
        if(-987654321 == time_t_epoch_with_UTC.tm_year)
            return invalid_tt;
    }

    static int GM_offset = get_GM_offset();

    time_point _t_with_GM_offset = _t + hours(GM_offset);

    stdex::tm 
        result_tm = make_utc_tm(_t_with_GM_offset);
    stdex::time_t 
        result_tt = stdex::mktime(&result_tm);
    
    if(invalid_tt == result_tt)
    {
        result_tm = make_utc_tm(_t_with_GM_offset + stdex::chrono::seconds(1));
        stdex::time_t tmp =
          stdex::mktime(&result_tm),
          time_t_epoch(0);

        if(invalid_tt != tmp && duration_long_long(stdex::difftime(tmp, time_t_epoch)) == 1)
            result_tt = time_t_epoch;
    }
    return result_tt;
}


stdex::chrono::system_clock::time_point
    stdex::chrono::system_clock::from_time_t(stdex::time_t _t) _STDEX_NOEXCEPT_FUNCTION
{
    const stdex::time_t invalid_tt(-1);

    if(invalid_tt == _t)
        return time_point();

    static stdex::tm time_t_epoch_with_UTC = get_time_t_epoch_with_UTC();
    

    if(-987654321 == time_t_epoch_with_UTC.tm_year)
    {
        time_t_epoch_with_UTC = get_time_t_epoch_with_UTC();
        if(-987654321 == time_t_epoch_with_UTC.tm_year)
            return time_point();
    }

    const stdex::time_t tt_epoch(0);

    double _t_secs = 
        stdex::difftime(_t, tt_epoch);
    seconds _t_chrono = duration_long_long(_t_secs);

    time_point time_t_epoch_with_UTC_chrono = 
        tm_to_time_point(time_t_epoch_with_UTC);
    
    static int GM_offset = get_GM_offset();

    duration time_from_time_t_epoch = 
        duration_cast<duration>(_t_chrono) - time_t_epoch_with_UTC_chrono.time_since_epoch() + hours(GM_offset);
    
    return time_point() + time_from_time_t_epoch;
}

namespace stdex 
{
    namespace chrono
    {
        template<class _Dur>
        static
        void _add_duration(
            typename conditional<
                detail::_is_duration<_Dur>::value,
                long,
                void*
            >::type delta, _Dur & dur)
        {
            dur += delta;
        }

        template<class _Dur>
        static
        void _add_duration(
            typename conditional<
                detail::_is_duration<_Dur>::value,
                void*,
                long
            >::type, _Dur &)
        {
        }

        inline
        static void _convert_to_duration(long ts_nsec, nanoseconds &out)
        {
            _add_duration(ts_nsec, out);
        }

        inline
        static void _convert_to_duration(long ts_nsec, microseconds &out)
        {
            _add_duration(ts_nsec/1000, out);
        }

        inline
        static void _convert_to_duration(long ts_nsec, milliseconds &out)
        {
            ts_nsec /= 1000;
            _add_duration(ts_nsec/1000, out);
        }

        inline
        static void _convert_to_duration(long ts_nsec, seconds &out)
        {
            ts_nsec /= 1000;
            ts_nsec /= 1000;
            _add_duration(ts_nsec/1000, out);
        }
    }
}

stdex::chrono::system_clock::time_point stdex::chrono::system_clock::now() _STDEX_NOEXCEPT_FUNCTION
{    // get current time
    {
        stdex::timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 0;

        for (std::size_t i = 0; i < 10; ++i)
        {
            if ((*clock_gettime_func_pointer)(_STDEX_CHRONO_CLOCK_REALTIME, &ts) == 0)
                break;
        }
        
        duration tv_nsec = 0;

        _convert_to_duration(ts.tv_nsec, tv_nsec);

        return
        time_point( 
            seconds(ts.tv_sec) + tv_nsec);
    }
}

stdex::chrono::steady_clock::time_point stdex::chrono::steady_clock::now() _STDEX_NOEXCEPT_FUNCTION
{    // get current time
    {
        stdex::timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 0;

        for (std::size_t i = 0; i < 10; ++i)
        {
            if ((*clock_gettime_func_pointer)(_STDEX_CHRONO_CLOCK_MONOTONIC, &ts) == 0)
                break;
        }

        duration tv_nsec = 0;

        _convert_to_duration(ts.tv_nsec, tv_nsec);

        return
        time_point( 
            seconds(ts.tv_sec) + tv_nsec);
    }
}
