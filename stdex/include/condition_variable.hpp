#ifndef _STDEX_CONDITION_VARIABLE_H
#define _STDEX_CONDITION_VARIABLE_H
 
 #if _MSC_VER > 1000
 #pragma once
 #endif // _MSC_VER > 1000
 
 // stdex includes
 #include "./chrono.hpp"
 #include "./system_error.hpp"
 
 // POSIX includes
 #include <pthread.h>
 
 // std includes
 #include <memory>
 #include <limits>
 
 #ifdef _STDEX_NATIVE_CPP11_SUPPORT
 
 #define _STDEX_DELETED_FUNCTION =delete
 #define _STDEX_NOEXCEPT_FUNCTION noexcept
 #define _STDEX_NOEXCEPT(args) noexcept(args)
 
 #else
 
 #define _STDEX_DELETED_FUNCTION 
 #define _STDEX_NOEXCEPT_FUNCTION throw()
 #define _STDEX_NOEXCEPT(args)
 
 #endif

// <mutex.hpp> forward declarations
namespace stdex
{
    class mutex;

    template<class _Tp>
    class unique_lock;

    namespace detail
    {
        pthread_mutex_t* _lock_mutex_native_handle(const unique_lock<mutex>&);
        bool _lock_owns_lock(const unique_lock<mutex>&);
    } // namespace detail

} // namespace stdex

 namespace stdex
 {
     /// cv_status
     struct cv_status
     {
         enum cv_status_t { no_timeout, timeout };
 
         cv_status(cv_status_t _cvs) :
             _cvstat(_cvs)
         {
 
         }
 
         operator cv_status_t() const
         {
             return _cvstat;
         }
 
         private:
             cv_status_t _cvstat;
     };
 
     class condition_variable 
     {
         typedef chrono::steady_clock clock_t;
 
     public:
         typedef pthread_cond_t* native_handle_type;
 
         condition_variable()
         {
             int _err = 
                pthread_cond_init(&_condition_handle, NULL);

             if (0 != _err)
                 throw(stdex::system_error( stdex::errc::errc_t(_err)) );
         }
 
         ~condition_variable() _STDEX_NOEXCEPT(false)
         {
             int _err = 
                pthread_cond_destroy(&_condition_handle);

             if (0 != _err)
                 throw(stdex::system_error( stdex::errc::errc_t(_err)) );
         }
 
         inline void wait(unique_lock<mutex> &_lock) _STDEX_NOEXCEPT_FUNCTION
         {
             int _err = 
                 pthread_cond_wait(&_condition_handle, detail::_lock_mutex_native_handle(_lock));
 
             if (0 != _err)
                 std::terminate();
         }
 
         template<class _Predicate>
         void wait(unique_lock<mutex>& _lock, _Predicate _p)
         {
             while (!_p())
                 wait(_lock);
         }
 
         template<class _Duration>
         cv_status wait_until(unique_lock<mutex> &_lock, const chrono::time_point<clock_t, _Duration> &_atime)
         {
             return wait_until_impl(_lock, _atime);
         }
 
         template<class _Clock, class _Duration>
         cv_status wait_until(unique_lock<mutex> &_lock, const chrono::time_point<_Clock, _Duration> &_atime)
         {
             // DR 887 - Sync unknown clock to known clock.
             const typename _Clock::time_point _c_entry = _Clock::now();
             const clock_t::time_point _s_entry = clock_t::now();
 
             return wait_until_impl(_lock, (_s_entry + (_atime - _c_entry)));
         }
 
         template<class _Clock, class _Duration, class _Predicate>
         bool wait_until(unique_lock<mutex> &_lock, const chrono::time_point<_Clock, _Duration> &_atime, _Predicate _p)
         {
             while (!_p())
                 if (wait_until(_lock, _atime) == cv_status::timeout)
                     return _p();
             return true;
         }
 
         template<class _Rep, class _Period>
         cv_status wait_for(unique_lock<mutex> &_lock, const chrono::duration<_Rep, _Period> &_rtime)
         {
             return wait_for_impl(_lock, _rtime);
         }
 
         template<class _Rep, class _Period, class _Predicate>
         bool wait_for(unique_lock<mutex> &_lock, const chrono::duration<_Rep, _Period> &_rtime, _Predicate _p)
         {
             return wait_until(_lock, clock_t::now() + chrono::duration_cast<clock_t::duration>(_rtime), _p);
         }
 
         native_handle_type native_handle()
         {
             return &_condition_handle;
         }
 
         inline void notify_one() _STDEX_NOEXCEPT_FUNCTION
         {
             pthread_cond_signal(&_condition_handle);
         }

         inline void notify_all() _STDEX_NOEXCEPT_FUNCTION
         {
             pthread_cond_broadcast(&_condition_handle);
         }
 
     private:
 
         template<class _Dur>
         cv_status wait_until_impl(unique_lock<mutex> &_lock, const chrono::time_point<clock_t, _Dur> &_atime)
         {
             if (!detail::_lock_owns_lock(_lock))
                 std::terminate();
 
             chrono::time_point<clock_t, chrono::seconds> _s = chrono::time_point_cast<chrono::seconds>(_atime) - chrono::milliseconds(1500);
             chrono::nanoseconds _ns = chrono::duration_cast<chrono::nanoseconds>(_atime - _s);
 
             typename chrono::time_point<clock_t, chrono::seconds>::rep _s_count = _s.time_since_epoch().count();
 
             timespec _ts;

             const stdex::time_t _ts_sec_max = 
                std::numeric_limits<stdex::time_t>::max();
             if (_s_count < _ts_sec_max)
             {
                _ts.tv_sec = static_cast<stdex::time_t>(_s_count > 0 ? _s_count : 0);
                _ts.tv_nsec = static_cast<long>(_ns.count());
             }
             else
             {
                _ts.tv_sec = _ts_sec_max;
                _ts.tv_nsec = (unsigned long)(0) - (unsigned long)(1);
             }
 
             int _err =
                pthread_cond_timedwait(&_condition_handle, detail::_lock_mutex_native_handle(_lock), &_ts);

             #ifdef ETIMEDOUT
                if(_err && _err != ETIMEDOUT)
                    std::terminate();
             #endif
 
             return (clock_t::now() < _atime
                 ? cv_status::no_timeout : cv_status::timeout);
         }
 
         template<class _Rep, class _Period>
         cv_status wait_for_impl(unique_lock<mutex> &_lock, const chrono::duration<_Rep, _Period> &_rtime)
         {
             clock_t::time_point _start_time_point = clock_t::now();
 
             if (!detail::_lock_owns_lock(_lock))
                 std::terminate();
 
             chrono::seconds _rs = chrono::duration_cast<chrono::seconds>(_rtime);
             chrono::nanoseconds _rns = chrono::duration_cast<chrono::nanoseconds>(_rtime - _rs);
 
             timespec _ts;
 
             _ts.tv_sec = static_cast<stdex::time_t>(_rs.count());
             _ts.tv_nsec = static_cast<long>(_rns.count());
 
             chrono::time_point<clock_t, chrono::seconds> _sec = chrono::time_point_cast<chrono::seconds>(_start_time_point);
             chrono::nanoseconds _nsec = chrono::duration_cast<chrono::nanoseconds>(_start_time_point - _sec);
 
             _ts.tv_sec += static_cast<stdex::time_t>(_sec.time_since_epoch().count());
             _ts.tv_nsec += static_cast<long>(_nsec.count());
 
             int _err = 
                 pthread_cond_timedwait(&_condition_handle, detail::_lock_mutex_native_handle(_lock), &_ts);
             
             #ifdef ETIMEDOUT
                if(_err && _err != ETIMEDOUT)
                    std::terminate();
             #endif
 
             return (clock_t::now() - _start_time_point < _rtime
                 ? cv_status::no_timeout : cv_status::timeout);
         }
 
         pthread_cond_t _condition_handle;
 
         condition_variable(const condition_variable&) _STDEX_DELETED_FUNCTION;
         condition_variable& operator=(const condition_variable&) _STDEX_DELETED_FUNCTION;
     };
 
     void notify_all_at_thread_exit(condition_variable &cond, unique_lock<mutex> &lk); 
 } // namespace stdex
 
 #undef _STDEX_DELETED_FUNCTION
 #undef _STDEX_NOEXCEPT_FUNCTION
 #undef _STDEX_NOEXCEPT
 
 #endif // _STDEX_CONDITION_VARIABLE_H
