#ifndef _STDEX_MUTEX_H
#define _STDEX_MUTEX_H
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
// stdex includes
#include "./system_error.hpp"
#include "./chrono.hpp"
#include "./condition_variable.hpp"
#include "./thread.hpp"
 
// POSIX includes
#include <pthread.h>
 
// std includes
#include <algorithm>
 
#ifdef _STDEX_NATIVE_CPP11_SUPPORT
 
#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept
#define _STDEX_NOEXCEPT(args) noexcept(args)
 
#else
 
#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()
#define _STDEX_NOEXCEPT(args)
 
#endif
 
 
namespace stdex
{
    namespace detail
    {
        class _mutex_base
        {
        protected:

            pthread_mutex_t  _mutex_handle;

            _mutex_base() _STDEX_NOEXCEPT_FUNCTION
            {
                // XXX EAGAIN, ENOMEM, EPERM, EBUSY(may), EINVAL(may)
                int _err = pthread_mutex_init(&_mutex_handle, NULL);
                if (0 != _err)
                    _throw_system_error(stdex::errc::errc_t(_err));
            }

            ~_mutex_base() _STDEX_NOEXCEPT(false)
            { 
                int _err = pthread_mutex_destroy(&_mutex_handle);
                if (0 != _err)
                    _throw_system_error(stdex::errc::errc_t(_err));
            }

        private:
            _mutex_base(const _mutex_base&) _STDEX_DELETED_FUNCTION;
            _mutex_base& operator=(const _mutex_base&) _STDEX_DELETED_FUNCTION;
        };

        class _recursive_mutex_base
        {
        protected:
            pthread_mutex_t  _mutex_handle;

            _recursive_mutex_base()
            {
                pthread_mutexattr_t attr;

                int _err = pthread_mutexattr_init(&attr);

                if (0 == _err)
                {
                    _err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

                    if (0 == _err)
                    _err = pthread_mutex_init(&_mutex_handle, &attr); // XXX EAGAIN, ENOMEM, EPERM, EBUSY(may), EINVAL(may)
                }

                if (0 != _err)
                    throw(
                        stdex::system_error(
                            stdex::make_error_code(stdex::errc::errc_t(_err))
                        ) 
                    );
            }

            ~_recursive_mutex_base() _STDEX_NOEXCEPT(false)
            {
                int _err = pthread_mutex_destroy(&_mutex_handle);
                if (0 != _err)
                    _throw_system_error(stdex::errc::errc_t(_err));
            }

        private:
            _recursive_mutex_base(const _recursive_mutex_base&) _STDEX_DELETED_FUNCTION;
            _recursive_mutex_base& operator=(const _recursive_mutex_base&) _STDEX_DELETED_FUNCTION;
        };
    }

    class mutex:
        private detail::_mutex_base
    {
    public:
        typedef pthread_mutex_t* native_handle_type;

        mutex() _STDEX_NOEXCEPT_FUNCTION
        {}

        ~mutex()
        {}

        inline void lock()
        {
            int _err = pthread_mutex_lock(&_mutex_handle);

            // EINVAL, EAGAIN, EBUSY, EINVAL, EDEADLK(may)
            if (0 != _err)
                throw system_error( 
                    stdex::make_error_code(stdex::errc::errc_t(_err))
                );
        }

        inline bool try_lock() _STDEX_NOEXCEPT_FUNCTION
        {
            // XXX EINVAL, EAGAIN, EBUSY
            return (pthread_mutex_trylock(&_mutex_handle) == 0) ? true : false;
        }

        inline void unlock()
        {
            int _err = pthread_mutex_unlock(&_mutex_handle);

            if (0 != _err)
                throw system_error( 
                    stdex::make_error_code(stdex::errc::errc_t(_err))
                );
        }

        native_handle_type native_handle() _STDEX_NOEXCEPT_FUNCTION
        {
            return &_mutex_handle;
        }

    private:

        mutex(const mutex&) _STDEX_DELETED_FUNCTION;
        mutex& operator=(const mutex&) _STDEX_DELETED_FUNCTION;
    };

    class recursive_mutex :
        private detail::_recursive_mutex_base
    {
    public:
        typedef pthread_mutex_t* native_handle_type;

        recursive_mutex()
        {}

        ~recursive_mutex()
        {}

        inline void lock()
        {
            int _err = pthread_mutex_lock(&_mutex_handle);

            // EINVAL, EAGAIN, EBUSY, EINVAL, EDEADLK(may)
            if (0 != _err)
                throw system_error(
                    stdex::make_error_code(stdex::errc::errc_t(_err))
                );
        }

        inline bool try_lock()
        {
            // XXX EINVAL, EAGAIN, EBUSY
            return (pthread_mutex_trylock(&_mutex_handle) == 0) ? true : false;
        }

        inline void unlock()
        {
            int _err = pthread_mutex_unlock(&_mutex_handle);
            // XXX EINVAL, EAGAIN, EBUSY
            if (0 != _err)
                throw system_error(
                    stdex::make_error_code(stdex::errc::errc_t(_err))
                );
        }

        native_handle_type native_handle() _STDEX_NOEXCEPT_FUNCTION
        {
            return &_mutex_handle;
        }

    private:

        recursive_mutex(const recursive_mutex&) _STDEX_DELETED_FUNCTION;
        recursive_mutex& operator=(const recursive_mutex&) _STDEX_DELETED_FUNCTION;
    };

    // LOCK PROPERTIES
    struct adopt_lock_t
    {};

    struct defer_lock_t
    {};

    struct try_to_lock_t
    {};

    extern const adopt_lock_t adopt_lock;
    extern const defer_lock_t defer_lock;
    extern const try_to_lock_t try_to_lock;

    template <class _Lockbl>
    class lock_guard {
    public:
        typedef _Lockbl mutex_type;

        explicit lock_guard(mutex_type &_m):
            _device(_m)
        {
            _device.lock();
        }

        lock_guard(mutex_type &_m, adopt_lock_t) _STDEX_NOEXCEPT_FUNCTION:
            _device(_m)
        {}

