#ifndef _STDEX_THREAD_H
#define _STDEX_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./chrono.hpp"
#include "./condition_variable.hpp"
#include "./type_traits.hpp"
#include "./functional.hpp"

#define _STDEX_PARAMETER_PACK_DEFINE
#include "./parameter_pack.h"
#undef _STDEX_PARAMETER_PACK_DEFINE

// POSIX includes
#include <pthread.h>

// std includes
#include <ostream>
#include <memory>
#include <algorithm> // std::swap
#include <utility> // std::swap
#include <limits>

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
        template<class _Tp>
        struct _thread_param_wrapper
        {
            template<class _ConvT>
            _thread_param_wrapper(_ConvT arg)
            {
                STATIC_ASSERT((sizeof(_ConvT) <= max_type_size::value), stdex_thread_param_type_is_not_supported);

                _convert_func = &_converter<_ConvT>;
                _delete_func = &_deleter<_ConvT>;
                _ConvT *copy = new _ConvT(arg);
                std::memcpy(&_pointer_buf[0], &copy, sizeof(copy));
            }

            _thread_param_wrapper(const _thread_param_wrapper& other)
            {
                _convert_func = other._convert_func;
                _delete_func = other._delete_func;
                std::memcpy(&_pointer_buf[0], &other._pointer_buf[0], sizeof(_pointer_buf));
                std::memset(&other._pointer_buf[0], 0, sizeof(_pointer_buf));
            }

            operator _Tp() const
            {
                return _convert_func(_pointer_buf);
            }

            ~_thread_param_wrapper()
            {
                _delete_func(_pointer_buf);
            }

        private:
            typedef 
            integral_constant<
                int, 
                sizeof(void*) * 4> 
            max_type_size;

            typedef unsigned char (&pointer_buf_ref)[max_type_size::value];
            typedef _Tp (*conversion_func)(pointer_buf_ref);
            typedef void (*deleter_func)(pointer_buf_ref);

            template<class _ConvT>
            static _Tp _converter(pointer_buf_ref _ptr_buf)
            {
                _ConvT *ptr_ = 0;
                std::memcpy(&ptr_, &_ptr_buf[0], sizeof(ptr_));

                return *ptr_;
            }

            template<class _ConvT>
            static void _deleter(pointer_buf_ref _ptr_buf)
            {
                _ConvT *ptr_ = 0;
                std::memcpy(&ptr_, &_ptr_buf[0], sizeof(ptr_));

                delete ptr_;
            }
            
            conversion_func _convert_func;
            deleter_func _delete_func;
            mutable unsigned char _pointer_buf[max_type_size::value];
        };
    }

    //! Thread class.
    class thread {

        typedef void(*caller_function_type)(void *);
        struct init_args
        {
            caller_function_type caller_function;
            void *bindable_ptr;
        };
        
        void init(init_args);

        template<class _BindableT>
        static init_args call_bindable(const _BindableT& bindable)
        {
            struct lambdas
            {
                static void call(void *bindable_void)
                {
                    _BindableT &bindable = 
                        *reinterpret_cast<_BindableT*>(bindable_void);

                    bindable();

                    delete &bindable;
                }
            };

            init_args result;
            result.caller_function = &lambdas::call;
            result.bindable_ptr = new _BindableT(bindable);

            return result;
        }

    public:
        typedef pthread_t native_handle_type;

        class id {
        public:
            
            id() _STDEX_NOEXCEPT_FUNCTION :
                _uid(invalid_id)
            { }

            id(const id &aId) :
                _uid(aId._uid)
            { }

            inline id& operator=(const id &aId)
            {
                _uid = aId._uid;

                return *this;
            }

            bool operator==(const id &other) const _STDEX_NOEXCEPT_FUNCTION
            {
                return _uid == other._uid;
            }

            inline friend bool operator!=(const id &aId1, const id &aId2) _STDEX_NOEXCEPT_FUNCTION
            {
                return !(aId1 == aId2);
            }

            inline friend bool operator<=(const id &aId1, const id &aId2) _STDEX_NOEXCEPT_FUNCTION
            {
                return aId1._uid <= aId2._uid;
            }

            bool operator<(const id &other) const _STDEX_NOEXCEPT_FUNCTION
            {
                return _uid < other._uid;
            }

            inline friend bool operator>=(const id &aId1, const id &aId2) _STDEX_NOEXCEPT_FUNCTION
            {
                return aId1._uid >= aId2._uid;
            }

            inline friend bool operator>(const id &aId1, const id &aId2) _STDEX_NOEXCEPT_FUNCTION
            {
                return aId1._uid > aId2._uid;
            }

            ::std::ostream& print(::std::ostream &out) const
            {
                if (*this == id())
                    return out << "stdex::thread::id of a non-executing thread";
                else
                    return out << _uid;
            }

        protected:
            typedef stdex::uintmax_t id_type;
            static const id_type invalid_id =
                id_type(-1);

            explicit id(const id_type& uid) :
                _uid(uid)
            { }

        private:
            friend class thread;
            id_type _uid;
        };

        //! Default constructor.
        //! Construct a @c thread object without an associated thread of execution
        //! (i.e. non-joinable).
        thread() _STDEX_NOEXCEPT_FUNCTION
            : _handle() 
        { }

        template<class _FuncT>
        explicit thread(_FuncT _func)
            : _handle() 
        { init(call_bindable(stdex::bind(_func))); }

        template<class _R, class _ObjectT>
        explicit thread(_R(stdex::remove_pointer<typename stdex::remove_reference<_ObjectT>::type>::type::*_func)(), _ObjectT _obj)
            : _handle() 
        { init(call_bindable(stdex::bind(_func, _obj))); }

        template<class _R, class _ObjectT>
        explicit thread(_R(stdex::remove_pointer<typename stdex::remove_reference<_ObjectT>::type>::type::*_func)() const, _ObjectT _obj)
            : _handle() 
        { init(call_bindable(stdex::bind(_func, _obj))); }

#define _STDEX_THREAD_CONSTRUCTOR(count) \
        template<class _FuncT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)> \
        thread(_FuncT _func, _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK) \
                , typename stdex::enable_if< \
                    intern::_has_feature<intern::_stdex_nullptr_implemented_as_distinct_type>::value == bool(true) || \
                    stdex::is_union<_FuncT>::value == bool(true) || \
                    stdex::is_class<_FuncT>::value == bool(true)>::type* = 0) \
            : _handle() \
        { init(call_bindable(stdex::bind(_func, _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)))); } \
