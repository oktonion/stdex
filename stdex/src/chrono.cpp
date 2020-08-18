// stdex includes
#include "../include/chrono.hpp"

// POSIX includes
//#include <time.h> // for clock_gettime
#include <pthread.h>

// std includes
#include <exception>
#include <cstdlib>
#include <numeric>
#include <cstring>


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

struct mytimespec
{
    stdex::time_t tv_sec;
    long tv_nsec;
};

typedef LONGLONG duration_long_long;

namespace clock_gettime_impl
{
    static LARGE_INTEGER get_start_point()
    {
        if (sizeof(stdex::intmax_t) < 64)
        {
            struct lamdas
            {
                static LARGE_INTEGER get_current_date()
                {
                    LARGE_INTEGER current_date;
                    FILETIME ft;

                    GetSystemTimeAsFileTime(&ft);
                    current_date.LowPart = ft.dwLowDateTime;
                    current_date.HighPart = ft.dwHighDateTime;

                    return current_date;
                }
            };

            static LARGE_INTEGER current_date = lamdas::get_current_date();

            return current_date;
        }
        else
        {
            LARGE_INTEGER jan_1_1970;

            // 116444736000000000 in ULARGE_INTEGER
            jan_1_1970.LowPart = 0xD53E8000;
            jan_1_1970.HighPart = 0x19DB1DE;

            return jan_1_1970;
        }
    }

#define _STDEX_CHRONO_CLOCK_REALTIME 0
#define _STDEX_CHRONO_CLOCK_MONOTONIC 1

#if defined(LLONG_MAX) && defined(OLOLO)
#define exp7           10000000i64     //1E+7     //C-file part
#define exp9         1000000000i64     //1E+9
#define w2ux 116444736000000000i64     //1.jan1601 to 1.jan1970
    void unix_time(struct mytimespec *spec)
    {
        LARGE_INTEGER wintime;
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        wintime.LowPart = ft.dwLowDateTime;
        wintime.HighPart = ft.dwHighDateTime;

        wintime.QuadPart -= w2ux;  spec->tv_sec = wintime.QuadPart / exp7;
        spec->tv_nsec = wintime.QuadPart % exp7 * 100;
    }

    int clock_gettime_steady(int, mytimespec *spec)
    {
        static  struct mytimespec startspec; 
        static double ticks2nano = 100.;
        static LARGE_INTEGER startticks, tps;
        LARGE_INTEGER tmp, curticks;
        tps.QuadPart = 0;

        QueryPerformanceFrequency(&tmp); //some strange system can
        if (tps.QuadPart != tmp.QuadPart) {
            tps.QuadPart = tmp.QuadPart; //init ~~ONCE         //possibly change freq ?
            QueryPerformanceCounter(&startticks);
            unix_time(&startspec); ticks2nano = (double) exp9 / tps.QuadPart;
        }
        QueryPerformanceCounter((LARGE_INTEGER*) &curticks); curticks.QuadPart -= startticks.QuadPart;
        spec->tv_sec = startspec.tv_sec + (curticks.QuadPart / tps.QuadPart);
        spec->tv_nsec = startspec.tv_nsec + (long)((double) (curticks.QuadPart % tps.QuadPart) * ticks2nano);
        if (!(spec->tv_nsec < exp9)) { spec->tv_sec++; spec->tv_nsec -= exp9; }
        return 0;
    }

    int
        clock_gettime(int X, mytimespec *tv)
    {
        LARGE_INTEGER           t;
        FILETIME            f;
        double                  microseconds;
        static LARGE_INTEGER    offset;
        static double           frequencyToMicroseconds;
        static int              initialized = 0;
        static BOOL             usePerformanceCounter = 0;

        if (!initialized) {
            LARGE_INTEGER performanceFrequency;
            initialized = 1;
            usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
            if (!usePerformanceCounter) {
                offset = getFILETIMEoffset();
                frequencyToMicroseconds = 10.;
            }
        }
        if (usePerformanceCounter) 
            return clock_gettime_steady(X, tv);
        else {
            GetSystemTimeAsFileTime(&f);
            t.QuadPart = f.dwHighDateTime;
            t.QuadPart <<= 32;
            t.QuadPart |= f.dwLowDateTime;
        }

        t.QuadPart -= offset.QuadPart;
        microseconds = (double) t.QuadPart / frequencyToMicroseconds;
        t.QuadPart = LONGLONG(microseconds + 0.5);
        tv->tv_sec = t.QuadPart / 1000000;
        tv->tv_nsec = (t.QuadPart % 1000000) * 1000;
        return (0);
    }


#define _STDEX_CHRONO_USE_MICROSECONDS
#else

