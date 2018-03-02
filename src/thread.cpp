// stdex includes
#include "../include/thread"
#include "../include/mutex"
#include "../include/condition_variable"

// POSIX includes
/*none*/

// std includes
#include <map>
#include <set>

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
	GetThreadID,
	AddThreadID
};

struct _pthread_t_less
{
	bool operator() (const stdex::thread::native_handle_type &lhs, const stdex::thread::native_handle_type &rhs) const
	{
		if (&lhs == &rhs)
			return false;

		if (pthread_equal(lhs, rhs) != 0)
			return false;

		return memcmp(&lhs, &rhs, sizeof(const stdex::thread::native_handle_type)) < 0; // not so efficient but whatever
	}
};

static void _pthread_t_ID(const eThreadIDOperation operation, stdex::uintmax_t *id_out = NULL)
{
	typedef std::map<stdex::thread::native_handle_type, stdex::uintmax_t, _pthread_t_less> id_map_type;

	static stdex::mutex idMapLock;
	static id_map_type idMap;
	static stdex::uintmax_t idCount = 1;

	stdex::lock_guard<stdex::mutex> guard(idMapLock);

	stdex::thread::native_handle_type aHandle = pthread_self();

	if (operation == GetThreadID)
	{
		id_map_type::iterator result =
			idMap.find(aHandle);

		id_map_type::mapped_type out;

		if (idMap.end() == result)
		{
			idMap.insert(std::make_pair(aHandle, idCount));

			out = idCount;

			idCount++;
		}
		else
		{
			out = result->second;
		}

		if (id_out != NULL)
		{
			*id_out = out;
		}
	}
	else if (operation == AddThreadID)
	{
		std::pair<id_map_type::iterator, bool> result =
			idMap.insert(std::make_pair(aHandle, idCount));

		id_map_type::mapped_type out = result.first->second;

		if (result.second) // new element
		{
			idCount++;
		}
		else
		{
			std::terminate();
		}

		if (id_out != NULL)
		{
			*id_out = out;
		}
	}
	else
	{
		id_map_type::iterator result =
			idMap.find(aHandle);

		if (idMap.end() == result)
		{
			std::terminate();
		}
		else
		{
			idMap.erase(result);
			if (idMap.size() == 0)
				idCount = 1;
			else if(idMap.size() == 1)
				idCount = idMap.rbegin()->second + 1;
		}
	}
}

using namespace stdex;

/// Information to pass to the new thread (what to run).
struct thread_start_info {
	void(*exec_function)(void *); ///< Pointer to the function to be executed.
	void *argument;               ///< Function argument for the thread function.

	stdex::mutex *mtx;
	stdex::condition_variable *cond;
	bool *notified;
	stdex::thread::id *id;
};

// Thread wrapper function.
void* thread::wrapper_function(void *aArg)
{
	// Get thread startup information
	thread_start_info *ti = (thread_start_info *) aArg;

	{
		stdex::unique_lock<stdex::mutex> lock((*ti->mtx));

		_pthread_t_ID(AddThreadID, &ti->id->_uid);

		(*ti->notified) = true;
		ti->cond->notify_one();
	}

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

	_pthread_t_ID(RemoveThreadID);

	delete ti;

	return 0;
}

void thread::init(void(*aFunction)(void *), void *aArg)
{
	// Fill out the thread startup information (passed to the thread wrapper,
	// which will eventually free it)
	thread_start_info *thread_info = new thread_start_info;
	thread_info->exec_function = aFunction;
	thread_info->argument = aArg;

	stdex::mutex mtx;
	stdex::condition_variable cond;
	bool notified = false;

	thread_info->mtx = &mtx;
	thread_info->cond = &cond;
	thread_info->notified = &notified;
	thread_info->id = &_id;

	stdex::unique_lock<stdex::mutex> lock(mtx);

	// Create the thread
	int _e = pthread_create(&_handle, NULL, &wrapper_function, (void *) thread_info);

	if (_e)
	{// Did we fail to create the thread?

		lock.unlock();

		delete thread_info;

		throw system_error(error_code(errc(_e)));
	}
	else
	{
		while(!notified)
			cond.wait(lock);
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
		_e = pthread_join(_handle, NULL);
	}

	if (_e)
		throw system_error(error_code(errc(_e)));

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
		_e = pthread_detach(_handle);
	}

	if (_e)
		throw system_error(error_code(errc(_e)));

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

	swap(_handle, other._handle);
	swap(_id, other._id);
}

thread::id this_thread::get_id() NOEXCEPT_FUNCTION
{
	stdex::uintmax_t uid;

	_pthread_t_ID(GetThreadID, &uid);

	return thread::id(uid);
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

		LARGE_INTEGER dueTime = { {0} };
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


