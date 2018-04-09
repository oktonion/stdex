// stdex includes
#include "../include/chrono.hpp"

// POSIX includes
//#include <time.h> // for clock_gettime
#include <pthread.h>

// std includes
#include <exception>

using namespace stdex;
using namespace stdex::chrono;

#ifndef CLOCK_REALTIME // assuming we are on windows platform and have no realtime clock

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define __UNDEF_LEAN_AND_MEAN
#endif
#include <windows.h>
#ifdef __UNDEF_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef __UNDEF_LEAN_AND_MEAN
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
		SystemTimeToFileTime(&s, &f);
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
		t.QuadPart = (microseconds + 0.5);
		tv->tv_sec = t.QuadPart / 1000000;
		tv->tv_nsec = (t.QuadPart % 1000000) * 1000;
		return (0);
	}

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
		t.QuadPart = (microseconds + 0.5);
		tv->tv_sec = t.QuadPart / 1000000;
		tv->tv_nsec = (t.QuadPart % 1000000) * 1000;
		return (0);
	}
#endif
}

#ifdef LLONG_MAX
LARGE_INTEGER performanceFrequency;

const bool system_clock::is_steady = QueryPerformanceFrequency(&performanceFrequency);
const bool steady_clock::is_steady = QueryPerformanceFrequency(&performanceFrequency);
#else
const bool system_clock::is_steady = false;
const bool steady_clock::is_steady = false;
#endif

#define CLOCK_REALTIME 0
int(*clock_gettime_func_pointer)(int X, mytimespec *tv) = &clock_gettime_impl::clock_gettime;
#else
struct mytimespec:
	public timespec
{};

const bool system_clock::is_steady = true;
const bool steady_clock::is_steady = true;

int clock_gettime(int X, timespec *tv);
int(*clock_gettime_func_pointer)(int X, timespec *tv) = &clock_gettime;
#endif

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define NOEXCEPT_FUNCTION noexcept

#else

#define NOEXCEPT_FUNCTION throw()

#endif

system_clock::time_point system_clock::now() NOEXCEPT_FUNCTION
{	// get current time
	{
		mytimespec ts;

		if ((*clock_gettime_func_pointer)(CLOCK_REALTIME, &ts) != 0)
		{
			std::terminate();
		}

		return time_point(duration(
			seconds(ts.tv_sec) + nanoseconds(ts.tv_nsec)));
	}
}

steady_clock::time_point steady_clock::now() NOEXCEPT_FUNCTION
{	// get current time
	{
		mytimespec ts;

		if ((*clock_gettime_func_pointer)(CLOCK_REALTIME, &ts) != 0)
		{
			std::terminate();
		}

		return time_point(duration(
			seconds(ts.tv_sec) + nanoseconds(ts.tv_nsec)));
	}
}