    int clock_gettime_realtime(mytimespec &ts)
    {
        FILETIME    filetime;
        LARGE_INTEGER today;
        const LARGE_INTEGER start_point = get_start_point();

        GetSystemTimeAsFileTime(&filetime);

        today.LowPart = filetime.dwLowDateTime;
        today.HighPart = filetime.dwHighDateTime;

        if (today.QuadPart < start_point.QuadPart)
            std::terminate();

        today.QuadPart -= start_point.QuadPart;

        LARGE_INTEGER secs_in_filetime;
        secs_in_filetime.QuadPart = 10000000;

        LARGE_INTEGER delta_sec, delta_nsec;

        delta_sec.QuadPart = today.QuadPart/ secs_in_filetime.QuadPart;
        delta_nsec.QuadPart = (today.QuadPart % secs_in_filetime.QuadPart) * 100;

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
                static_cast<long>(delta_nsec.QuadPart) + 1;
        }
        else
        {
            ts.tv_sec = _ts_sec_max;
            ts.tv_nsec = 999999999;
        }

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

    static const LARGE_INTEGER &start_point() {
        static const LARGE_INTEGER &cached_freq = cache_freq();

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

    static const LARGE_INTEGER &_dumb_init = start_point();

    int clock_gettime_monotonic(mytimespec &ts)
    {
        const LARGE_INTEGER &sp = start_point();

        if (sp.QuadPart == 0)
            return clock_gettime_realtime(ts);

        LARGE_INTEGER end_point;
        if (0 == QueryPerformanceCounter(&end_point) || end_point.QuadPart < sp.QuadPart)
            return clock_gettime_realtime(ts);

        const LARGE_INTEGER &cached_freq_sec = cache_freq();
        LARGE_INTEGER sec_to_ns_ratio;
        sec_to_ns_ratio.QuadPart = 1000 * 1000 * 1000;
        LARGE_INTEGER delta;
        delta.QuadPart = (end_point.QuadPart - sp.QuadPart);

        LARGE_INTEGER delta_sec, delta_nsec;

        delta_sec.QuadPart = delta.QuadPart / cached_freq_sec.QuadPart;
        delta_nsec.QuadPart = (delta.QuadPart % cached_freq_sec.QuadPart) * sec_to_ns_ratio.QuadPart / cached_freq_sec.QuadPart;

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
                static_cast<long>(delta_nsec.QuadPart) + 1;
        }
        else
        {
            ts.tv_sec = _ts_sec_max;
            ts.tv_nsec = 999999999;
        }

        return 0;
    }

    int clock_gettime(int clk_id, mytimespec* ts) {
        if (clk_id == _STDEX_CHRONO_CLOCK_MONOTONIC)
            return clock_gettime_monotonic(*ts);
        else if (clk_id == _STDEX_CHRONO_CLOCK_REALTIME)
            return clock_gettime_monotonic(*ts);
        return -1;

    }

#define _STDEX_CHRONO_USE_MICROSECONDS
#endif
}

int(*clock_gettime_func_pointer)(int, mytimespec*) = &clock_gettime_impl::clock_gettime;
#elif defined(__MACH__) && !defined(CLOCK_REALTIME)
#include <time.h>
#include <sys/time.h>       /* gettimeofday */
#include <mach/mach_time.h> /* mach_absolute_time */

typedef ::int64_t duration_long_long;

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
        abort();                            /* very unlikely error */

    if (gettimeofday(&micro, NULL) != 0)
        abort();                            /* very unlikely error */

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
struct mytimespec:
    public timespec
{};

int clock_gettime(int X, timespec *tv)
{
    *tv = get_abs_future_time_fine(0);

    return (0);
}
int(*clock_gettime_func_pointer)(int, timespec*) = &clock_gettime;
#else

typedef ::int64_t duration_long_long;

struct mytimespec:
    public timespec
{};

int clock_gettime(clockid_t, struct timespec*);
int(*clock_gettime_func_pointer)(clockid_t, struct timespec*) = &clock_gettime;
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
#ifdef LLONG_MAX
    LARGE_INTEGER performanceFrequency;

    const bool stdex::chrono::system_clock::is_steady = QueryPerformanceFrequency(&performanceFrequency) != 0;
    const bool stdex::chrono::steady_clock::is_steady = QueryPerformanceFrequency(&performanceFrequency) != 0;
#else
    const bool stdex::chrono::system_clock::is_steady = false;
    const bool stdex::chrono::steady_clock::is_steady = false;
#endif

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

            _big_int::_big_int(const stdex::intmax_t& _value)
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

            _big_int::operator stdex::intmax_t() const
            {
                duration_long_long result = convert(*this);

                if (result > (std::numeric_limits<stdex::intmax_t>::max)())
                    throw(std::out_of_range("stdex::intmax_t overflow"));

                return stdex::intmax_t(result);
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

            _big_int& _big_int::operator*=(const stdex::intmax_t& _r_in)
            {
                *this = convert(convert(*this) * duration_long_long(_r_in));
                return *this;
            }

            _big_int& _big_int::operator/=(const stdex::intmax_t& _r_in)
            {
                *this = convert(convert(*this) / duration_long_long(_r_in));
                return *this;
            }

            _big_int& _big_int::operator%=(const stdex::intmax_t& _r_in)
            {
                *this = convert(convert(*this) % duration_long_long(_r_in));
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
        
        }
    }
}

stdex::chrono::system_clock::time_point stdex::chrono::system_clock::now() _STDEX_NOEXCEPT_FUNCTION
{    // get current time
    {
        mytimespec ts;
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
        mytimespec ts;
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
