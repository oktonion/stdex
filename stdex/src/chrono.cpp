// stdex includes
#include "../include/chrono.hpp"

// POSIX includes
//#include <time.h> // for clock_gettime
#include <pthread.h>

// std includes
#include <exception>
#include <cstdlib>


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

namespace clock_gettime_impl
{
	LARGE_INTEGER
		getFILETIMEoffset()
	{
		SYSTEMTIME s;
		FILETIME f;
		LARGE_INTEGER t;

		s.wYear = 1970;
		s.wMonth = 1;
		s.wDay = 1;
		s.wHour = 0;
		s.wMinute = 0;
		s.wSecond = 0;
		s.wMilliseconds = 0;
		if (SystemTimeToFileTime(&s, &f) == 0)
			std::abort(); // never ever happens
		t.QuadPart = f.dwHighDateTime;
		t.QuadPart <<= 32;
		t.QuadPart |= f.dwLowDateTime;
		return (t);
	}

#ifdef LLONG_MAX
#define exp7           10000000i64     //1E+7     //C-file part
#define exp9         1000000000i64     //1E+9
#define w2ux 116444736000000000i64     //1.jan1601 to 1.jan1970
	void unix_time(struct mytimespec *spec)
	{
		__int64 wintime; GetSystemTimeAsFileTime((FILETIME*) &wintime);
		wintime -= w2ux;  spec->tv_sec = wintime / exp7;
		spec->tv_nsec = wintime % exp7 * 100;
	}

	int clock_gettime_steady(int, mytimespec *spec)
	{
		static  struct mytimespec startspec; static double ticks2nano;
		static __int64 startticks, tps = 0;    __int64 tmp, curticks;
		QueryPerformanceFrequency((LARGE_INTEGER*) &tmp); //some strange system can
		if (tps != tmp) {
			tps = tmp; //init ~~ONCE         //possibly change freq ?
			QueryPerformanceCounter((LARGE_INTEGER*) &startticks);
			unix_time(&startspec); ticks2nano = (double) exp9 / tps;
		}
		QueryPerformanceCounter((LARGE_INTEGER*) &curticks); curticks -= startticks;
		spec->tv_sec = startspec.tv_sec + (curticks / tps);
		spec->tv_nsec = startspec.tv_nsec + (long)((double) (curticks % tps) * ticks2nano);
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
			if (usePerformanceCounter) {
				QueryPerformanceCounter(&offset);
				frequencyToMicroseconds = (double) performanceFrequency.QuadPart / 1000000.;
			}
			else {
				offset = getFILETIMEoffset();
				frequencyToMicroseconds = 10.;
			}
		}
		if (usePerformanceCounter) QueryPerformanceCounter(&t);
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
#endif
}

#define _STDEX_CHRONO_CLOCK_REALTIME 0
#define _STDEX_CHRONO_CLOCK_MONOTONIC 0
int(*clock_gettime_func_pointer)(int, mytimespec*) = &clock_gettime_impl::clock_gettime;
#elif defined(__MACH__) && !defined(CLOCK_REALTIME)
#include <time.h>
#include <sys/time.h>       /* gettimeofday */
#include <mach/mach_time.h> /* mach_absolute_time */

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

stdex::chrono::system_clock::time_point stdex::chrono::system_clock::now() _STDEX_NOEXCEPT_FUNCTION
{	// get current time
	{
		mytimespec ts = {0, 0};

		if ((*clock_gettime_func_pointer)(_STDEX_CHRONO_CLOCK_REALTIME, &ts) != 0)
		{
			std::terminate();
		}

#ifdef _STDEX_CHRONO_USE_MICROSECONDS
		return time_point(
			seconds(ts.tv_sec) + microseconds(ts.tv_nsec / 1000));
#else
		return time_point(
			seconds(ts.tv_sec) + nanoseconds(ts.tv_nsec));
#endif
	}
}

stdex::chrono::steady_clock::time_point stdex::chrono::steady_clock::now() _STDEX_NOEXCEPT_FUNCTION
{	// get current time
	{
		mytimespec ts = {0, 0};

		if ((*clock_gettime_func_pointer)(_STDEX_CHRONO_CLOCK_MONOTONIC, &ts) != 0)
		{
			std::terminate();
		}

#ifdef _STDEX_CHRONO_USE_MICROSECONDS
		return time_point(
			seconds(ts.tv_sec) + microseconds(ts.tv_nsec / 1000));
#else
		return time_point(
			seconds(ts.tv_sec) + nanoseconds(ts.tv_nsec));
#endif
	}
}
