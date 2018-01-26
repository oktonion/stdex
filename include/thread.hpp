#ifndef _STDEX_THREAD_H
#define _STDEX_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./mutex"
#include "./condition_variable"
#include "./chrono"

// POSIX includes
#include <pthread.h>

// std includes
#include <ostream>
#include <memory>

#ifdef _STDEX_HAS_CPP11_SUPPORT

#define DELETED_FUNCTION =delete
#define NOEXCEPT_FUNCTION throw() 

#else

#define DELETED_FUNCTION 
#define NOEXCEPT_FUNCTION

#endif

namespace stdex
{
	//! Thread class.
	class thread {

		template<class ClassT>
		struct classfunc
		{
			typedef void(ClassT::*function_type)(void);

			classfunc(ClassT *obj_, function_type func_) :obj(obj_), func(func_) {}

			ClassT *obj;
			function_type func;

			inline void operator()()
			{
				(obj->*func)();
			}

			static void proxy(void *obj)
			{
				classfunc &f = *reinterpret_cast<classfunc*>(obj);

				f();

				delete &f;
			}
		};

		template<class ClassT, class DataT>
		struct classfuncwithdata
		{
			typedef void(ClassT::*function_type)(DataT*);

			classfuncwithdata(ClassT *obj_, function_type func, DataT *data_) :obj(obj_), func(func_), data(data_) {}

			ClassT *obj;
			function_type func;
			DataT *data;

			inline void operator()()
			{
				(obj->*func)(data);
			}

			static void proxy(void *obj)
			{
				classfuncwithdata &f = *reinterpret_cast<classfuncwithdata*>(obj);

				f();

				delete &f;
			}
		};

		template<class ClassT, class DataT>
		struct classfuncwithdata<ClassT, DataT*>
		{
			typedef void(ClassT::*function_type)(DataT*);

			classfuncwithdata(ClassT *obj_, function_type func, DataT *data_) :obj(obj_), func(func_), data(data_) {}

			ClassT *obj;
			function_type func;
			DataT *data;

			static void proxy(void *obj)
			{
				classfuncwithdata *d = reinterpret_cast<classfuncwithdata*>(obj);

				d->(*func)(data);

				delete d;
			}
		};

		void init(void(*aFunction)(void *), void * aArg);

	public:
		typedef pthread_t native_handle_type;

		class id;

		//! Default constructor.
		//! Construct a @c thread object without an associated thread of execution
		//! (i.e. non-joinable).
		thread() NOEXCEPT_FUNCTION: 
			_not_a_thread(true)
		{
		}

		//! Thread starting constructor.
		//! Construct a @c thread object with a new thread of execution.
		//! @param[in] aFunction A function pointer to a function of type:
		//!          <tt>void fun(void * arg)</tt>
		//! @param[in] aArg Argument to the thread function.
		//! @note This constructor is not fully compatible with the standard C++
		//! thread class. It is more similar to the pthread_create() (POSIX) and
		//! CreateThread() (Windows) functions.
		thread(void(*aFunction)(void *), void * aArg);

		template<class ClassT>
		thread(void(ClassT::*aFunction)(void), ClassT *obj) :
			_not_a_thread(true)
		{
			init(&classfunc<ClassT>::proxy, new classfunc<ClassT>(obj, aFunction));
		}

		template<class ClassT, class DataT>
		thread(void(ClassT::*aFunction)(DataT*), ClassT *obj, DataT *aArg) :
			_not_a_thread(true)
		{
			init(&classfuncwithdata<ClassT, DataT*>::proxy, new classfuncwithdata<ClassT, DataT*>(obj, aFunction, aArg));
		}

		//! Destructor.
		//! @note If the thread is joinable upon destruction, @c std::terminate()
		//! will be called, which terminates the process. It is always wise to do
		//! @c join() before deleting a thread object.
		~thread();

		//! Wait for the thread to finish (join execution flows).
		//! After calling @c join(), the thread object is no longer associated with
		//! a thread of execution (i.e. it is not joinable, and you may not join
		//! with it nor detach from it).
		void join();

		//! Check if the thread is joinable.
		//! A thread object is joinable if it has an associated thread of execution.
		bool joinable() const NOEXCEPT_FUNCTION;

		//! Detach from the thread.
		//! After calling @c detach(), the thread object is no longer assicated with
		//! a thread of execution (i.e. it is not joinable). The thread continues
		//! execution without the calling thread blocking, and when the thread
		//! ends execution, any owned resources are released.
		void detach();

		//! Return the thread ID of a thread object.
		id get_id() const NOEXCEPT_FUNCTION;