        ~lock_guard()
        {
            _device.unlock();
        }


    private:
        mutex_type &_device;

        lock_guard(const lock_guard&) _STDEX_DELETED_FUNCTION;
        lock_guard& operator=(const lock_guard&) _STDEX_DELETED_FUNCTION;
    };

    template<class _Lockbl>
    class unique_lock
    {
    public:
        typedef _Lockbl mutex_type;

        unique_lock() _STDEX_NOEXCEPT_FUNCTION : 
            _device(0), 
            _owns(false)
        { }

        explicit unique_lock(mutex_type &_m): 
            _device(&(_m)),
            _owns(false)
        {
            lock();
            _owns = true;
        }

        unique_lock(mutex_type &_m, defer_lock_t) _STDEX_NOEXCEPT_FUNCTION: 
            _device(&(_m)), 
            _owns(false)
        { }

        unique_lock(mutex_type &_m, try_to_lock_t): 
            _device(&(_m)), 
            _owns(_device->try_lock())
        { }

        unique_lock(mutex_type &_m, adopt_lock_t) _STDEX_NOEXCEPT_FUNCTION: 
            _device(&(_m)), 
            _owns(true)
        {
            // XXX calling thread owns mutex
        }

        template<class _Clock, class _Duration>
        unique_lock(mutex_type &_m, const chrono::time_point<_Clock, _Duration> &atime): 
            _device(&(_m)),
            _owns(_device->try_lock_until(atime))
        { }

        template<class _Rep, class _Period>
        unique_lock(mutex_type &_m, const chrono::duration<_Rep, _Period> &rtime):
            _device(&(_m)),
            _owns(_device->try_lock_for(rtime))
        { }

        ~unique_lock()
        {
            if (_owns)
                unlock();
        }

        /* move move move, no move in pre-C++11
        unique_lock(unique_lock &&u) _STDEX_NOEXCEPT_FUNCTION: 
            _device(u._device), 
            _owns(u._owns)
        {
            u._device = 0;
            u._owns = false;
        }

        unique_lock& operator=(unique_lock &&u) _STDEX_NOEXCEPT_FUNCTION
        {
            if (_owns)
                unlock();

            unique_lock(std::move(u)).swap(*this);

            u._device = 0;
            u._owns = false;

            return *this;
        }
        */

        void lock()
        {
            if (!_device)
                throw system_error(
                    stdex::make_error_code(errc::operation_not_permitted)
                );
            else if (_owns)
                throw system_error(
                    stdex::make_error_code(errc::resource_deadlock_would_occur)
                );
            else
            {
                _device->lock();
                _owns = true;
            }
        }

        bool try_lock()
        {
            if (!_device)
                throw system_error(
                    stdex::make_error_code(errc::operation_not_permitted)
                );
            else if (_owns)
                throw system_error(
                    stdex::make_error_code(errc::resource_deadlock_would_occur)
                );
            else
            {
                _owns = _device->try_lock();
                return _owns;
            }
        }

        template<class _Clock, class _Duration>
        bool try_lock_until(const chrono::time_point<_Clock, _Duration> &atime)
        {
            if (!_device)
                throw system_error(
                    stdex::make_error_code(errc::operation_not_permitted)
                );
            else if (_owns)
                throw system_error(
                    stdex::make_error_code(errc::resource_deadlock_would_occur)
                );
            else
            {
                _owns = _device->try_lock_until(atime);
                return _owns;
            }
        }

        template<class _Rep, class _Period>
        bool try_lock_for(const chrono::duration<_Rep, _Period> &rtime)
        {
            if (!_device)
                throw system_error(
                    stdex::make_error_code(errc::operation_not_permitted)
                );
            else if (_owns)
                throw system_error(
                    stdex::make_error_code(errc::resource_deadlock_would_occur)
                );
            else
            {
                _owns = _device->try_lock_for(rtime);
                return _owns;
            }
        }

        void unlock()
        {
            if (!_owns)
                throw system_error(
                    stdex::make_error_code(errc::operation_not_permitted)
                );
            else if (_device)
            {
                _device->unlock();
                _owns = false;
            }
        }

        void swap(unique_lock &other) _STDEX_NOEXCEPT_FUNCTION
        {
            {
                mutex_type *tmp_value = _device;
                _device = other._device;
                other._device = tmp_value;
            }

            {
                bool tmp_value = _owns;
                _owns = other._owns;
                other._owns = tmp_value;
            }
        }

        mutex_type* release() _STDEX_NOEXCEPT_FUNCTION
        {
            mutex_type *ret = _device;
            _device = 0;
            _owns = false;
            return ret;
        }

        bool owns_lock() const _STDEX_NOEXCEPT_FUNCTION
        {
            return _owns;
        }

        operator bool() const _STDEX_NOEXCEPT_FUNCTION
        {
            return owns_lock();
        }

        mutex_type* mutex() const _STDEX_NOEXCEPT_FUNCTION
        {
            return _device;
        }

    private:
        mutex_type *_device;
        bool _owns; // XXX use atomic_bool

        unique_lock(const unique_lock&) _STDEX_DELETED_FUNCTION;
        unique_lock& operator=(const unique_lock&) _STDEX_DELETED_FUNCTION;
    };

    class timed_mutex;
    class recursive_timed_mutex;

    namespace detail
    {
      
        namespace mutex_type_traits
        {
#ifndef PTW32_VERSION
            float* pthread_mutex_timedlock(...); // dummy   
#endif

            template<class _Tp>
            _Tp declval();

            _yes_type _pthread_func_tester(int);
            _no_type _pthread_func_tester(...);

            struct _has_pthread_mutex_timedlock
            {
                static const bool value =
                    sizeof( 
                        _pthread_func_tester(
                            pthread_mutex_timedlock(
                                declval< ::pthread_mutex_t*>(), 
                                declval< ::timespec* >()
                        )
                    ) 
                ) == sizeof(_yes_type);
            };
        }
        
        struct _timed_mutex_with_timedlock
        {

