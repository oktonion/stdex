// stdex includes
#include "../include/thread"

// POSIX includes
/*none*/

// std includes
#include <map>

// since windows is a 'special' platform there is some macro definitions to define platform
#if defined(WIN32) || defined(_WIN32)
#define _STDEX_THREAD_WIN
#endif

#ifdef _STDEX_THREAD_WIN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define __UNDEF_LEAN_AND_MEAN
#endif
#include <windows.h>
#ifdef __UNDEF_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef __UNDEF_LEAN_AND_MEAN
#endif

#include <winnt.h>

#endif

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define NOEXCEPT_FUNCTION noexcept

#else

#define NOEXCEPT_FUNCTION throw()

#endif

enum eThreadIDOperation
{
	RemoveThreadID,
	GetThreadID
};

static void _pthread_t_ID(stdex::thread::id aHandle, const eThreadIDOperation operation, stdex::uintmax_t *id_out = NULL)
{
	typedef std::map<stdex::thread::id, stdex::uintmax_t> id_map_type;

	static stdex::mutex idMapLock;
	static id_map_type idMap;
	static stdex::uintmax_t idCount = 1;

	stdex::lock_guard<stdex::mutex> guard(idMapLock);

	if (idMap.size() == 0)
		idCount = 1;
	else if (idMap.size() == 1)
		idCount = idMap.begin()->second + 1;


	if (operation == GetThreadID)
	{
		id_map_type::iterator it = idMap.find(aHandle);

		if (it == idMap.end())
		{
			std::pair<id_map_type::iterator, bool> result =
				idMap.insert(std::make_pair(aHandle, idCount++));
			it = result.first;
		}

		if (id_out != NULL)
		{
			*id_out = it->second;
		}
	}
	else
	{
		idMap.erase(aHandle);
	}
}

using namespace stdex;

stdex::uintmax_t thread::id::uid() const
{
	if (!_is_valid)
		return stdex::uintmax_t(0);

	stdex::uintmax_t _uid;

	_pthread_t_ID(*this, GetThreadID, &_uid);

	return _uid;

}

/// Information to pass to the new thread (what to run).
struct thread_start_info {
	void(*exec_function)(void *); ///< Pointer to the function to be executed.
	void *argument;               ///< Function argument for the thread function.
};

// Thread wrapper function.
void* thread::wrapper_function(void *aArg)
{
	// Get thread startup information
	thread_start_info *ti = (thread_start_info *) aArg;

	try
	{
		// Call the actual client thread function
		(*ti->exec_function)(ti->argument);
	}
	catch (...)
	{
		// Uncaught exceptions will terminate the application (default behavior
		// according to C++11)
		std::terminate();
	}

	// The thread is no longer executing
	
	// The thread is responsible for freeing the startup information
	delete ti;

	_pthread_t_ID(this_thread::get_id(), RemoveThreadID);

	return 0;
}

void thread::init(void(*aFunction)(void *), void *aArg)
{
	// Fill out the thread startup information (passed to the thread wrapper,
	// which will eventually free it)
	thread_start_info *thread_info = new thread_start_info;
	thread_info->exec_function = aFunction;
	thread_info->argument = aArg;

	_id._is_valid = true;

	// Create the thread
	int _e = pthread_create(&_id._handle, NULL, &wrapper_function, (void *) thread_info);
	if (_e)
	{// Did we fail to create the thread?
		_id._is_valid = false;
		delete thread_info;

		throw system_error(errc(_e));
	}
}

thread::~thread()
{
	if (joinable())
		std::terminate();
}

void thread::join()
{
	int _e = invalid_argument;

	if (joinable())
	{
		_e = pthread_join(_id._handle, NULL);
	}

	if (_e)
		throw system_error(errc(_e));

	_id = id();
}

bool thread::joinable() const NOEXCEPT_FUNCTION
{
	return _id != id();
}

void thread::detach()
{
	int _e = invalid_argument;


	if (joinable())
	{
		_e = pthread_detach(_id._handle);
	}

	if (_e)
		throw system_error(errc(_e));

	_id = id();
}

thread::id thread::get_id() const NOEXCEPT_FUNCTION
{
	if (!joinable())
		return id();

	return _id;
}

