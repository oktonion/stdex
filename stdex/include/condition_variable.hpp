#ifndef _STDEX_CONDITION_VARIABLE_H
#define _STDEX_CONDITION_VARIABLE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./mutex"
#include "./chrono"

// POSIX includes
#include <pthread.h>

// std includes
#include <memory>

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define DELETED_FUNCTION =delete
#define NOEXCEPT_FUNCTION noexcept

#else

#define DELETED_FUNCTION 
#define NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
	/// cv_status
	struct cv_status
	{
		enum cv_status_t { no_timeout, timeout };

		cv_status(cv_status_t cvstat) :
			_cvstat(cvstat)
		{

		}

		operator cv_status_t() const
		{
			return _cvstat;
		}

		private:
			cv_status_t _cvstat;
	};

	//! Condition variable class.
	//! This is a signaling object for synchronizing the execution flow for
	//! several threads. Example usage:
	//! @code
	//! // Shared data and associated mutex and condition variable objects
	//! int count;
	//! mutex m;
	//! condition_variable cond;
	//!
	//! // Wait for the counter to reach a certain number
	//! void wait_counter(int targetCount)
	//! {
	//!   lock_guard<mutex> guard(m);
	//!   while(count < targetCount)
	//!     cond.wait(m);
	//! }
	//!
	//! // Increment the counter, and notify waiting threads
	//! void increment()
	//! {
	//!   lock_guard<mutex> guard(m);
	//!   ++ count;
	//!   cond.notify_all();
	//! }
	//! @endcode
	class condition_variable 
	{
		typedef chrono::system_clock clock_t;

	public:
		typedef pthread_cond_t* native_handle_type;

		//! Constructor.
		condition_variable() NOEXCEPT_FUNCTION
		{
			pthread_cond_init(&_condition_handle, NULL);
		}

		//! Destructor.
		~condition_variable() NOEXCEPT_FUNCTION
		{
			pthread_cond_destroy(&_condition_handle);
		}

		//! Wait for the condition.
		//! The function will block the calling thread until the condition variable
		//! is woken by @c notify_one(), @c notify_all() or a spurious wake up.
		inline void wait(unique_lock<mutex> &lock) NOEXCEPT_FUNCTION
		{
			int _e = pthread_cond_wait(&_condition_handle, lock.mutex()->native_handle());

			if (_e)
				std::terminate();
		}

		template<class _Predicate>
		void wait(unique_lock<mutex>& lock, _Predicate p)
		{
			while (!p())
				wait(lock);
		}

		template<class _Duration>
		cv_status wait_until(unique_lock<mutex> &lock, const chrono::time_point<clock_t, _Duration> &atime)
		{
			return wait_until_impl(lock, atime);
		}

		template<class _Clock, class _Duration>
		cv_status wait_until(unique_lock<mutex> &lock, const chrono::time_point<_Clock, _Duration> &atime)
		{
			// DR 887 - Sync unknown clock to known clock.
			const typename _Clock::time_point c_entry = _Clock::now();
			const clock_t::time_point s_entry = clock_t::now();

			return wait_until_impl(lock, (s_entry + (atime - c_entry)));
		}

		template<class _Clock, class _Duration, class _Predicate>
		bool wait_until(unique_lock<mutex> &lock, const chrono::time_point<_Clock, _Duration> &atime, _Predicate p)
		{
			while (!p())
				if (wait_until(lock, atime) == cv_status::timeout)
					return p();
			return true;
		}

		template<class _Rep, class _Period>
		cv_status wait_for(unique_lock<mutex> &lock, const chrono::duration<_Rep, _Period> &rtime)
		{
			return wait_for_impl(lock, rtime);
		}

		template<class _Rep, class _Period, class _Predicate>
		bool wait_for(unique_lock<mutex> &lock, const chrono::duration<_Rep, _Period> &rtime, _Predicate p)
		{
			return wait_until(lock, clock_t::now() + chrono::duration_cast<clock_t::duration>(rtime), p);
		}

		native_handle_type native_handle()
		{
			return &_condition_handle;
		}

		//! Notify one thread that is waiting for the condition.
		//! If at least one thread is blocked waiting for this condition variable,
		//! one will be woken up.
		//! @note Only threads that started waiting prior to this call will be
		//! woken up.
		inline void notify_one() NOEXCEPT_FUNCTION
		{
			pthread_cond_signal(&_condition_handle);
		}

		//! Notify all threads that are waiting for the condition.
		//! All threads that are blocked waiting for this condition variable will
		//! be woken up.
		//! @note Only threads that started waiting prior to this call will be
		//! woken up.
		inline void notify_all() NOEXCEPT_FUNCTION
		{
			pthread_cond_broadcast(&_condition_handle);
		}

	private:

		template<class _Dur>
		cv_status wait_until_impl(unique_lock<mutex> &lock, const chrono::time_point<clock_t, _Dur> &atime)
		{
			if (!lock.owns_lock())
				std::terminate();

			chrono::time_point<clock_t, chrono::seconds> s = chrono::time_point_cast<chrono::seconds>(atime);
			chrono::nanoseconds ns = chrono::duration_cast<chrono::nanoseconds>(atime - s);

			timespec ts;
			ts.tv_sec = static_cast<stdex::time_t>(s.time_since_epoch().count());
			ts.tv_nsec = static_cast<long>(ns.count());

			int res = pthread_cond_timedwait(&_condition_handle, lock.mutex()->native_handle(), &ts);

			return (clock_t::now() < atime
				? cv_status::no_timeout : cv_status::timeout);
		}

		template<class _Rep, class _Period>
		cv_status wait_for_impl(unique_lock<mutex> &lock, const chrono::duration<_Rep, _Period> &rtime)
		{
			clock_t::time_point start_time_point = clock_t::now();

			if (!lock.owns_lock())
				std::terminate();

			chrono::seconds rs = chrono::duration_cast<chrono::seconds>(rtime);
			chrono::nanoseconds rns = chrono::duration_cast<chrono::nanoseconds>(rtime - rs);

			timespec ts;

			ts.tv_sec = static_cast<stdex::time_t>(rs.count());
			ts.tv_nsec = static_cast<long>(rns.count());

			chrono::time_point<clock_t, chrono::seconds> s = chrono::time_point_cast<chrono::seconds>(start_time_point);
			chrono::nanoseconds ns = chrono::duration_cast<chrono::nanoseconds>(start_time_point - s);

			ts.tv_sec += static_cast<stdex::time_t>(s.time_since_epoch().count());
			ts.tv_nsec += static_cast<long>(ns.count());

			int res = pthread_cond_timedwait(&_condition_handle, lock.mutex()->native_handle(), &ts);

			return (clock_t::now() - start_time_point < rtime
				? cv_status::no_timeout : cv_status::timeout);
		}

		pthread_cond_t _condition_handle;

		condition_variable(const condition_variable&) DELETED_FUNCTION;
		condition_variable& operator=(const condition_variable&) DELETED_FUNCTION;
	};

	void notify_all_at_thread_exit(condition_variable& cond, unique_lock<mutex> &lk); 
} // namespace stdex

#undef DELETED_FUNCTION
#undef NOEXCEPT_FUNCTION

#endif // _STDEX_CONDITION_VARIABLE_H