        public:
            template<class _Rep, class _Period>
            static bool try_lock_for(pthread_mutex_t& _mutex_handle, 
                const chrono::duration<_Rep, _Period>& _rtime)
            {
                typedef chrono::system_clock _clock;

                _clock::duration _rt = 
                    chrono::duration_cast<_clock::duration>(_rtime);

                const bool check =
                    ratio_greater<_clock::period, _Period>::value;
                if (check)
                    ++_rt;
                return try_lock_until1(_mutex_handle, _clock::now() + _rt);
            }

            template<class _Duration, class _MtxHandle>
            static bool try_lock_until1(_MtxHandle& _mutex_handle,
                const chrono::time_point<chrono::system_clock, _Duration>& _atime)
            {
                stdex::timespec _tp_as_ts = 
                    chrono::system_clock::to_timespec(_atime);
                
                ::timespec _ts;

                _ts.tv_nsec = _tp_as_ts.tv_nsec;
                _ts.tv_sec = _tp_as_ts.tv_sec;

                bool success = 
                    (pthread_mutex_timedlock(&_mutex_handle, &_ts) == 0);

                return success;
            }

            template<class _Clock, class _Duration>
            static bool try_lock_until(pthread_mutex_t& _mutex_handle,
                const chrono::time_point<_Clock, _Duration>& _atime)
            {
                // The user-supplied clock may not tick at the same rate as
                // steady_clock, so we must loop in order to guarantee that
                // the timeout has expired before returning false.
                typename _Clock::time_point _now = _Clock::now();
                do {
                    typename _Clock::duration _rtime = _atime - _now;
                    if (try_lock_for(_mutex_handle, _rtime))
                        return true;
                    _now = _Clock::now();
                } while (_atime > _now);
                return false;
            }
        };

        template<class, bool>
        class _timed_mutex_impl_base;

        // without pthread_mutex_timedlock support
        template<>
        class _timed_mutex_impl_base<_mutex_base, false>
        { 
            struct _Pred
            {
                bool operator()() const {
                    return !*_locked;
                }

                bool* _locked;
            } ;

        public:

            typedef mutex::native_handle_type native_handle_type;

            ~_timed_mutex_impl_base()
            {}

            void lock()
            {
                unique_lock<mutex> _lk(_mut);

                _Pred _pred;
                _pred._locked = &_locked;

                _cv.wait(_lk, _pred);
                _locked = true;
            }

            bool try_lock()
            {
                lock_guard<mutex> _lk(_mut);
                if (_locked)
                    return false;
                _locked = true;
                return true;
            }

            template<class _Rep, class _Period>
            bool try_lock_for(const chrono::duration<_Rep, _Period>& _rtime)
            {
                unique_lock<mutex> _lk(_mut);

                _Pred _pred;
                _pred._locked = &_locked;

                if (!_cv.wait_for(_lk, _rtime, _pred))
                    return false;
                _locked = true;
                return true;
            }

            template<class _Clock, class _Duration>
            bool try_lock_until(const chrono::time_point<_Clock, _Duration>& _atime)
            {
                unique_lock<mutex> _lk(_mut);

                _Pred _pred;
                _pred._locked = &_locked;

                if (!_cv.wait_until(_lk, _atime, _pred))
                    return false;
                _locked = true;
                return true;
            }

            void unlock()
            {
                lock_guard<mutex> _lk(_mut);

                _locked = false;
                _cv.notify_one();
            }

            native_handle_type native_handle() _STDEX_NOEXCEPT_FUNCTION
            {
                return _mut.native_handle();
            }

        protected:
        _timed_mutex_impl_base() _STDEX_NOEXCEPT_FUNCTION:
                _locked(false)
            {}

        private:

            mutex        _mut;
            condition_variable    _cv;
            bool        _locked;
        };

        template<>
        class _timed_mutex_impl_base<_recursive_mutex_base, false>
        {
            // Predicate type that tests whether the current thread can lock a mutex.
            class _Can_lock
            {
            public:
                // Returns true if the mutex is unlocked or is locked by _caller.
                bool operator()() const _STDEX_NOEXCEPT_FUNCTION
                {
                    return _mx->_count == 0 || _mx->_owner == _caller;
                }

                const _timed_mutex_impl_base<_recursive_mutex_base, false>* _mx;
                thread::id _caller;
            };

            friend class _timed_mutex_impl_base<_recursive_mutex_base, false>::_Can_lock;

        public:

            typedef mutex::native_handle_type native_handle_type;

            ~_timed_mutex_impl_base()
            {}

            void lock()
            {
                thread::id _id = this_thread::get_id();
                _Can_lock _can_lock;

                _can_lock._mx = this;
                _can_lock._caller = _id;

                unique_lock<mutex> _lk(_mut);
                _cv.wait(_lk, _can_lock);
                if ((0u - 1u) == _count)
                    throw system_error(
                        stdex::make_error_code(stdex::errc::errc_t(EAGAIN))
                    );// [thread.timedmutex.recursive]/3
                _owner = _id;
                ++_count;
            }

            bool try_lock()
            {
                thread::id _id = this_thread::get_id();
                _Can_lock _can_lock;

                _can_lock._mx = this;
                _can_lock._caller = _id;

                lock_guard<mutex> _lk(_mut);
                if (!_can_lock())
                    return false;
                if ((0u - 1u) == _count)
                    return false;
                _owner = _id;
                ++_count;
                return true;
            }

            template<class _Rep, class _Period>
            bool try_lock_for(const chrono::duration<_Rep, _Period>& _rtime)
            {
                thread::id _id = this_thread::get_id();
                _Can_lock _can_lock;

                _can_lock._mx = this;
                _can_lock._caller = _id;

                unique_lock<mutex> _lk(_mut);
                if (!_cv.wait_for(_lk, _rtime, _can_lock))
                    return false;
                if ((0u - 1u) == _count)
                    return false;
                _owner = _id;
                ++_count;
                return true;
            }