unsigned thread::hardware_concurrency() NOEXCEPT_FUNCTION
{
#if defined(_STDEX_THREAD_WIN)
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return (int) si.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
	return (int) sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(_SC_NPROC_ONLN)
	return (int) sysconf(_SC_NPROC_ONLN);
#else
	// The standard requires this function to return zero if the number of
	// hardware cores could not be determined.
	return 0;
#endif
}

void thread::swap(thread & other) NOEXCEPT_FUNCTION
{
	if (&other == this)
		return;

	using std::swap;

	swap(_id, other._id);
}

thread::id this_thread::get_id() NOEXCEPT_FUNCTION
{
	return thread::id(pthread_self());
}

#ifdef _STDEX_THREAD_WIN // windows platform

#ifndef NT_ERROR
#define NT_ERROR(Status) ((((ULONG)(Status)) >> 30) == 3)
#endif

typedef long NTSTATUS;

#ifndef _In_
#define _In_
#endif
#ifndef _Out_
#define _Out_
#endif

extern "C"
{

	NTSYSAPI
		NTSTATUS
		NTAPI
		NtSetTimerResolution(
			_In_ ULONG                DesiredResolution,
			_In_ BOOLEAN              SetResolution,
			_Out_ PULONG              CurrentResolution);

	NTSYSAPI
		NTSTATUS
		NTAPI
		NtQueryTimerResolution(
			_Out_ PULONG              MaximumResolution,
			_Out_ PULONG              MinimumResolution,
			_Out_ PULONG              CurrentResolution);

}
//#pragma comment (lib, "ntdll.lib") // uncomment to add lib explicitly

ULONG AdjustSystemTimerResolutionTo500mcs()
{
	static ULONG resolution = 5000; // 0.5 мс в 100-наносекундных интервалах.

	ULONG sysTimerOrigResolution = 10000;

	ULONG minRes;
	ULONG maxRes;
	NTSTATUS ntRes = NtQueryTimerResolution(&maxRes, &minRes, &sysTimerOrigResolution);
	if (NT_ERROR(ntRes))
	{
		throw std::runtime_error("Failed query system timer resolution");
	}

	resolution = minRes < resolution ? minRes : resolution;

	ULONG curRes;
	ntRes = NtSetTimerResolution(resolution, TRUE, &curRes);
	if (NT_ERROR(ntRes))
	{
		// throw std::runtime_error("Failed set system timer resolution" ); // does not matter actually
	}

	return sysTimerOrigResolution;
}

void AdjustSystemTimerResolutionTo(const ULONG &sysTimerOrigResolution)
{
	ULONG curRes;
	NTSTATUS ntRes = NtSetTimerResolution(sysTimerOrigResolution, TRUE, &curRes);
	if (NT_ERROR(ntRes))
	{
		throw std::runtime_error("Failed set system timer resolution");
	}
}


class WaitableTimer
{
public:

	WaitableTimer()
	{
		m_timer = ::CreateWaitableTimer(NULL, FALSE, NULL);
		if (!m_timer)
			throw std::runtime_error("Failed to create waitable time (CreateWaitableTimer)");
	}

	~WaitableTimer()
	{
		::CloseHandle(m_timer);
		m_timer = NULL;
	}

	void Start(stdex::intmax_t relativeTime100Ns)
	{
		const ULONG sysTimerOrigResolution = AdjustSystemTimerResolutionTo500mcs();

		LARGE_INTEGER dueTime = { 0 };
		dueTime.QuadPart = static_cast<LONGLONG>(relativeTime100Ns) * -1;


		BOOL res = ::SetWaitableTimer(m_timer, &dueTime, 0, NULL, NULL, FALSE);

		if (!res)
		{
			AdjustSystemTimerResolutionTo(sysTimerOrigResolution);
			throw std::runtime_error("SetAndWait: failed set waitable time (SetWaitableTimer)");
		}

		::WaitForSingleObjectEx(m_timer, INFINITE, FALSE);

		AdjustSystemTimerResolutionTo(sysTimerOrigResolution);
	}

private:
	HANDLE m_timer;
};

void detail::sleep_for_impl(const struct timespec *reltime)
{
	WaitableTimer timer;

	stdex::intmax_t us100 = reltime->tv_sec * 1000 * 1000 * 10 + reltime->tv_nsec / 100;

	timer.Start(us100);
}

#else
void detail::sleep_for_impl(const struct timespec *reltime)
{
	nanosleep(reltime, NULL);
}

#endif


#undef NOEXCEPT_FUNCTION