\
        template<class _R, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)> \
        explicit thread(_R(*_func)( _STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) ) \
            , _STDEX_PARAMS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_CUSTOM, _STDEX_ARG_DEFAULT)) \
            : _handle() \
        { init(call_bindable(stdex::bind(_func, _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)))); } \
\
        template<class _R, class _ObjectT, class _ArgObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)> \
        explicit thread(_R(_ObjectT::*_func)( _STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) ) \
            , _ArgObjectT _obj \
            , _STDEX_PARAMS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_CUSTOM, _STDEX_ARG_DEFAULT)) \
            : _handle() \
        { init(call_bindable(stdex::bind(_func, _obj, _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)))); } \
\
        template<class _R, class _ObjectT, class _ArgObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)> \
        explicit thread(_R(_ObjectT::*_func)( _STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) ) const \
            , _ArgObjectT _obj \
            , _STDEX_PARAMS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_CUSTOM, _STDEX_ARG_DEFAULT)) \
            : _handle() \
        { init(call_bindable(stdex::bind(_func, _obj, _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)))); }

#define _STDEX_TYPE_CUSTOM(count) \
    typename \
    stdex::conditional< \
        stdex::is_pointer<_STDEX_TYPE_DEFAULT(count)>::value == bool(true) || \
        stdex::is_member_function_pointer<_STDEX_TYPE_DEFAULT(count)>::value == bool(true), \
        _STDEX_TYPE_DEFAULT(count), stdex::detail::_thread_param_wrapper<_STDEX_TYPE_DEFAULT(count)> >::type

#if (STDEX_FUNCTION_MAX_ARG_N >= 0)
        _STDEX_THREAD_CONSTRUCTOR(0 )
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 1)
        _STDEX_THREAD_CONSTRUCTOR(1 )
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 2)
        _STDEX_THREAD_CONSTRUCTOR(2 )
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 3)
        _STDEX_THREAD_CONSTRUCTOR(3 )
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 4)
        _STDEX_THREAD_CONSTRUCTOR(4 )
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 5)
        _STDEX_THREAD_CONSTRUCTOR(5 )
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 6)
        _STDEX_THREAD_CONSTRUCTOR(6 )
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 7)
        _STDEX_THREAD_CONSTRUCTOR(7 )
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 8)
        _STDEX_THREAD_CONSTRUCTOR(8 )
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 9)
        _STDEX_THREAD_CONSTRUCTOR(9 )
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 10)
        _STDEX_THREAD_CONSTRUCTOR(10)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 11)
        _STDEX_THREAD_CONSTRUCTOR(11)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 12)
        _STDEX_THREAD_CONSTRUCTOR(12)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 13)
        _STDEX_THREAD_CONSTRUCTOR(13)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 14)
        _STDEX_THREAD_CONSTRUCTOR(14)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 15)
        _STDEX_THREAD_CONSTRUCTOR(15)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 16)
        _STDEX_THREAD_CONSTRUCTOR(16)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 17)
        _STDEX_THREAD_CONSTRUCTOR(17)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 18)
        _STDEX_THREAD_CONSTRUCTOR(18)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 19)
        _STDEX_THREAD_CONSTRUCTOR(19)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 20)
        _STDEX_THREAD_CONSTRUCTOR(20)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 21)
        _STDEX_THREAD_CONSTRUCTOR(21)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 22)
        _STDEX_THREAD_CONSTRUCTOR(22)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 23)
        _STDEX_THREAD_CONSTRUCTOR(23)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 24)
        _STDEX_THREAD_CONSTRUCTOR(24)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 25)
        _STDEX_THREAD_CONSTRUCTOR(25)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 26)
        _STDEX_THREAD_CONSTRUCTOR(26)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 27)
        _STDEX_THREAD_CONSTRUCTOR(27)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 28)
        _STDEX_THREAD_CONSTRUCTOR(28)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 29)
        _STDEX_THREAD_CONSTRUCTOR(29)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 30)
        _STDEX_THREAD_CONSTRUCTOR(30)
