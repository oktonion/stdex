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

#if defined(_DEBUG) || defined(DEBUG)
    #include <iostream> // for logging
#endif

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
    namespace detail
    {
        template<class _Tp>
        static void _throw_system_error(const _Tp &_errc)
        {
            throw stdex::system_error(
                stdex::make_error_code(_errc));
        }
    }

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
        typedef chrono::system_clock clock_t;

    public:
        typedef pthread_cond_t* native_handle_type;

        condition_variable()
        {
            int _err = 
                pthread_cond_init(&_condition_handle, NULL);

            if (0 != _err)
                throw(
                    stdex::system_error( 
                        stdex::make_error_code(stdex::errc::errc_t(_err)) 
                    )
                );
        }

        ~condition_variable() _STDEX_NOEXCEPT(false)
        {
            int _err = 
                pthread_cond_destroy(&_condition_handle);

            if (0 != _err)
                detail::_throw_system_error(stdex::errc::errc_t(_err));
        }

        inline void wait(unique_lock<mutex> &_lock) _STDEX_NOEXCEPT_FUNCTION
        {
            int _err = 
                pthread_cond_wait(&_condition_handle, detail::_lock_mutex_native_handle(_lock));

            if (0 != _err)
            {
            #if defined(_DEBUG) || defined(DEBUG)
                std::cerr << stdex::error_code(
                    stdex::make_error_code(stdex::errc::errc_t(_err))
                ).message() << std::endl;
            #endif
                std::terminate();
            }
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
            chrono::steady_clock::time_point _atime = 
                chrono::steady_clock::now() + chrono::duration_cast<chrono::steady_clock::duration>(_rtime);

            // exactly the same as wait_until with predicate
            while (!_p())
                if (wait_until(_lock, _atime) == cv_status::timeout)
                    return _p();
            return true;
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

            stdex::timespec _tp_as_ts = 
                clock_t::to_timespec(chrono::time_point_cast<clock_t::duration>(_atime));
            
            ::timespec _ts;

            _ts.tv_nsec = _tp_as_ts.tv_nsec;
            _ts.tv_sec = _tp_as_ts.tv_sec;

            int _err =
                pthread_cond_timedwait(&_condition_handle, detail::_lock_mutex_native_handle(_lock), &_ts);

        #ifdef ETIMEDOUT
            if(_err && _err != ETIMEDOUT)
            {
            #if defined(_DEBUG) || defined(DEBUG)
                std::cerr << stdex::error_code(
                    stdex::make_error_code(stdex::errc::errc_t(_err))
                ).message() << std::endl;
            #endif
                std::terminate();
            }
        #endif

            return (clock_t::now() < _atime
                ? cv_status::no_timeout : cv_status::timeout);
        }

        template<class _Rep, class _Period>
        cv_status wait_for_impl(unique_lock<mutex> &_lock, chrono::duration<_Rep, _Period> _rtime)
        {
            if (!detail::_lock_owns_lock(_lock))
                std::terminate();

            if (_rtime.count() < 0)
                return cv_status::timeout;

            typedef chrono::system_clock time_measure_clock_t;

            if (ratio_greater<time_measure_clock_t::period, _Period>::value)
                ++_rtime;

            time_measure_clock_t::time_point 
                _start_time_point = time_measure_clock_t::now(),
                _end_time_point = _start_time_point + stdex::chrono::duration_cast<time_measure_clock_t::duration>(_rtime);

            stdex::timespec _tp_as_ts =
                clock_t::to_timespec(clock_t::now() + stdex::chrono::duration_cast<clock_t::duration>(_rtime));

            ::timespec _ts;
            _ts.tv_sec = _tp_as_ts.tv_sec;
            _ts.tv_nsec = _tp_as_ts.tv_nsec;

            if ((time_measure_clock_t::now() - _start_time_point) > _rtime)
                return cv_status::timeout;

            int _err = 
                pthread_cond_timedwait(&_condition_handle, detail::_lock_mutex_native_handle(_lock), &_ts);
            
        #ifdef ETIMEDOUT
            if(_err && _err != ETIMEDOUT)
            {
            #if defined(_DEBUG) || defined(DEBUG)
                std::cerr << stdex::error_code(
                    stdex::make_error_code(stdex::errc::errc_t(_err))
                ).message() << std::endl;
            #endif
                std::terminate();
            }
        #endif

            return (time_measure_clock_t::now() - _start_time_point < _rtime
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
