#ifndef _STDEX_MUTEX_H
#define _STDEX_MUTEX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./system_error"
#include "./chrono"

// POSIX includes
#include <pthread.h>

// std includes
#include <algorithm>

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define DELETED_FUNCTION =delete
#define NOEXCEPT_FUNCTION noexcept

#else

#define DELETED_FUNCTION 
#define NOEXCEPT_FUNCTION throw()

#endif


namespace stdex
{
	class mutex_base
	{
	protected:

		pthread_mutex_t  _mutex_handle;

		mutex_base() NOEXCEPT_FUNCTION
		{
			// XXX EAGAIN, ENOMEM, EPERM, EBUSY(may), EINVAL(may)
			pthread_mutex_init(&_mutex_handle, NULL);
		}

		~mutex_base() NOEXCEPT_FUNCTION { pthread_mutex_destroy(&_mutex_handle); }

	private:
		mutex_base(const mutex_base&) DELETED_FUNCTION;
		mutex_base& operator=(const mutex_base&) DELETED_FUNCTION;
	};

	//! Mutex class.
	//! This is a mutual exclusion object for synchronizing access to shared
	//! memory areas for several threads. The mutex is non-recursive (i.e. a
	//! program may deadlock if the thread that owns a mutex object calls lock()
	//! on that object).
	//! @see recursive_mutex
	class mutex:
		private mutex_base
	{
	public:
		typedef pthread_mutex_t* native_handle_type;

		//! Constructor.
		mutex() NOEXCEPT_FUNCTION
		{}


		//! Destructor.
		~mutex() NOEXCEPT_FUNCTION
		{}

		//! Lock the mutex.
		//! The method will block the calling thread until a lock on the mutex can
		//! be obtained. The mutex remains locked until @c unlock() is called.
		//! @see lock_guard
		//! @throws system_error
		inline void lock()
		{
			int e = pthread_mutex_lock(&_mutex_handle);

			// EINVAL, EAGAIN, EBUSY, EINVAL, EDEADLK(may)
			if (e)
				throw system_error( error_code(errc::errc_t(e)) );
		}

		//! Try to lock the mutex.
		//! The method will try to lock the mutex. If it fails, the function will
		//! return immediately (non-blocking).
		//! @return @c true if the lock was acquired, or @c false if the lock could
		//! not be acquired.
		inline bool try_lock() NOEXCEPT_FUNCTION
		{
			// XXX EINVAL, EAGAIN, EBUSY
			return (pthread_mutex_trylock(&_mutex_handle) == 0) ? true : false;
		}

		//! Unlock the mutex.
		//! If any threads are waiting for the lock on this mutex, one of them will
		//! be unblocked.
		inline void unlock()
		{
			pthread_mutex_unlock(&_mutex_handle);
		}

		native_handle_type native_handle() NOEXCEPT_FUNCTION
		{
			return &_mutex_handle;
		}

	private:
		mutex(const mutex&) DELETED_FUNCTION;
		mutex& operator=(const mutex&) DELETED_FUNCTION;

		//friend class condition_variable;
	};

	class recursive_mutex_base
	{
	protected:
		pthread_mutex_t _mutex_handle;

		recursive_mutex_base()
		{
			// XXX EAGAIN, ENOMEM, EPERM, EBUSY(may), EINVAL(may)
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&_mutex_handle, &attr);
		}

		~recursive_mutex_base()
		{
			pthread_mutex_destroy(&_mutex_handle);
		}