#endif
#if (STDEX_FUNCTION_MAX_ARG_N >= 31)
        _STDEX_THREAD_CONSTRUCTOR(31)
#endif

#undef _STDEX_TYPE_CUSTOM


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
        bool joinable() const _STDEX_NOEXCEPT_FUNCTION;

        //! Detach from the thread.
        //! After calling @c detach(), the thread object is no longer associated with
        //! a thread of execution (i.e. it is not joinable). The thread continues
        //! execution without the calling thread blocking, and when the thread
        //! ends execution, any owned resources are released.
        void detach();

        //! Return the thread ID of a thread object.
        id get_id() const _STDEX_NOEXCEPT_FUNCTION;

        //! Get the native handle for this thread.
        inline native_handle_type native_handle()
        {
            return _handle;
        }

        //! Determine the number of threads which can possibly execute concurrently.
        //! This function is useful for determining the optimal number of threads to
        //! use for a task.
        //! @return The number of hardware thread contexts in the system.
        //! @note If this value is not defined, the function returns zero.
        static unsigned hardware_concurrency() _STDEX_NOEXCEPT_FUNCTION;

        void swap(thread &other) _STDEX_NOEXCEPT_FUNCTION;

    private:

        id _id;
        native_handle_type _handle;

        //thread(thread&) _STDEX_DELETED_FUNCTION;
        thread(const thread&) _STDEX_DELETED_FUNCTION;
        thread& operator=(const thread&) _STDEX_DELETED_FUNCTION;

        // This is the internal thread wrapper function.
        static void* wrapper_function(void *aArg);
    };

    namespace detail
    {
        void sleep_for_impl(const stdex::timespec *reltime);
    }

    //! The namespace @c this_thread provides methods for dealing with the
    //! calling thread.
    namespace this_thread 
    {
        //! Return the thread ID of the calling thread.
        thread::id get_id() _STDEX_NOEXCEPT_FUNCTION;

        //! Yield execution to another thread.
        //! Offers the operating system the opportunity to schedule another thread
        //! that is ready to run on the current processor.
        inline static void yield() _STDEX_NOEXCEPT_FUNCTION
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
        inline void sleep_for(const chrono::duration<_Rep, _Period> &_dur)
        {
            chrono::seconds _sec = chrono::duration_cast<chrono::seconds>(_dur);
            chrono::nanoseconds _ns = chrono::duration_cast<chrono::nanoseconds>(_dur - _sec);
            
            if(_sec.count() < 0 || _ns.count() < 0)
                return;
            if(_sec.count() == 0 && _ns.count() == 0)
                return;

            chrono::seconds::rep _s_count = _sec.count();

            stdex::timespec _ts;

            const stdex::time_t _ts_sec_max = 
                (std::numeric_limits<stdex::time_t>::max)();
            if (_s_count < _ts_sec_max)
            {
                _ts.tv_sec = static_cast<stdex::time_t>(_s_count > 0 ? _s_count : 0);
                _ts.tv_nsec = static_cast<long>(_ns.count());
            }
            else
            {
                _ts.tv_sec = _ts_sec_max;
                _ts.tv_nsec = 999999999;
            }

            detail::sleep_for_impl(&_ts);
        }

        template <class _Clock, class _Duration>
        inline void sleep_until(const chrono::time_point<_Clock, _Duration> &_tp)
        {
            chrono::time_point<_Clock> _now = _Clock::now();

            if (_Clock::is_steady)
            {
                if (_now < _tp)
                    sleep_for(_tp - _now);
                return;
            }

            while (_now < _tp)
            {
                sleep_for(_tp - _now);
                _now = _Clock::now();
            }

        }
    }
} // namespace stdex



namespace
{
    inline ::std::ostream& operator<<(::std::ostream &out, const stdex::thread::id &id)
    {
        return id.print(out);
    }
}

namespace std
{
    //template<>
    inline void swap(stdex::thread &lhs, stdex::thread &rhs)
    {
        lhs.swap(rhs);
    }
} // namespace std

namespace stdex
{
    using std::swap;
} // namespace stdex


#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#define _STDEX_PARAMETER_PACK_UNDEF
#include "./parameter_pack.h"
#undef _STDEX_PARAMETER_PACK_UNDEF

#endif // _STDEX_THREAD_H