            template<class _Clock, class _Duration>
            bool try_lock_until(const chrono::time_point<_Clock, _Duration>& _atime)
            {
                thread::id _id = this_thread::get_id();
                _Can_lock _can_lock;

                _can_lock._mx = this;
                _can_lock._caller = _id;

                unique_lock<mutex> _lk(_mut);
                if (!_cv.wait_until(_lk, _atime, _can_lock))
                    return false;
                if ((0u - 1u) == _count)
                    return false;
                _owner = _id;
                ++_count;
                return true;
            }

            void unlock()
            {
                lock_guard<mutex> _lk(_mut);

                if (--_count == 0)
                {
                    _owner = thread::id();
                    _cv.notify_one();
                }
            }

            native_handle_type native_handle() _STDEX_NOEXCEPT_FUNCTION
            {
                return _mut.native_handle();
            }

        protected:
            _timed_mutex_impl_base() _STDEX_NOEXCEPT_FUNCTION :
                _count(0)
            {}

        private:

            mutex        _mut;
            condition_variable    _cv;
            thread::id        _owner;
            unsigned        _count;

            _timed_mutex_impl_base(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
            _timed_mutex_impl_base& operator=(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
        };

        // with pthread_mutex_timedlock support
        template<>
        class _timed_mutex_impl_base<_mutex_base, true> :
            private _mutex_base
        {
        public:

            typedef mutex::native_handle_type native_handle_type;

            ~_timed_mutex_impl_base()
            {}

            inline void lock()
            {
                int _err = pthread_mutex_lock(&_mutex_handle);

                // EINVAL, EAGAIN, EBUSY, EINVAL, EDEADLK(may)
                if (_err)
                    throw system_error(
                        stdex::make_error_code(stdex::errc::errc_t(_err))
                    );
            }

            inline bool try_lock() _STDEX_NOEXCEPT_FUNCTION
            {
                // XXX EINVAL, EAGAIN, EBUSY
                return (pthread_mutex_trylock(&_mutex_handle) == 0) ? true : false;
            }

            template <class _Rep, class _Period>
            bool try_lock_for(const chrono::duration<_Rep, _Period>& _rtime)
            {
                return _timed_mutex_with_timedlock::try_lock_for(_mutex_handle, _rtime);
            }

            template <class _Clock, class _Duration>
            bool try_lock_until(const chrono::time_point<_Clock, _Duration>& _atime)
            {
                return _timed_mutex_with_timedlock::try_lock_until(_mutex_handle, _atime);
            }

            inline void unlock()
            {
                pthread_mutex_unlock(&_mutex_handle);
            }

            native_handle_type native_handle() _STDEX_NOEXCEPT_FUNCTION
            {
                return &_mutex_handle;
            }

        protected:

            _timed_mutex_impl_base() _STDEX_NOEXCEPT_FUNCTION
            {}

        private:

            _timed_mutex_impl_base(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
            _timed_mutex_impl_base& operator=(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
        };

        template<>
        class _timed_mutex_impl_base<_recursive_mutex_base, true> :
            private _recursive_mutex_base
        {
        public:

            typedef mutex::native_handle_type native_handle_type;

            ~_timed_mutex_impl_base()
            {}

            inline void lock()
            {
                int _err = pthread_mutex_lock(&_mutex_handle);

                // EINVAL, EAGAIN, EBUSY, EINVAL, EDEADLK(may)
                if (_err)
                    throw system_error(
                        stdex::make_error_code(stdex::errc::errc_t(_err))
                    );
            }

            inline bool try_lock()
            {
                // XXX EINVAL, EAGAIN, EBUSY
                return (pthread_mutex_trylock(&_mutex_handle) == 0) ? true : false;
            }

            template <class _Rep, class _Period>
            bool try_lock_for(const chrono::duration<_Rep, _Period>& _rtime)
            {
                return _timed_mutex_with_timedlock::try_lock_for(_mutex_handle, _rtime);
            }

            template <class _Clock, class _Duration>
            bool try_lock_until(const chrono::time_point<_Clock, _Duration>& _atime)
            {
                return _timed_mutex_with_timedlock::try_lock_until(_mutex_handle, _atime);
            }

            inline void unlock()
            {
                // XXX EINVAL, EAGAIN, EBUSY
                pthread_mutex_unlock(&_mutex_handle);
            }

            native_handle_type native_handle() _STDEX_NOEXCEPT_FUNCTION
            {
                return &_mutex_handle;
            }

        protected:

            _timed_mutex_impl_base()
            {}

        private:

            _timed_mutex_impl_base(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
            _timed_mutex_impl_base& operator=(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
        };

        template<class>
        class _timed_mutex_impl;

        template<>
        class _timed_mutex_impl<timed_mutex> :
            public _timed_mutex_impl_base<
                _mutex_base,
                mutex_type_traits::_has_pthread_mutex_timedlock::value
            >
        { 
        protected:
            _timed_mutex_impl() _STDEX_NOEXCEPT_FUNCTION
            {}
        };

        template<>
        class _timed_mutex_impl<recursive_timed_mutex> :
            public _timed_mutex_impl_base<
                _recursive_mutex_base,
                mutex_type_traits::_has_pthread_mutex_timedlock::value
            >
        { 
        protected:
            _timed_mutex_impl()
            {}
        };
    }

    class timed_mutex :
        public detail::_timed_mutex_impl<timed_mutex>
    { };

    class recursive_timed_mutex :
        public detail::_timed_mutex_impl<recursive_timed_mutex>
    { };

    /// Swap overload for unique_lock objects.
    template<class _Lockbl>
    inline void swap(stdex::unique_lock<_Lockbl> &lhs, stdex::unique_lock<_Lockbl> &rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        lhs.swap(rhs);
    }

    namespace detail
    {
        pthread_mutex_t* _lock_mutex_native_handle(const unique_lock<mutex>&);
        bool _lock_owns_lock(const unique_lock<mutex>&);
    } // namespace detail

} // namespace stdex

// mutex algorithms

namespace stdex
{
    namespace detail
    {

        namespace
        {
            template <class _Lockbl1, class _Lockbl2>
            unsigned int _try_lock(_Lockbl1 &_m1, _Lockbl2 &_m2)
            {
                stdex::unique_lock<_Lockbl1> _l1(_m1, stdex::try_to_lock);

                if (!_l1)
                    return 1;

                if (!_m2.try_lock())
                    return 2;

                _l1.release();
                return 0;
            }

            template <class _Lockbl1, class _Lockbl2>
            unsigned int _lock_helper(_Lockbl1 &_m1, _Lockbl2 &_m2)
            {
                stdex::unique_lock<_Lockbl1> _l1(_m1);
                if (!_m2.try_lock())
                    return 1;
            
                _l1.release();
                return 0;
            }

// some boilreplate code:

#undef _STDEX_TRY_LOCK
#undef _STDEX_LOCK_HELPER

#undef _STDEX_TYPES
#undef _STDEX_ARGS
#undef _STDEX_ARGS_NAMES

#define _STDEX_TRY_LOCK(N) \
            template <class _Lockbl1, _STDEX_TYPES##N > \
            unsigned int _try_lock(_Lockbl1 &_m1, _STDEX_ARGS##N ) \
            { \
                stdex::unique_lock<_Lockbl1> _l1(_m1, stdex::try_to_lock); \
                if (!_l1) \
                    return 1; \
                \
                const unsigned int _failed_lock = _try_lock(_STDEX_ARGS_NAMES##N ); \
                \
                if (_failed_lock > 0) \
                    return _failed_lock + 1; \
                \
                _l1.release();\
                return 0;\
            } \
            template <class _Lockbl1, _STDEX_TYPES##N > \
            int try_lock(_Lockbl1 &_m1, _STDEX_ARGS##N ) \
            { \
                return ((int) _try_lock(_m1, _STDEX_ARGS_NAMES##N )) - 1; \
            }

#define _STDEX_LOCK_HELPER(N) \
            template <class _Lockbl1, _STDEX_TYPES##N > \
            unsigned int _lock_helper(_Lockbl1 &_m1, _STDEX_ARGS##N ) \
            { \
                stdex::unique_lock<_Lockbl1> _l1(_m1); \
                const unsigned int _failed_lock = _try_lock(_STDEX_ARGS_NAMES##N ); \
                if (_failed_lock > 0) \
                    return _failed_lock; \
                \
                _l1.release(); \
                return 0; \
            }



#define _STDEX_TYPES_N(N)      class _Lockbl##N
#define _STDEX_ARGS_N(N)       _Lockbl##N &_m##N
#define _STDEX_ARGS_NAMES_N(N) _m##N

#define      _STDEX_TYPES3  class _Lockbl2, class _Lockbl3
#define       _STDEX_ARGS3  _Lockbl2 &_m2, _Lockbl3 &_m3
#define _STDEX_ARGS_NAMES3  _m2, _m3
#define      _STDEX_TYPES4        _STDEX_TYPES3 , _STDEX_TYPES_N      (4)
#define       _STDEX_ARGS4         _STDEX_ARGS3 , _STDEX_ARGS_N       (4)
#define _STDEX_ARGS_NAMES4   _STDEX_ARGS_NAMES3 , _STDEX_ARGS_NAMES_N (4)
#define      _STDEX_TYPES5        _STDEX_TYPES4 , _STDEX_TYPES_N      (5)
#define       _STDEX_ARGS5         _STDEX_ARGS4 , _STDEX_ARGS_N       (5)
#define _STDEX_ARGS_NAMES5   _STDEX_ARGS_NAMES4 , _STDEX_ARGS_NAMES_N (5)
#define      _STDEX_TYPES6        _STDEX_TYPES5 , _STDEX_TYPES_N      (6)
#define       _STDEX_ARGS6         _STDEX_ARGS5 , _STDEX_ARGS_N       (6)
#define _STDEX_ARGS_NAMES6   _STDEX_ARGS_NAMES5 , _STDEX_ARGS_NAMES_N (6)
#define      _STDEX_TYPES7        _STDEX_TYPES6 , _STDEX_TYPES_N      (7)
#define       _STDEX_ARGS7         _STDEX_ARGS6 , _STDEX_ARGS_N       (7)
#define _STDEX_ARGS_NAMES7   _STDEX_ARGS_NAMES6 , _STDEX_ARGS_NAMES_N (7)
#define      _STDEX_TYPES8        _STDEX_TYPES7 , _STDEX_TYPES_N      (8)
#define       _STDEX_ARGS8         _STDEX_ARGS7 , _STDEX_ARGS_N       (8)
#define _STDEX_ARGS_NAMES8   _STDEX_ARGS_NAMES7 , _STDEX_ARGS_NAMES_N (8)
#define      _STDEX_TYPES9        _STDEX_TYPES8 , _STDEX_TYPES_N      (9)
#define       _STDEX_ARGS9         _STDEX_ARGS8 , _STDEX_ARGS_N       (9)
#define _STDEX_ARGS_NAMES9   _STDEX_ARGS_NAMES8 , _STDEX_ARGS_NAMES_N (9)
#define      _STDEX_TYPES10       _STDEX_TYPES9 , _STDEX_TYPES_N      (10)
#define       _STDEX_ARGS10        _STDEX_ARGS9 , _STDEX_ARGS_N       (10)
#define _STDEX_ARGS_NAMES10  _STDEX_ARGS_NAMES9 , _STDEX_ARGS_NAMES_N (10)
#define      _STDEX_TYPES11       _STDEX_TYPES10, _STDEX_TYPES_N      (11)
#define       _STDEX_ARGS11        _STDEX_ARGS10, _STDEX_ARGS_N       (11)
#define _STDEX_ARGS_NAMES11  _STDEX_ARGS_NAMES10, _STDEX_ARGS_NAMES_N (11)
#define      _STDEX_TYPES12       _STDEX_TYPES11, _STDEX_TYPES_N      (12)
#define       _STDEX_ARGS12        _STDEX_ARGS11, _STDEX_ARGS_N       (12)
#define _STDEX_ARGS_NAMES12  _STDEX_ARGS_NAMES11, _STDEX_ARGS_NAMES_N (12)
#define      _STDEX_TYPES13       _STDEX_TYPES12, _STDEX_TYPES_N      (13)
#define       _STDEX_ARGS13        _STDEX_ARGS12, _STDEX_ARGS_N       (13)
#define _STDEX_ARGS_NAMES13  _STDEX_ARGS_NAMES12, _STDEX_ARGS_NAMES_N (13)
#define      _STDEX_TYPES14       _STDEX_TYPES13, _STDEX_TYPES_N      (14)
#define       _STDEX_ARGS14        _STDEX_ARGS13, _STDEX_ARGS_N       (14)
#define _STDEX_ARGS_NAMES14  _STDEX_ARGS_NAMES13, _STDEX_ARGS_NAMES_N (14)
#define      _STDEX_TYPES15       _STDEX_TYPES14, _STDEX_TYPES_N      (15)
#define       _STDEX_ARGS15        _STDEX_ARGS14, _STDEX_ARGS_N       (15)
#define _STDEX_ARGS_NAMES15  _STDEX_ARGS_NAMES14, _STDEX_ARGS_NAMES_N (15)
#define      _STDEX_TYPES16       _STDEX_TYPES15, _STDEX_TYPES_N      (16)
#define       _STDEX_ARGS16        _STDEX_ARGS15, _STDEX_ARGS_N       (16)
#define _STDEX_ARGS_NAMES16  _STDEX_ARGS_NAMES15, _STDEX_ARGS_NAMES_N (16)
#define      _STDEX_TYPES17       _STDEX_TYPES16, _STDEX_TYPES_N      (17)
#define       _STDEX_ARGS17        _STDEX_ARGS16, _STDEX_ARGS_N       (17)
#define _STDEX_ARGS_NAMES17  _STDEX_ARGS_NAMES16, _STDEX_ARGS_NAMES_N (17)
#define      _STDEX_TYPES18       _STDEX_TYPES17, _STDEX_TYPES_N      (18)
#define       _STDEX_ARGS18        _STDEX_ARGS17, _STDEX_ARGS_N       (18)
#define _STDEX_ARGS_NAMES18  _STDEX_ARGS_NAMES17, _STDEX_ARGS_NAMES_N (18)
#define      _STDEX_TYPES19       _STDEX_TYPES18, _STDEX_TYPES_N      (19)
#define       _STDEX_ARGS19        _STDEX_ARGS18, _STDEX_ARGS_N       (19)
#define _STDEX_ARGS_NAMES19  _STDEX_ARGS_NAMES18, _STDEX_ARGS_NAMES_N (19)
#define      _STDEX_TYPES20       _STDEX_TYPES19, _STDEX_TYPES_N      (20)
#define       _STDEX_ARGS20        _STDEX_ARGS19, _STDEX_ARGS_N       (20)
#define _STDEX_ARGS_NAMES20  _STDEX_ARGS_NAMES19, _STDEX_ARGS_NAMES_N (20)
#define      _STDEX_TYPES21       _STDEX_TYPES20, _STDEX_TYPES_N      (21)
#define       _STDEX_ARGS21        _STDEX_ARGS20, _STDEX_ARGS_N       (21)
#define _STDEX_ARGS_NAMES21  _STDEX_ARGS_NAMES20, _STDEX_ARGS_NAMES_N (21)
#define      _STDEX_TYPES22       _STDEX_TYPES21, _STDEX_TYPES_N      (22)
#define       _STDEX_ARGS22        _STDEX_ARGS21, _STDEX_ARGS_N       (22)
#define _STDEX_ARGS_NAMES22  _STDEX_ARGS_NAMES21, _STDEX_ARGS_NAMES_N (22)
#define      _STDEX_TYPES23       _STDEX_TYPES22, _STDEX_TYPES_N      (23)
#define       _STDEX_ARGS23        _STDEX_ARGS22, _STDEX_ARGS_N       (23)
#define _STDEX_ARGS_NAMES23  _STDEX_ARGS_NAMES22, _STDEX_ARGS_NAMES_N (23)
#define      _STDEX_TYPES24       _STDEX_TYPES23, _STDEX_TYPES_N      (24)
#define       _STDEX_ARGS24        _STDEX_ARGS23, _STDEX_ARGS_N       (24)
#define _STDEX_ARGS_NAMES24  _STDEX_ARGS_NAMES23, _STDEX_ARGS_NAMES_N (24)

#define _STDEX_LOCK_DEFINE(N) \
    _STDEX_TRY_LOCK(N) \
    _STDEX_LOCK_HELPER(N)

            _STDEX_LOCK_DEFINE(3)
            _STDEX_LOCK_DEFINE(4)
            _STDEX_LOCK_DEFINE(5)
            _STDEX_LOCK_DEFINE(6)
            _STDEX_LOCK_DEFINE(7)
            _STDEX_LOCK_DEFINE(8)
            _STDEX_LOCK_DEFINE(9)
            _STDEX_LOCK_DEFINE(10)
            _STDEX_LOCK_DEFINE(11)
            _STDEX_LOCK_DEFINE(12)
            _STDEX_LOCK_DEFINE(13)
            _STDEX_LOCK_DEFINE(14)
            _STDEX_LOCK_DEFINE(15)
            _STDEX_LOCK_DEFINE(16)
            _STDEX_LOCK_DEFINE(17)
            _STDEX_LOCK_DEFINE(18)
            _STDEX_LOCK_DEFINE(19)
            _STDEX_LOCK_DEFINE(20)
            _STDEX_LOCK_DEFINE(21)
            _STDEX_LOCK_DEFINE(22)
            _STDEX_LOCK_DEFINE(23)
            _STDEX_LOCK_DEFINE(24)

#undef _STDEX_TRY_LOCK
#undef _STDEX_LOCK_HELPER
#undef _STDEX_LOCK_DEFINE
        }

        template <class _Lockbl1, class _Lockbl2>
        void _lock_impl(_Lockbl1 &_m1, _Lockbl2 &_m2)
        {
            const unsigned int _lock_count = 2;
            unsigned int _lock_first = 0;
            for (;;)
            {
                switch (_lock_first)
                {
                case 0:
                    _lock_first = detail::_lock_helper(_m1, _m2);
                    if (0 == _lock_first) return;
                    break;
                case 1:
                    _lock_first = detail::_lock_helper(_m2, _m1);
                    if (0 == _lock_first) return;
                    _lock_first = (_lock_first * 2) % _lock_count;
                    break;
                }
            }
        }

        template <class _It>
        void _lock_impl_iterators(_It begin, _It end);

        template <class _Lockbl1, class _Lockbl2>
        int _try_lock_impl(_Lockbl1 &_m1, _Lockbl2 &_m2)
        {
            return ((int) detail::_try_lock(_m1, _m2)) - 1;
        }

        template <class _It>
        _It _try_lock_impl_iterators(_It begin, _It end);
    } // namespace detail

    template <class _Lockbl1, class _Lockbl2>
    void lock(_Lockbl1 &_m1, _Lockbl2 &_m2)
    {
        detail::_lock_impl(_m1, _m2);
    }

    template <class _Lockbl1, class _Lockbl2>
    void lock(const _Lockbl1 &_m1, _Lockbl2 &_m2)
    {
        detail::_lock_impl(_m1, _m2);
    }

    template <class _Lockbl1, class _Lockbl2>
    void lock(_Lockbl1 &_m1, const _Lockbl2 &_m2)
    {
        detail::_lock_impl(_m1, _m2);
    }

    template <class _Lockbl1, class _Lockbl2>
    void lock(const _Lockbl1 &_m1, const _Lockbl2 &_m2)
    {
        detail::_lock_impl(_m1, _m2);
    }

    template <class _Lockbl1, _STDEX_TYPES3>
    void lock(_Lockbl1 &_m1, _STDEX_ARGS3)
    {
        const unsigned int _lock_count = 3;
        unsigned int _lock_first = 0;
        for (;;)
        {
            switch (_lock_first)
            {
            case 0:
                _lock_first = detail::_lock_helper(_m1, _m2, _m3);
                if (0 == _lock_first) return;
                break;
            case 1:
                _lock_first = detail::_lock_helper(_m2, _m3, _m1);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 2:
                _lock_first = detail::_lock_helper(_m3, _m1, _m2);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            }
        }
    }

    template <class _Lockbl1, _STDEX_TYPES4>
    void lock(_Lockbl1 &_m1, _STDEX_ARGS4)
    {
        const unsigned int _lock_count = 4;
        unsigned int _lock_first = 0;
        for (;;)
        {
            switch (_lock_first)
            {
            case 0:
                _lock_first = detail::_lock_helper(_m1, _m2, _m3, _m4);
                if (0 == _lock_first) return;
                break;
            case 1:
                _lock_first = detail::_lock_helper(_m2, _m3, _m4, _m1);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 2:
                _lock_first = detail::_lock_helper(_m3, _m4, _m1, _m2);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 3:
                _lock_first = detail::_lock_helper(_m4, _m1, _m2, _m3);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            }
        }
    }

    template <class _Lockbl1, _STDEX_TYPES5>
    void lock(_Lockbl1 &_m1, _STDEX_ARGS5)
    {
        const unsigned int _lock_count = 5;
        unsigned int _lock_first = 0;
        for (;;)
        {
            switch (_lock_first)
            {
            case 0:
                _lock_first = detail::_lock_helper(_m1, _m2, _m3, _m4, _m5);
                if (0 == _lock_first) return;
                break;
            case 1:
                _lock_first = detail::_lock_helper(_m2, _m3, _m4, _m5, _m1);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 2:
                _lock_first = detail::_lock_helper(_m3, _m4, _m5, _m1, _m2);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 3:
                _lock_first = detail::_lock_helper(_m4, _m5, _m1, _m2, _m3);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 4:
                _lock_first = detail::_lock_helper(_m5, _m1, _m2, _m3, _m4);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            }
        }
    }

    template <class _Lockbl1, _STDEX_TYPES6>
    void lock(_Lockbl1 &_m1, _STDEX_ARGS6)
    {
        const unsigned int _lock_count = 6;
        unsigned int _lock_first = 0;
        for (;;)
        {
            switch (_lock_first)
            {
            case 0:
                _lock_first = detail::_lock_helper(_m1, _m2, _m3, _m4, _m5, _m6);
                if (0 == _lock_first) return;
                break;
            case 1:
                _lock_first = detail::_lock_helper(_m2, _m3, _m4, _m5, _m6, _m1);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 2:
                _lock_first = detail::_lock_helper(_m3, _m4, _m5, _m6, _m1, _m2);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 3:
                _lock_first = detail::_lock_helper(_m4, _m5, _m6, _m1, _m2, _m3);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 4:
                _lock_first = detail::_lock_helper(_m5, _m6, _m1, _m2, _m3, _m4);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 5:
                _lock_first = detail::_lock_helper(_m6, _m1, _m2, _m3, _m4, _m5);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            }
        }
    }

    template <class _Lockbl1, _STDEX_TYPES7>
    void lock(_Lockbl1 &_m1, _STDEX_ARGS7)
    {
        const unsigned int _lock_count = 7;
        unsigned int _lock_first = 0;
        for (;;)
        {
            switch (_lock_first)
            {
            case 0:
                _lock_first = detail::_lock_helper(_m1, _m2, _m3, _m4, _m5, _m6, _m7);
                if (0 == _lock_first) return;
                break;
            case 1:
                _lock_first = detail::_lock_helper(_m2, _m3, _m4, _m5, _m6, _m7, _m1);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 2:
                _lock_first = detail::_lock_helper(_m3, _m4, _m5, _m6, _m7, _m1, _m2);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 3:
                _lock_first = detail::_lock_helper(_m4, _m5, _m6, _m7, _m1, _m2, _m3);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 4:
                _lock_first = detail::_lock_helper(_m5, _m6, _m7, _m1, _m2, _m3, _m4);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 5:
                _lock_first = detail::_lock_helper(_m6, _m7, _m1, _m2, _m3, _m4, _m5);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 6:
                _lock_first = detail::_lock_helper(_m7, _m1, _m2, _m3, _m4, _m5, _m6);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            }
        }
    }

    template <class _Lockbl1, _STDEX_TYPES8>
    void lock(_Lockbl1 &_m1, _STDEX_ARGS8)
    {
        const unsigned int _lock_count = 8;
        unsigned int _lock_first = 0;
        for (;;)
        {
            switch (_lock_first)
            {
            case 0:
                _lock_first = detail::_lock_helper(_m1, _m2, _m3, _m4, _m5, _m6, _m7, _m8);
                if (0 == _lock_first) return;
                break;
            case 1:
                _lock_first = detail::_lock_helper(_m2, _m3, _m4, _m5, _m6, _m7, _m8, _m1);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 2:
                _lock_first = detail::_lock_helper(_m3, _m4, _m5, _m6, _m7, _m8, _m1, _m2);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 3:
                _lock_first = detail::_lock_helper(_m4, _m5, _m6, _m7, _m8, _m1, _m2, _m3);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 4:
                _lock_first = detail::_lock_helper(_m5, _m6, _m7, _m8, _m1, _m2, _m3, _m4);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 5:
                _lock_first = detail::_lock_helper(_m6, _m7, _m8, _m1, _m2, _m3, _m4, _m5);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 6:
                _lock_first = detail::_lock_helper(_m7, _m8, _m1, _m2, _m3, _m4, _m5, _m6);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            case 7:
                _lock_first = detail::_lock_helper(_m8, _m1, _m2, _m3, _m4, _m5, _m6, _m7);
                if (0 == _lock_first) return;
                _lock_first = (_lock_first * 2) % _lock_count;
                break;
            }
        }
    }

    template <class _Lockbl1, class _Lockbl2>
    int try_lock(_Lockbl1 &_m1, _Lockbl2 &_m2)
    {
        return detail::_try_lock_impl(_m1, _m2);
    }

    template <class _Lockbl1, class _Lockbl2>
    int try_lock(const _Lockbl1 &_m1, _Lockbl2 &_m2)
    {
        return detail::_try_lock_impl(_m1, _m2);
    }

    template <class _Lockbl1, class _Lockbl2>
    int try_lock(_Lockbl1 &_m1, const _Lockbl2 &_m2)
    {
        return detail::_try_lock_impl(_m1, _m2);
    }

    template <class _Lockbl1, class _Lockbl2>
    int try_lock(const _Lockbl1 &_m1, const _Lockbl2 &_m2)
    {
        return detail::_try_lock_impl(_m1, _m2);
    }

    using detail::try_lock;

#undef _STDEX_TYPES_N
#undef _STDEX_ARGS_N
#undef _STDEX_ARGS_NAMES_N

#undef      _STDEX_TYPES3 
#undef       _STDEX_ARGS3 
#undef _STDEX_ARGS_NAMES3 
#undef      _STDEX_TYPES4 
#undef       _STDEX_ARGS4 
#undef _STDEX_ARGS_NAMES4 
#undef      _STDEX_TYPES5 
#undef       _STDEX_ARGS5 
#undef _STDEX_ARGS_NAMES5 
#undef      _STDEX_TYPES6 
#undef       _STDEX_ARGS6 
#undef _STDEX_ARGS_NAMES6 
#undef      _STDEX_TYPES7 
#undef       _STDEX_ARGS7 
#undef _STDEX_ARGS_NAMES7 
#undef      _STDEX_TYPES8 
#undef       _STDEX_ARGS8 
#undef _STDEX_ARGS_NAMES8 
#undef      _STDEX_TYPES9 
#undef       _STDEX_ARGS9 
#undef _STDEX_ARGS_NAMES9 
#undef      _STDEX_TYPES10
#undef       _STDEX_ARGS10
#undef _STDEX_ARGS_NAMES10
#undef      _STDEX_TYPES11
#undef       _STDEX_ARGS11
#undef _STDEX_ARGS_NAMES11
#undef      _STDEX_TYPES12
#undef       _STDEX_ARGS12
#undef _STDEX_ARGS_NAMES12
#undef      _STDEX_TYPES13
#undef       _STDEX_ARGS13
#undef _STDEX_ARGS_NAMES13
#undef      _STDEX_TYPES14
#undef       _STDEX_ARGS14
#undef _STDEX_ARGS_NAMES14
#undef      _STDEX_TYPES15
#undef       _STDEX_ARGS15
#undef _STDEX_ARGS_NAMES15
#undef      _STDEX_TYPES16
#undef       _STDEX_ARGS16
#undef _STDEX_ARGS_NAMES16
#undef      _STDEX_TYPES17
#undef       _STDEX_ARGS17
#undef _STDEX_ARGS_NAMES17
#undef      _STDEX_TYPES18
#undef       _STDEX_ARGS18
#undef _STDEX_ARGS_NAMES18
#undef      _STDEX_TYPES19
#undef       _STDEX_ARGS19
#undef _STDEX_ARGS_NAMES19
#undef      _STDEX_TYPES20
#undef       _STDEX_ARGS20
#undef _STDEX_ARGS_NAMES20
#undef      _STDEX_TYPES21
#undef       _STDEX_ARGS21
#undef _STDEX_ARGS_NAMES21
#undef      _STDEX_TYPES22
#undef       _STDEX_ARGS22
#undef _STDEX_ARGS_NAMES22
#undef      _STDEX_TYPES23
#undef       _STDEX_ARGS23
#undef _STDEX_ARGS_NAMES23
#undef      _STDEX_TYPES24
#undef       _STDEX_ARGS24
#undef _STDEX_ARGS_NAMES24
} // namespace stdex

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION
#undef _STDEX_NOEXCEPT


#endif // _STDEX_MUTEX_H
