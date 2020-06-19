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
 
#else
 
#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()
 
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
                 pthread_mutex_init(&_mutex_handle, NULL);
             }

             ~_mutex_base() 
             { 
                 int _err = pthread_mutex_destroy(&_mutex_handle);
                 if (0 != _err)
                     throw(stdex::system_error( stdex::errc::errc_t(_err)) );
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
                     throw(stdex::system_error( stdex::errc::errc_t(_err)) );
             }

             ~_recursive_mutex_base() 
             {
                 int _err = pthread_mutex_destroy(&_mutex_handle);
                 if (0 != _err)
                     throw(stdex::system_error( stdex::errc::errc_t(_err)) );
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
             if (_err)
                 throw system_error( error_code(errc::errc_t(_err)) );
         }

         inline bool try_lock() _STDEX_NOEXCEPT_FUNCTION
         {
             // XXX EINVAL, EAGAIN, EBUSY
             return (pthread_mutex_trylock(&_mutex_handle) == 0) ? true : false;
         }
 
         inline void unlock()
         {
             int _err = pthread_mutex_unlock(&_mutex_handle);

             if (_err)
                throw system_error( error_code(errc::errc_t(_err)) );
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
             if (_err)
                 throw system_error(error_code(errc::errc_t(_err)));
         }

         inline bool try_lock()
         {
             // XXX EINVAL, EAGAIN, EBUSY
             return (pthread_mutex_trylock(&_mutex_handle) == 0) ? true : false;
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
 
     template <class _Tp>
     class lock_guard {
     public:
         typedef _Tp mutex_type;

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
 
     template<class _Tp>
     class unique_lock
     {
     public:
         typedef _Tp mutex_type;
 
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
                 throw system_error(error_code(errc::operation_not_permitted));
             else if (_owns)
                 throw system_error(error_code(errc::resource_deadlock_would_occur));
             else
             {
                 _device->lock();
                 _owns = true;
             }
         }
 
         bool try_lock()
         {
             if (!_device)
                 throw system_error(error_code(errc::operation_not_permitted));
             else if (_owns)
                 throw system_error(error_code(errc::resource_deadlock_would_occur));
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
                 throw system_error(error_code(errc::operation_not_permitted));
             else if (_owns)
                 throw system_error(error_code(errc::resource_deadlock_would_occur));
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
                 throw system_error(error_code(errc::operation_not_permitted));
             else if (_owns)
                 throw system_error(error_code(errc::resource_deadlock_would_occur));
             else
             {
                 _owns = _device->try_lock_for(rtime);
                 return _owns;
             }
         }
 
         void unlock()
         {
             if (!_owns)
                 throw system_error(error_code(errc::operation_not_permitted));
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
         void* pthread_mutex_timedlock(...); // dummy

         namespace mutex_type_traits
         {
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
                                 declval<pthread_mutex_t*>(), 
                                 declval<struct timespec*>()
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

                 if (ratio_greater<_clock::period, _Period>::value)
                     ++_rt;
                 return try_lock_until(_mutex_handle, _clock::now() + _rt);
             }

             template<class _Duration>
             static bool try_lock_until(pthread_mutex_t& _mutex_handle,
                 const chrono::time_point<chrono::system_clock, _Duration>& _atime)
             {
                 chrono::time_point<chrono::system_clock, chrono::seconds> _s =
                     chrono::time_point_cast<chrono::seconds>(_atime);
                 chrono::nanoseconds _ns = 
                     chrono::duration_cast<chrono::nanoseconds>(_atime - _s);

                 timespec _ts;
                 _ts.tv_sec = 
                     static_cast<stdex::time_t>(_s.time_since_epoch().count());
                 _ts.tv_nsec = 
                     static_cast<long>(_ns.count());

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

         template<class _Tp, bool>
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
             _timed_mutex_impl_base() _STDEX_NOEXCEPT_FUNCTION:
                 _locked(false)
             {}

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


         private:

             mutex		_mut;
             condition_variable	_cv;
             bool		_locked;

             _timed_mutex_impl_base(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
             _timed_mutex_impl_base& operator=(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
         };

         template<>
         class _timed_mutex_impl_base<_recursive_mutex_base, false>
         {
             // Predicate type that tests whether the current thread can lock a mutex.
             struct _Can_lock
             {
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
             _timed_mutex_impl_base() _STDEX_NOEXCEPT_FUNCTION :
                 _count(0)
             {}

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
                     throw system_error(error_code(errc::errc_t(EAGAIN)));// [thread.timedmutex.recursive]/3
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

         private:

             mutex		_mut;
             condition_variable	_cv;
             thread::id		_owner;
             unsigned		_count;

             _timed_mutex_impl_base(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
             _timed_mutex_impl_base& operator=(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
         };

         // with pthread_mutex_timedlock support
         template<>
         class _timed_mutex_impl_base<_mutex_base, true> :
             private _mutex_base
         {
             _timed_mutex_impl_base() _STDEX_NOEXCEPT_FUNCTION
             {}

             ~_timed_mutex_impl_base()
             {}

             inline void lock()
             {
                 int _err = pthread_mutex_lock(&_mutex_handle);

                 // EINVAL, EAGAIN, EBUSY, EINVAL, EDEADLK(may)
                 if (_err)
                     throw system_error(error_code(errc::errc_t(_err)));
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

         private:

             _timed_mutex_impl_base(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
             _timed_mutex_impl_base& operator=(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
         };

         template<>
         class _timed_mutex_impl_base<_recursive_mutex_base, true> :
             private _recursive_mutex_base
         {
         public:
             _timed_mutex_impl_base()
             {}

             ~_timed_mutex_impl_base()
             {}

             inline void lock()
             {
                 int _err = pthread_mutex_lock(&_mutex_handle);

                 // EINVAL, EAGAIN, EBUSY, EINVAL, EDEADLK(may)
                 if (_err)
                     throw system_error(error_code(errc::errc_t(_err)));
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

         private:

             _timed_mutex_impl_base(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
             _timed_mutex_impl_base& operator=(const _timed_mutex_impl_base&) _STDEX_DELETED_FUNCTION;
         };

         template<class>
         class _timed_mutex_impl;

         template<>
         class _timed_mutex_impl<timed_mutex> :
             private _timed_mutex_impl_base<
                _mutex_base,
                mutex_type_traits::_has_pthread_mutex_timedlock::value
             >
         { };

         template<>
         class _timed_mutex_impl<recursive_timed_mutex> :
             private _timed_mutex_impl_base<
                _recursive_mutex_base,
                mutex_type_traits::_has_pthread_mutex_timedlock::value
             >
         { };
     }

     class timed_mutex :
         private detail::_timed_mutex_impl<timed_mutex>
     {
     private:

         timed_mutex(const timed_mutex&) _STDEX_DELETED_FUNCTION;
         timed_mutex& operator=(const timed_mutex&) _STDEX_DELETED_FUNCTION;
     };

     class recursive_timed_mutex :
         private detail::_timed_mutex_impl<recursive_timed_mutex>
     {
     private:

         recursive_timed_mutex(const recursive_timed_mutex&) _STDEX_DELETED_FUNCTION;
         recursive_timed_mutex& operator=(const recursive_timed_mutex&) _STDEX_DELETED_FUNCTION;
     };
 
     /// Swap overload for unique_lock objects.
     template<class _Mutex>
     inline void swap(stdex::unique_lock<_Mutex> &lhs, stdex::unique_lock<_Mutex> &rhs) _STDEX_NOEXCEPT_FUNCTION
     {
         lhs.swap(rhs);
     }

     namespace detail
     {
         pthread_mutex_t* _lock_mutex_native_handle(const unique_lock<mutex>&);
         bool _lock_owns_lock(const unique_lock<mutex>&);
     } // namespace detail
 
 } // namespace stdex
 
 #undef _STDEX_DELETED_FUNCTION
 #undef _STDEX_NOEXCEPT_FUNCTION
 
 
 #endif // _STDEX_MUTEX_H
