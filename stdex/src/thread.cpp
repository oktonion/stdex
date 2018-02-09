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

using namespace stdex;


bool operator<(const pthread_t &lhs, const pthread_t &rhs)
{
	bool are_equal = (pthread_equal(lhs, rhs) != 0);
	if (are_equal)
		return false;

	return std::less<const pthread_t*>()(&lhs, &rhs);
}

enum eThreadIDOperation
{
	RemoveThreadID,
	GetThreadID
};

static void _pthread_t_ID(const pthread_t &aHandle, const eThreadIDOperation operation, thread::id *id_out = NULL)
{
	static mutex idMapLock;
	static std::map<pthread_t, unsigned long int> idMap;
	static unsigned long int idCount(1);

	lock_guard<mutex> guard(idMapLock);

	if (operation == GetThreadID)
	{
		if (idMap.find(aHandle) == idMap.end())
			idMap[aHandle] = idCount++;

		if(id_out != NULL)
			*id_out = thread::id(idMap[aHandle]);
	}
	else
	{
		idMap.erase(aHandle);
	}
}

/// Information to pass to the new thread (what to run).
struct thread::thread_start_info {
	void(*exec_function)(void *); ///< Pointer to the function to be executed.
	void *argument;               ///< Function argument for the thread function.
	thread *thread_object;          ///< Pointer to the thread object.
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
	
	_pthread_t_ID(ti->thread_object->_thread_handle, RemoveThreadID);

	lock_guard<mutex> guard(ti->thread_object->_data_mutex);
	ti->thread_object->_not_a_thread = true;
	

	// The thread is responsible for freeing the startup information
	delete ti;

	return 0;
}

void thread::init(void(*aFunction)(void *), void *aArg)
{
	// Serialize access to this thread structure
	lock_guard<mutex> guard(_data_mutex);

	// Fill out the thread startup information (passed to the thread wrapper,
	// which will eventually free it)
	_thread_info = new thread_start_info;
	_thread_info->exec_function = aFunction;
	_thread_info->argument = aArg;
	_thread_info->thread_object = this;

	// The thread is now alive
	_not_a_thread = false;	

	// Create the thread
	if (pthread_create(&_thread_handle, NULL, &wrapper_function, (void *) _thread_info) != 0)
	{// Did we fail to create the thread?
		_not_a_thread = true;
		delete _thread_info;
	}
}

thread::~thread()
{
	if (joinable())
		std::terminate();
}

void thread::join()
{
	if (get_id() != id())
	{
		pthread_join(_thread_handle, NULL);
	}
}

bool thread::joinable() const NOEXCEPT_FUNCTION
{
	_data_mutex.lock();
	bool result = !_not_a_thread;
	_data_mutex.unlock();

	return result;
}

void thread::detach()
{
	_data_mutex.lock();

	if (!_not_a_thread)
	{
		pthread_detach(_thread_handle);
		_not_a_thread = true;
	}

	_data_mutex.unlock();
}

thread::id thread::get_id() const NOEXCEPT_FUNCTION
{
	thread::id tmp;
	if (!joinable())
		return id();

	_pthread_t_ID(_thread_handle, GetThreadID, &tmp);

	return tmp;
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

	lock_guard<mutex> lk1(_data_mutex);
	lock_guard<mutex> lk2(other._data_mutex);

	swap(_not_a_thread, other._not_a_thread);
	swap(_thread_info, other._thread_info);

	if (!_not_a_thread)
		_thread_info->thread_object = this;

	if (!other._not_a_thread)
		other._thread_info->thread_object = &other;

	// swap handles of threads
	swap(_thread_handle, other._thread_handle);
}

thread::id this_thread::get_id() NOEXCEPT_FUNCTION
{
	thread::id tmp;

	_pthread_t_ID(pthread_self(), GetThreadID, &tmp);

	return tmp;
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

		DWORD waitRes = ::WaitForSingleObjectEx(m_timer, INFINITE, FALSE);

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