	private:
		recursive_mutex_base(const recursive_mutex_base&) DELETED_FUNCTION;
		recursive_mutex_base& operator=(const recursive_mutex_base&) DELETED_FUNCTION;
	};

	//! Recursive mutex class.
	//! This is a mutual exclusion object for synchronizing access to shared
	//! memory areas for several threads. The mutex is recursive (i.e. a thread
	//! may lock the mutex several times, as long as it unlocks the mutex the same
	//! number of times).
	//! @see mutex
	class recursive_mutex :
		private recursive_mutex_base
	{
	public:
		typedef pthread_mutex_t* native_handle_type;

		//! Constructor.
		recursive_mutex()
		{}

		//! Destructor.
		~recursive_mutex()
		{}

		//! Lock the mutex.
		//! The method will block the calling thread until a lock on the mutex can
		//! be obtained. The mutex remains locked until @c unlock() is called.
		//! @see lock_guard
		inline void lock()
		{
			int e = pthread_mutex_lock(&_mutex_handle);

			// EINVAL, EAGAIN, EBUSY, EINVAL, EDEADLK(may)
			if (e)
				throw system_error(error_code(errc::errc_t(e)));
		}

		//! Try to lock the mutex.
		//! The method will try to lock the mutex. If it fails, the function will
		//! return immediately (non-blocking).
		//! @return @c true if the lock was acquired, or @c false if the lock could
		//! not be acquired.
		inline bool try_lock()
		{
			// XXX EINVAL, EAGAIN, EBUSY
			return (pthread_mutex_trylock(&_mutex_handle) == 0) ? true : false;
		}

		//! Unlock the mutex.
		//! If any threads are waiting for the lock on this mutex, one of them will
		//! be unblocked.
		inline void unlock()
		{
			// XXX EINVAL, EAGAIN, EBUSY
			pthread_mutex_unlock(&_mutex_handle);
		}

		native_handle_type native_handle() NOEXCEPT_FUNCTION
		{
			return &_mutex_handle;
		}

	private:

		//friend class condition_variable;

		recursive_mutex(const recursive_mutex&) DELETED_FUNCTION;
		recursive_mutex& operator=(const recursive_mutex&) DELETED_FUNCTION;
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

	//! Lock guard class.
	//! The constructor locks the mutex, and the destructor unlocks the mutex, so
	//! the mutex will automatically be unlocked when the lock guard goes out of
	//! scope. Example usage:
	//! @code
	//! mutex m;
	//! int counter;
	//!
	//! void increment()
	//! {
	//!   lock_guard<mutex> guard(m);
	//!   ++ counter;
	//! }
	//! @endcode

	template <class T>
	class lock_guard {
	public:
		typedef T mutex_type;

		lock_guard() : _device(0) {}

		//! The constructor locks the mutex.
		explicit lock_guard(mutex_type &m):
			_device(m)
		{
			_device.lock();
		}

		//! The constructor does not lock the mutex because calling thread owns it
		lock_guard(mutex_type &m, adopt_lock_t) NOEXCEPT_FUNCTION:
			_device(m)
		{}

		//! The destructor unlocks the mutex.
		~lock_guard()
		{
			_device.unlock();
		}


	private:
		mutex_type &_device;

		lock_guard(const lock_guard&) DELETED_FUNCTION;
		lock_guard& operator=(const lock_guard&) DELETED_FUNCTION;
	};

	/** @brief A movable scoped lock type.
	*
	* A unique_lock controls mutex ownership within a scope. Ownership of the
	* mutex can be delayed until after construction and can be transferred
	* to another unique_lock by move construction or move assignment. If a
	* mutex lock is owned when the destructor runs ownership will be released.
	*/
	template<class T>
	class unique_lock
	{
	public:
		typedef T mutex_type;

		unique_lock() NOEXCEPT_FUNCTION : 
			_device(0), 
			_owns(false)
		{ }

		explicit unique_lock(mutex_type &m): 
			_device(&(m)),
			_owns(false)
		{
			lock();
			_owns = true;
		}

		unique_lock(mutex_type &m, defer_lock_t) NOEXCEPT_FUNCTION: 
			_device(&(m)), 
			_owns(false)
		{ }

		unique_lock(mutex_type &m, try_to_lock_t): 
			_device(&(m)), 
			_owns(_device->try_lock())
		{ }

		unique_lock(mutex_type &m, adopt_lock_t) NOEXCEPT_FUNCTION: 
			_device(&(m)), 
			_owns(true)
		{
			// XXX calling thread owns mutex
		}

		template<class _Clock, class _Duration>
		unique_lock(mutex_type &m, const chrono::time_point<_Clock, _Duration> &atime): 
			_device(&(m)),
			_owns(_device->try_lock_until(atime))
		{ }

		template<class _Rep, class _Period>
		unique_lock(mutex_type &m, const chrono::duration<_Rep, _Period> &rtime):
			_device(&(m)),
			_owns(_device->try_lock_for(rtime))
		{ }

		~unique_lock()
		{
			if (_owns)
				unlock();
		}

		/* move move move, no move in pre-C++11
		unique_lock(unique_lock &&u) NOEXCEPT_FUNCTION: 
			_device(u._device), 
			_owns(u._owns)
		{
			u._device = 0;
			u._owns = false;
		}

		unique_lock& operator=(unique_lock &&u) NOEXCEPT_FUNCTION
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

		void swap(unique_lock &other) NOEXCEPT_FUNCTION
		{
			{
				mutex_type *tmp = _device;
				_device = other._device;
				other._device = tmp;
			}

			{
				bool tmp = _owns;
				_owns = other._owns;
				other._owns = tmp;
			}
		}

		mutex_type* release() NOEXCEPT_FUNCTION
		{
			mutex_type *ret = _device;
			_device = 0;
			_owns = false;
			return ret;
		}

		bool owns_lock() const NOEXCEPT_FUNCTION
		{
			return _owns;
		}

		operator bool() const NOEXCEPT_FUNCTION
		{
			return owns_lock();
		}

		mutex_type* mutex() const NOEXCEPT_FUNCTION
		{
			return _device;
		}

	private:
		mutex_type *_device;
		bool _owns; // XXX use atomic_bool

		unique_lock(const unique_lock&) DELETED_FUNCTION;
		unique_lock& operator=(const unique_lock&) DELETED_FUNCTION;
	};

	/// Swap overload for unique_lock objects.
	template<class _Mutex>
	inline void swap(stdex::unique_lock<_Mutex> &lhs, stdex::unique_lock<_Mutex> &rhs) NOEXCEPT_FUNCTION
	{
		lhs.swap(rhs);
	}

} // namespace stdex

#undef DELETED_FUNCTION
#undef NOEXCEPT_FUNCTION


#endif // _STDEX_MUTEX_H