		//! Get the native handle for this thread.
		//! @note Under Windows, this is a @c HANDLE, and under POSIX systems, this
		//! is a @c pthread_t.
		inline native_handle_type native_handle()
		{
			return _thread_handle;
		}

		//! Determine the number of threads which can possibly execute concurrently.
		//! This function is useful for determining the optimal number of threads to
		//! use for a task.
		//! @return The number of hardware thread contexts in the system.
		//! @note If this value is not defined, the function returns zero (0).
		static unsigned hardware_concurrency();

		void swap(thread &other) NOEXCEPT_FUNCTION;

	private:
		native_handle_type _thread_handle;   //!< Thread handle.
		mutable mutex _data_mutex;     //!< Serializer for access to the thread private data.
		bool _not_a_thread;			  //!< True if this object is not a thread of execution.

		struct thread_start_info;

		thread_start_info *_thread_info;

		//thread(thread&) DELETED_FUNCTION;
		thread(const thread&) DELETED_FUNCTION;
		thread& operator=(const thread&) DELETED_FUNCTION;

		// This is the internal thread wrapper function.
		static void* wrapper_function(void *aArg);
	};


	//! Thread ID.
	//! The thread ID is a unique identifier for each thread.
	//! @see thread::get_id()
	class thread::id {
	public:
		//! Default constructor.
		//! The default constructed ID is that of thread without a thread of
		//! execution.
		id() NOEXCEPT_FUNCTION :
			_id(0) 
		{}

		explicit id(unsigned long int aId) :
			_id(aId) 
		{}

		id(const id &aId) : 
			_id(aId._id) 
		{}

		inline id& operator=(const id &aId)
		{
			_id = aId._id;
			return *this;
		}

		inline friend bool operator==(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id == aId2._id);
		}

		inline friend bool operator!=(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id != aId2._id);
		}

		inline friend bool operator<=(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id <= aId2._id);
		}

		inline friend bool operator<(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id < aId2._id);
		}

		inline friend bool operator>=(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id >= aId2._id);
		}

		inline friend bool operator>(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
		{
			return (aId1._id > aId2._id);
		}

		template<class _CharT, class _Traits>
		inline friend ::std::basic_ostream<_CharT, _Traits>& operator<<(::std::basic_ostream<_CharT, _Traits> &out, thread::id id)
		{
			if (id == thread::id())
				return out << "thread::id of a non-executing thread";
			else
				return out << id._id;
		}

	private:
		unsigned long int _id;
	};

	inline void swap(thread &rhs, thread &lhs) NOEXCEPT_FUNCTION
	{
		rhs.swap(lhs);
	}

	namespace detail
	{
		void sleep_for_impl(const struct timespec *reltime);
	}

	//! The namespace @c this_thread provides methods for dealing with the
	//! calling thread.
	namespace this_thread 
	{
		//! Return the thread ID of the calling thread.
		thread::id get_id() NOEXCEPT_FUNCTION;

		//! Yield execution to another thread.
		//! Offers the operating system the opportunity to schedule another thread
		//! that is ready to run on the current processor.
		inline static void yield() NOEXCEPT_FUNCTION
		{
			sched_yield();
		}


		//! Blocks the calling thread for a period of time.
		//! @param[in] aTime Minimum time to put the thread to sleep.
		//! Example usage:
		//! @code
		//! // Sleep for 100 milliseconds
		//! this_thread::sleep_for(chrono::milliseconds(100));
		//! @endcode
		//! @note Supported duration types are: nanoseconds, microseconds,
		//! milliseconds, seconds, minutes and hours.

		template <class _Rep, class _Period>
		inline void sleep_for(const chrono::duration<_Rep, _Period> &d)
		{
			chrono::seconds s = chrono::duration_cast<chrono::seconds>(d);
			chrono::nanoseconds ns = chrono::duration_cast<chrono::nanoseconds>(d - s);
			
			timespec ts;
			ts.tv_sec = static_cast<stdex::time_t>(s.count());
			ts.tv_nsec = static_cast<long>(ns.count());

			detail::sleep_for_impl(&ts);
		}

		template <class _Clock, class _Duration>
		inline void sleep_until(const chrono::time_point<_Clock, _Duration> &t)
		{
			chrono::time_point<_Clock> now = _Clock::now();

			if (_Clock::is_steady)
			{
				if (now < t)
					sleep_for(t - now);
				return;
			}

			while (now < t)
			{
				sleep_for(t - now);
				now = _Clock::now();
			}

		}
	}
} // namespace stdex

#endif // _STDEX_THREAD_H