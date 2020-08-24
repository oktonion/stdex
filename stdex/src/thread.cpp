// stdex includes
#include "../include/thread.hpp"
#include "../include/mutex.hpp"
#include "../include/condition_variable.hpp"

// POSIX includes
/*none*/

// std includes
#include <map>
#include <set>
#include <vector>
#include <ctime>
#include <cerrno>
#include <time.h>

#ifdef __GLIBC__
#include <sys/sysinfo.h>
#elif defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#if defined(__QNXNTO__) || defined(__QNX__)
#include <sys/syspage.h>
#endif

// since windows is a 'special' platform there is some macro definitions to define platform
#if defined(WIN32) || defined(_WIN32)
#define _STDEX_THREAD_WIN
#endif

#ifdef _STDEX_THREAD_WIN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define _STDEX_UNDEF_LEAN_AND_MEAN
#endif
#include <windows.h>
#ifdef _STDEX_UNDEF_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef _STDEX_UNDEF_LEAN_AND_MEAN
#endif

#include <winnt.h>

#endif

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_NOEXCEPT_FUNCTION noexcept

#else

#define _STDEX_NOEXCEPT_FUNCTION throw()

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
        using namespace std;
        
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

    static stdex::mutex idLock;
    static id_map_type idCollection_shared;
    static stdex::uintmax_t idCount_shared = 1;

    stdex::thread::native_handle_type aHandle = pthread_self();

    stdex::unique_lock<stdex::mutex> guard(idLock); // shared section begin

    if (operation == GetThreadID)
    {
        const id_map_type idCollection = idCollection_shared;
        guard.unlock(); // shared section end

        id_map_type::const_iterator result =
            idCollection.find(aHandle);

        id_map_type::mapped_type out;

        if (idCollection.end() == result)
        {
            guard.lock(); // shared section begin

            idCollection_shared.insert(std::make_pair(aHandle, idCount_shared));

            out = idCount_shared;

            idCount_shared++;

            guard.unlock(); // shared section end
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
            idCollection_shared.insert(std::make_pair(aHandle, idCount_shared));

        id_map_type::mapped_type out = result.first->second;

        if (result.second) // new element
        {
            idCount_shared++;
        }
        else
        {
            std::terminate();
        }

        guard.unlock(); // shared section end

        if (id_out != NULL)
        {
            *id_out = out;
        }
    }
    else
    {
        id_map_type::iterator result =
            idCollection_shared.find(aHandle);

        if (idCollection_shared.end() == result)
        {
            std::terminate();
        }
        else
        {
            idCollection_shared.erase(result);
            if (idCollection_shared.size() == 0)
                idCount_shared = 1;
            else if(idCollection_shared.size() == 1)
                idCount_shared = idCollection_shared.rbegin()->second + 1;
        }
    }

    // shared section end
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



struct thread_notification_data {
    typedef std::vector<std::pair<condition_variable*, mutex*>
    > notify_list_t;

    notify_list_t notify;

    void notify_all_at_thread_exit(condition_variable* cv, mutex* m)
    {
        notify.push_back(std::make_pair(cv, m));
    }

    ~thread_notification_data()
    {
        for (notify_list_t::iterator i = notify.begin(), e = notify.end();
            i != e; ++i)
        {
            i->second->unlock();
            i->first->notify_all();
        }
    }

    enum eThreadDataOperation
    {
        RemoveThreadData,
        AddToThreadData,
        SetThreadData
    };

    static void _this_thread_notification_data(eThreadDataOperation operation, thread_notification_data *data = NULL, condition_variable *cond = NULL, unique_lock<mutex> *lk = NULL)
    {
        typedef std::map<thread::id, thread_notification_data*> data_map_type;
        static mutex dataMapLock;
        static data_map_type dataMap;

        unique_lock<mutex> lock(dataMapLock);

        if (operation == AddToThreadData)
        {
            thread_notification_data *result = dataMap[this_thread::get_id()];

            if(result)
            {
                lock.unlock();
                result->notify_all_at_thread_exit(cond, lk->release());
            }
        }
        else if (operation == SetThreadData)
        {
            dataMap[this_thread::get_id()] = data;
        }
        else if (operation == RemoveThreadData)
        {
            dataMap.erase(this_thread::get_id());
        }
    }

    static void add_to_this_thread_notification_data(condition_variable *cond, unique_lock<mutex> *lk)
    {
        _this_thread_notification_data(AddToThreadData, NULL, cond, lk);
    }

    static void set_this_thread_notification_data(thread_notification_data *data)
    {
        _this_thread_notification_data(SetThreadData, data);
    }

    static void remove_this_thread_notification_data()
    {
        _this_thread_notification_data(RemoveThreadData);
    }
};



// Thread wrapper function.
void* thread::wrapper_function(void *aArg)
{
    // Get thread startup information
    thread_start_info *ti = (thread_start_info *) aArg;
    thread_notification_data nd;

    {
        stdex::unique_lock<stdex::mutex> lock((*ti->mtx));

        _pthread_t_ID(AddThreadID, &ti->id->_uid);
        thread_notification_data::set_this_thread_notification_data(&nd);

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

    thread_notification_data::remove_this_thread_notification_data();

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

        throw system_error(
            stdex::make_error_code(stdex::errc::errc_t(_e))
        );
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
    int _e = errc::invalid_argument;

    if (joinable())
    {
        _e = pthread_join(_handle, NULL);
    }

    if (_e)
        throw system_error(
            stdex::make_error_code(stdex::errc::errc_t(_e))
        );

    _id = id();
}

bool thread::joinable() const _STDEX_NOEXCEPT_FUNCTION
{
    return _id != id();
}

void thread::detach()
{
    int _e = errc::invalid_argument;


    if (joinable())
    {
        _e = pthread_detach(_handle);
    }

    if (_e)
        throw system_error(
            stdex::make_error_code(stdex::errc::errc_t(_e))
        );

    _id = id();
}

thread::id thread::get_id() const _STDEX_NOEXCEPT_FUNCTION
{
    if (!joinable())
        return id();

    return _id;
}

unsigned thread::hardware_concurrency() _STDEX_NOEXCEPT_FUNCTION
{
#if defined(_STDEX_THREAD_WIN)
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwNumberOfProcessors > 0 ? si.dwNumberOfProcessors : 0;
#elif defined(_SC_NPROCESSORS_ONLN)
    int const count = sysconf(_SC_NPROCESSORS_ONLN);
    return (count > 0) ? count : 0;
#elif defined(_SC_NPROC_ONLN)
    int const count = sysconf(_SC_NPROC_ONLN);
    return (count > 0) ? count : 0;
#elif defined(PTW32_VERSION)
    return pthread_num_processors_np();
#elif defined(__GLIBC__)
    return get_nprocs();
#elif defined(SYSPAGE_CPU_ENTRY)
    return _syspage_ptr->num_cpu;
#elif defined(__APPLE__) || defined(__FreeBSD__)
    int count;
    size_t size=sizeof(count);
    return sysctlbyname("hw.ncpu", &count, &size, NULL, 0) ? 0 : count;
#else
    // The standard requires this function to return zero if the number of
    // hardware cores could not be determined.
    return 0;
#endif
}

void thread::swap(thread & other) _STDEX_NOEXCEPT_FUNCTION
{
    if (&other == this)
        return;
    {
       native_handle_type tmp_value;
       tmp_value = _handle;
       _handle = other._handle;
       other._handle = tmp_value;
    }

    {
       id tmp_value;
       tmp_value = _id;
       _id = other._id;
       other._id = tmp_value;
    }
}

thread::id this_thread::get_id() _STDEX_NOEXCEPT_FUNCTION
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
    static ULONG resolution = 5000; // 0.5 �� � 100-������������� ����������.

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

    void Start(LONGLONG relativeTime100Ns)
    {
        const ULONG sysTimerOrigResolution = AdjustSystemTimerResolutionTo500mcs();

        LARGE_INTEGER dueTime = { {0} };
        dueTime.QuadPart = relativeTime100Ns * -1;

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

    LONGLONG us100 = reltime->tv_sec * 1000 * 1000 * 10 + reltime->tv_nsec / 100;

    if (us100 < 0)
        return;
    if (us100 == 0)
    {
        stdex::this_thread::yield();
        return;
    }
    
    LARGE_INTEGER nStartTime;
    LARGE_INTEGER nStopTime;
    LARGE_INTEGER nElapsed;
    LARGE_INTEGER nFrequency;

    nElapsed.QuadPart = 0;
    nFrequency.QuadPart = 1;

    bool check_timings = 
        (0 != ::QueryPerformanceFrequency(&nFrequency));

    if (!check_timings)
        us100 += 1000 * 10;


    do {
        us100 -= (nElapsed.QuadPart * 10);
        
        check_timings =
            (0 != ::QueryPerformanceCounter(&nStartTime));

        timer.Start(us100);

        check_timings =
            (0 != ::QueryPerformanceCounter(&nStopTime));

        nElapsed.QuadPart = (nStopTime.QuadPart - nStartTime.QuadPart) * 1000000;
        nElapsed.QuadPart /= nFrequency.QuadPart;
    } while (check_timings && (nElapsed.QuadPart * 10) < us100);
}

#else

static char clock_gettime(...); // dummy
static char clock_nanosleep(...); // dummy

namespace thread_cpp_detail
{
    template<bool>
    struct nanosleep_impl1
    {
        static int call(const timespec *req, timespec *rem)
        {
            errno = 0;
            int err = ::nanosleep(req, rem);

            if(err && (rem->tv_sec || rem->tv_nsec))
            {
                errno = 0;
                err = ::nanosleep(rem, rem);
            }
            return err;
        }
    };

#ifdef CLOCK_MONOTONIC
    #undef _STDEX_THREAD_CLOCK_MONOTONIC
    #define _STDEX_THREAD_CLOCK_MONOTONIC CLOCK_MONOTONIC
    
    #ifndef _STDEX_THREAD_CLOCK_MONOTONIC_EXISTS
        #define _STDEX_THREAD_CLOCK_MONOTONIC_EXISTS
    #endif

#endif

#ifdef CLOCK_BOOTTIME 
    #undef _STDEX_THREAD_CLOCK_MONOTONIC
    #define _STDEX_THREAD_CLOCK_MONOTONIC CLOCK_BOOTTIME

    #ifndef _STDEX_THREAD_CLOCK_MONOTONIC_EXISTS
        #define _STDEX_THREAD_CLOCK_MONOTONIC_EXISTS
    #endif
#endif

#if defined(CLOCK_MONOTONIC) && \
        defined(_STDEX_THREAD_CLOCK_MONOTONIC_EXISTS) && \
            defined(TIMER_ABSTIME)
    template<>
    struct nanosleep_impl1<true>
    {
        enum {BILLION = 1000000000};
        static void timespec_add(timespec &result, const timespec &in)
        {
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

        static inline void timespec_diff(const timespec &a, const timespec &b,
            timespec &result) 
        {
            result.tv_sec  = a.tv_sec  - b.tv_sec;
            result.tv_nsec = a.tv_nsec - b.tv_nsec;
            if (result.tv_nsec < 0) {
                --result.tv_sec;
                result.tv_nsec += BILLION;
            }
        }

        static int clock_nanosleep_abs(const timespec* tp)
        {
            errno = 0;
            int err =
                ::clock_nanosleep(_STDEX_THREAD_CLOCK_MONOTONIC, TIMER_ABSTIME, tp, 0);

            if (0 == err || ENOTSUP != errno)
                return err;

            errno = 0;
            err =
                ::clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, tp, 0);

            if (0 == err || ENOTSUP != errno)
                return err;

            errno = 0;
            err = 
                ::clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, tp, 0);

            return err;
        }

        static int call(const timespec *req, timespec *rem)
        {
            timespec _begin, tp;

            int err = 
                ::clock_gettime(_STDEX_THREAD_CLOCK_MONOTONIC, &_begin);
            if(err != 0)
                return nanosleep_impl1<false>::call(req, rem);

            tp = _begin;
            timespec_add(tp, *req);
            rem->tv_sec = 0;
            rem->tv_nsec = 0;

            int nanosleep_err = 0;
            

            errno = 0;
            nanosleep_err = 
                clock_nanosleep_abs(&tp);

            if (0 != nanosleep_err && EINTR != errno)
            {
                errno = 0;
                nanosleep_err =
                    nanosleep_impl1<false>::call(req, rem);
                if (0 != nanosleep_err)
                    return nanosleep_err;
            }

            timespec _end, _passed;

            if (0 == nanosleep_err || EINTR == errno)
            {
                int myerrno = errno;
                err = 
                    ::clock_gettime(_STDEX_THREAD_CLOCK_MONOTONIC, &_end);
                errno = 0;
                if (0 != err)
                {
                    errno = myerrno;
                    return nanosleep_err;
                }
            }

            timespec_diff(_end, _begin, _passed);

            if (_passed.tv_sec > req->tv_sec ||
                (_passed.tv_sec == req->tv_sec && _passed.tv_nsec > req->tv_nsec))
                return 0;

            timespec_diff(*req, _passed, *rem);

            if (rem->tv_sec < 0) rem->tv_sec = 0;
            if (rem->tv_nsec < 0) rem->tv_nsec = 0;

            return nanosleep_err;
        }
    };
#endif
} // namespace thread_cpp_detail

namespace thread_cpp_detail
{
    template<class _Tp>
    static _Tp declval(); 

    struct nanosleep_impl:
        nanosleep_impl1<
            sizeof(::clock_gettime(declval<clockid_t&>(), declval<timespec*>())) != sizeof(char) &&
            sizeof(::clock_nanosleep(declval<clockid_t&>(), declval<int>(), declval<timespec*>(), declval<timespec*>())) != sizeof(char)
        >
    { };
} // namespace thread_cpp_detail



void detail::sleep_for_impl(const struct timespec *reltime)
{
    using namespace std;
    timespec remaining = *reltime;
    
    int err = 0;
    do
    {
        using thread_cpp_detail::nanosleep_impl;
        if(remaining.tv_sec < 0 || remaining.tv_nsec < 0)
            break;
        err = nanosleep_impl::call(&remaining, &remaining);
    }
    while (err == -1 && errno == EINTR);
}

#endif

namespace stdex
{
    void notify_all_at_thread_exit(condition_variable& cond, unique_lock<mutex> &lk)
    {
        unique_lock<mutex> lk2;

        lk.swap(lk2);

        thread_notification_data::add_to_this_thread_notification_data(&cond, &lk2);
    }
}


#undef _STDEX_NOEXCEPT_FUNCTION


