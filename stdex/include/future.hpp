#ifndef _STDEX_FUTURE_H
#define _STDEX_FUTURE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./core.h"
#include "./condition_variable.hpp"
#include "./mutex.hpp"
#include "./type_traits.hpp"
#include "./system_error.hpp"
#include "./cstdint.hpp"
#include "./chrono.hpp"


// POSIX includes
/*none*/

// std includes
#include <exception>
#include <stdexcept>

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
        namespace future_detail
        {
            struct _nil_type {}; // empty struct, for unused argument types
            template<class _Tp>
            struct _rv_reference {};

            template <class _Tp>
            inline _Tp* addressof(_Tp& v)
            {
                return reinterpret_cast<_Tp*>(
                    &const_cast<char&>(reinterpret_cast<const volatile char&>(v)));
            }
        } // namespace future_detail
    } // namespace detail
    struct future_errc
    {
        typedef
        enum future_errc_t { // names for futures errors
            broken_promise = 1,
            future_already_retrieved,
            promise_already_satisfied,
            no_state
        } type;

        future_errc(type code):
            _code(code)
        { }

        operator type&()
        {
            return _code;
        }

        operator const type&() const
        {
            return _code;
        }

        private:
            type _code;
    };

    struct launch
    {
        typedef
        enum launch_t { // names for launch options passed to async
            async    = 0x1,
            deferred = 0x2
        } type;

        launch(type code):
            _code(code)
        { }

        operator type&()
        {
            return _code;
        }

        operator const type&() const
        {
            return _code;
        }

        private:
            type _code;
    };

    inline launch::type& operator&=(launch::type& _lhs, launch::type _rhs) { _lhs = (launch::type)((int)_lhs & (int)_rhs); return (_lhs); } 
    inline launch::type& operator|=(launch::type& _lhs, launch::type _rhs) { _lhs = (launch::type)((int)_lhs | (int)_rhs); return (_lhs); } 
    inline launch::type& operator^=(launch::type& _lhs, launch::type _rhs) { _lhs = (launch::type)((int)_lhs ^ (int)_rhs); return (_lhs); } 
    inline launch::type operator&(launch::type _lhs, launch::type _rhs) { return ((launch::type)((int)_lhs & (int)_rhs)); } 
    inline launch::type operator|(launch::type _lhs, launch::type _rhs) { return ((launch::type)((int)_lhs | (int)_rhs)); } 
    inline launch::type operator^(launch::type _lhs, launch::type _rhs) { return ((launch::type)((int)_lhs ^ (int)_rhs)); } 
    inline launch::type operator~(launch::type _lhs) { return ((launch::type)~(int)_lhs); }

    inline launch& operator&=(launch& _lhs, launch _rhs) { launch::type &_lhs_ref = _lhs, _rhs_val = _rhs; operator&=(_lhs_ref, _rhs_val); return (_lhs);} 
    inline launch& operator|=(launch& _lhs, launch _rhs) { launch::type &_lhs_ref = _lhs, _rhs_val = _rhs; operator|=(_lhs_ref, _rhs_val); return (_lhs);} 
    inline launch& operator^=(launch& _lhs, launch _rhs) { launch::type &_lhs_ref = _lhs, _rhs_val = _rhs; operator^=(_lhs_ref, _rhs_val); return (_lhs);} 
    inline launch operator&(launch _lhs, launch _rhs) { const launch::type &_lhs_ref = _lhs, &_rhs_ref = _rhs; return operator&(_lhs_ref, _rhs_ref); } 
    inline launch operator|(launch _lhs, launch _rhs) { const launch::type &_lhs_ref = _lhs, &_rhs_ref = _rhs; return operator|(_lhs_ref, _rhs_ref); } 
    inline launch operator^(launch _lhs, launch _rhs) { const launch::type &_lhs_ref = _lhs, &_rhs_ref = _rhs; return operator^(_lhs_ref, _rhs_ref); } 
    inline launch operator~(launch _lhs) { const launch::type &_lhs_ref = _lhs; return operator~(_lhs_ref); }

    struct future_status
    {
        typedef
        enum future_status_t { // names for timed wait function returns
            ready,
            timeout,
            deferred
        } type;

        future_status(type code):
            _code(code)
        { }

        operator type&()
        {
            return _code;
        }

        operator const type&() const
        {
            return _code;
        }

        private:
            type _code;
    };

    class _nullable_exception
    {
    public:
        _nullable_exception() _STDEX_NOEXCEPT_FUNCTION
            : valid(false)
        { }

        _nullable_exception(nullptr_t) _STDEX_NOEXCEPT_FUNCTION
            : valid(false)
        { }

        _nullable_exception(const std::exception &exp) _STDEX_NOEXCEPT_FUNCTION
            : value(exp)
            , valid(true)
        { }

        _nullable_exception& operator=(nullptr_t) _STDEX_NOEXCEPT_FUNCTION
        { 
            return *this;
        }

        operator bool() const _STDEX_NOEXCEPT_FUNCTION
        {
            return valid;
        }

        const std::exception &operator*() const _STDEX_NOEXCEPT_FUNCTION
        {
            return value;
        }

        bool operator==(const _nullable_exception& other) const _STDEX_NOEXCEPT_FUNCTION
        {
            if(!other.valid && !valid)
                return true;
            return value.what() == other.value.what();
        }

    private:
        std::exception value;
        bool valid;
    };

    template <>
    struct is_error_code_enum<future_errc> : true_type {};

    const error_category& future_category() _STDEX_NOEXCEPT_FUNCTION;

    inline error_code make_error_code(future_errc _Errno) _STDEX_NOEXCEPT_FUNCTION 
    {
        return error_code(static_cast<int>(_Errno), ::stdex::future_category());
    }

    inline error_condition make_error_condition(future_errc _Errno) _STDEX_NOEXCEPT_FUNCTION 
    {
        return error_condition(static_cast<int>(_Errno), ::stdex::future_category());
    }

    inline const char* _future_error_map(int _Errcode) _STDEX_NOEXCEPT_FUNCTION 
    { // convert to name of future error
        switch (static_cast<future_errc::type>(_Errcode)) 
        { // switch on error code value
        case future_errc::broken_promise:
            return "broken promise";

        case future_errc::future_already_retrieved:
            return "future already retrieved";

        case future_errc::promise_already_satisfied:
            return "promise already satisfied";

        case future_errc::no_state:
            return "no state";

        default:
            return nullptr;
        }
    }

    class future_error : public std::logic_error { // future exception
    public:
        explicit future_error(error_code _Errcode) // internal, TRANSITION, will be removed
            : std::logic_error(""), _code(_Errcode) {}

        explicit future_error(future_errc _Errno) : std::logic_error(""), _code(::stdex::make_error_code(_Errno)) {}

        const error_code& code() const _STDEX_NOEXCEPT_FUNCTION {
            return _code;
        }

        virtual const char* what() const _STDEX_NOEXCEPT_FUNCTION { // get message string
            return _future_error_map(_code.value());
        }

    private:
        error_code _code; // the stored error code
    };

    class _future_error_category 
        : public detail::_generic_error_category { // categorize a future error
    public:
        _future_error_category() {}

        virtual const char* name() const _STDEX_NOEXCEPT_FUNCTION 
        {
            return ("future");
        }

        virtual std::string message(int _Errcode) const 
        {
            const char* _name = _future_error_map(_Errcode);
            if (_name) {
                return _name;
            }

            return detail::_generic_error_category::message(_Errcode);
        }
    };

    inline const error_category& future_category() _STDEX_NOEXCEPT_FUNCTION
    {    // get future_category
        return (detail::_error_objects<_future_error_category>::_object());
    }

    template <class _Tp>
    class _associated_state;

    template <class _Tp>
    struct _deleter_base { // abstract base class for managing deletion of state objects
        virtual void _delete(_associated_state<_Tp>*) = 0;
        virtual ~_deleter_base() _STDEX_NOEXCEPT_FUNCTION {}
    };

    template <class _Tp, class _DerivedT, class _AllocT>
    struct _state_deleter : _deleter_base<_Tp> { // manage allocator and deletion state objects
        _state_deleter(const _AllocT& _aloc_value) : _alloc(_aloc_value) {}

        _state_deleter(const _state_deleter&) _STDEX_DELETED_FUNCTION;
        _state_deleter& operator=(const _state_deleter&) _STDEX_DELETED_FUNCTION;

        virtual void _delete(_associated_state<_Tp>* _state);

        _AllocT _alloc;
    };

    class atomic_uintmax_t
    {
    public:
        typedef unsigned long value_type;

        atomic_uintmax_t() {}
        atomic_uintmax_t(value_type _desired)
            : _value(_desired)
        { }

        value_type operator=(value_type _desired) _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<stdex::mutex> lock(_sync);
            _value = _desired;
            return _value;
        }

        value_type operator=(value_type _desired) volatile _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<volatile stdex::mutex> lock(_sync);
            _value = _desired;
            return _value;
        }

        void store(value_type _desired) _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<stdex::mutex> lock(_sync);
            _value = _desired;
        }

        void store(value_type _desired) volatile _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<volatile stdex::mutex> lock(_sync);
            _value = _desired;
        }

        value_type load() const _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<stdex::mutex> lock(_sync);
            return _value;
        }

        value_type load() const volatile _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<volatile stdex::mutex> lock(_sync);
            return _value;
        }

        value_type exchange(value_type _desired) _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<stdex::mutex> lock(_sync);
            std::swap(_desired, _value);
            return _desired;
        }

        value_type exchange(value_type _desired) volatile _STDEX_NOEXCEPT_FUNCTION
        {
            volatile value_type _desired_vol = _desired;
            stdex::unique_lock<volatile stdex::mutex> lock(_sync);
            std::swap(_desired_vol, _value);
            return _desired_vol;
        }

        // pre-increment
        value_type operator++() _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<stdex::mutex> lock(_sync);
            ++_value;
            return _value;
        }

        value_type operator++() volatile _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<volatile stdex::mutex> lock(_sync);
            ++_value;
            return _value;
        }

        // post-increment
        value_type operator++(int) _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<stdex::mutex> lock(_sync);
            _value++;
            return _value;
        }
        
        value_type operator++(int) volatile _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<volatile stdex::mutex> lock(_sync);
            _value++;
            return _value;
        }

        // pre-decrement
        value_type operator--() _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<stdex::mutex> lock(_sync);
            --_value;
            return _value;
        }

        value_type operator--() volatile _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<volatile stdex::mutex> lock(_sync);
            --_value;
            return _value;
        }

        // post-decrement
        value_type operator--(int) _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<stdex::mutex> lock(_sync);
            _value--;
            return _value;
        }
        
        value_type operator--(int) volatile _STDEX_NOEXCEPT_FUNCTION
        {
            stdex::unique_lock<volatile stdex::mutex> lock(_sync);
            _value--;
            return _value;
        }

    private:

        mutable stdex::mutex _sync;
        value_type _value; 
    };

    template <class _Tp>
    class _associated_state 
    { // class for managing associated synchronous state
    public:
        typedef _Tp state_type;
        typedef _deleter_base<_Tp> deleter_type;

        _associated_state(deleter_type* _del = nullptr)
            : _ref_counter(1), // non-atomic initialization
            _exception(), _retrieved(false), _ready(false), _ready_at_thread_exit(false), _has_stored_result(false),
            _is_running(false), _deleter(_del) 
        {
            // TRANSITION: _associated_state ctor assumes _Tp is default constructible
        }

        virtual ~_associated_state() _STDEX_NOEXCEPT_FUNCTION 
        {
            if (_has_stored_result && !_ready) 
            { // registered for release at thread exit
                void remove_from_this_thread_notification_data(condition_variable*, unique_lock<mutex>*);
                remove_from_this_thread_notification_data(&_condition, &_sync);
            }
        }

        void _retain() 
        { // increment reference count
            ++_ref_counter;
        }

        void _release() 
        { // decrement reference count and destroy when zero
            if ((--_ref_counter) == 0) {
                _delete_this();
            }
        }

    private:
        atomic_uintmax_t _ref_counter;

    public:
        virtual void _wait() 
        { // wait for signal
            unique_lock<mutex> lock(_sync);
            _maybe_run_deferred_function(lock);
            while (!_ready) {
                _condition.wait(lock);
            }
        }

        struct _test_ready 
        { // wraps _associated_state
            _test_ready(const _associated_state* _St) : _state(_St) {}

            bool operator()() const { // test state
                return _state->_ready != 0;
            }
            const _associated_state* _state;
        };

        template <class _Rep, class _Per>
        future_status _wait_for(const chrono::duration<_Rep, _Per>& _rel_time) 
        { // wait for duration
            unique_lock<mutex> lock(_sync);
            if (_has_deferred_function()) {
                return future_status::deferred;
            }

            if (_condition.wait_for(lock, _rel_time, _test_ready(this))) {
                return future_status::ready;
            }

            return future_status::timeout;
        }

        template <class _Clock, class _Dur>
        future_status _wait_until(const chrono::time_point<_Clock, _Dur>& _abs_time) 
        { // wait until time point
            unique_lock<mutex> lock(_sync);
            if (_has_deferred_function()) {
                return future_status::deferred;
            }

            if (_condition.wait_until(lock, _abs_time, _test_ready(this))) {
                return future_status::ready;
            }

            return future_status::timeout;
        }

        virtual _Tp& _get_value(bool _get_only_once) 
        {
            unique_lock<mutex> lock(_sync);
            if (_get_only_once && _retrieved) {
                throw future_error(make_error_code(future_errc::future_already_retrieved));
            }

            if (_exception) {
                throw (*_exception);
            }

            _retrieved = true;
            _maybe_run_deferred_function(lock);
            while (!_ready) {
                _condition.wait(lock);
            }

            if (_exception) {
                throw (*_exception);
            }

            return _result;
        }

        void _set_value(const _Tp& _value, bool _at_thread_exit) 
        { // store a result
            unique_lock<mutex> lock(_sync);
            _set_value_raw(_value, &lock, _at_thread_exit);
        }

        void _set_value_raw(const _Tp& _value, unique_lock<mutex>* lock,
            bool _At_thread_exit) 
        { // store a result while inside a locked block
            if (_has_stored_result) {
                throw future_error(make_error_code(future_errc::promise_already_satisfied));
            }

            _result = _value;
            _do_notify(lock, _At_thread_exit);
        }

        //void _set_value(_Tp&& _value, bool _At_thread_exit) 
        //{ // store a result
        //    unique_lock<mutex> lock(_sync);
        //    _set_value_raw(::stdex::forward<_Tp>(_value), &lock, _At_thread_exit);
        //}

        //void _set_value_raw(_Tp&& _value, unique_lock<mutex>* lock,
        //    bool _At_thread_exit) 
        //{ // store a result while inside a locked block
        //    if (_has_stored_result) {
        //        throw future_error(make_error_code(future_errc::promise_already_satisfied));
        //    }
        //
        //    _result = ::stdex::forward<_Tp>(_value);
        //    _do_notify(lock, _At_thread_exit);
        //}

        void _set_value(bool _at_thread_exit) 
        { // store a (void) result
            unique_lock<mutex> lock(_sync);
            _set_value_raw(&lock, _at_thread_exit);
        }

        void _set_value_raw(
            unique_lock<mutex>* lock, bool _at_thread_exit) 
        { // store a (void) result while inside a locked block
            if (_has_stored_result) {
                throw future_error(make_error_code(future_errc::promise_already_satisfied));
            }

            _do_notify(lock, _at_thread_exit);
        }

        void _set_exception(_nullable_exception _excpt, bool _at_thread_exit) 
        { // store a result
            unique_lock<mutex> lock(_sync);
            _set_exception_raw(_excpt, &lock, _at_thread_exit);
        }

        void _set_exception_raw(_nullable_exception _excpt, unique_lock<mutex>* lock,
            bool _at_thread_exit) 
        { // store a result while inside a locked block
            if (_has_stored_result) {
                throw future_error(make_error_code(future_errc::promise_already_satisfied));
            }

            _exception = _excpt;
            _do_notify(lock, _at_thread_exit);
        }

        bool _is_ready() const 
        {
            return _ready != 0;
        }

        bool _is_ready_at_thread_exit() const 
        {
            return _ready_at_thread_exit;
        }

        bool _already_has_stored_result() const 
        {
            return _has_stored_result;
        }

        bool _already_retrieved() const 
        {
            return _retrieved;
        }

        void _abandon() 
        { // abandon shared state
            unique_lock<mutex> lock(_sync);
            if (!_has_stored_result) { // queue exception
                future_error _fut_err(make_error_code(future_errc::broken_promise));
                _set_exception_raw(::stdex::_nullable_exception(_fut_err), &lock, false);
            }
        }

    protected:
        void _Make_ready_at_thread_exit() 
        { // set ready status at thread exit
            if (_ready_at_thread_exit) {
                _ready = true;
            }
        }

        void _maybe_run_deferred_function(unique_lock<mutex>& _lock) 
        { // run a deferred function if not already done
            if (!_is_running) { // run the function
                _is_running = true;
                _run_deferred_function(_lock);
            }
        }

    public:
        _Tp _result;
        _nullable_exception _exception;
        mutex _sync;
        condition_variable _condition;
        bool _retrieved;
        int _ready;
        bool _ready_at_thread_exit;
        bool _has_stored_result;
        bool _is_running;

    private:
        virtual bool _has_deferred_function() const _STDEX_NOEXCEPT_FUNCTION 
        { // overridden by _Deferred_async_state
            return false;
        }

        virtual void _run_deferred_function(unique_lock<mutex>&) {} // do nothing

        virtual void _do_notify(unique_lock<mutex>* lock, bool _at_thread_exit) 
        { // notify waiting threads
            // TRANSITION, ABI: This is virtual, but never overridden.
            _has_stored_result = true;
            if (_at_thread_exit) { // notify at thread exit
                notify_all_at_thread_exit(_condition, *lock);
            } else { // notify immediately
                _ready = true;
                _condition.notify_all();
            }
        }

        void _delete_this() 
        { // delete this object
            if (_deleter) {
                _deleter->_delete(this);
            } else {
                delete this;
            }
        }

        deleter_type* _deleter;

    public:
        _associated_state(const _associated_state&) _STDEX_DELETED_FUNCTION;
        _associated_state& operator=(const _associated_state&) _STDEX_DELETED_FUNCTION;
    };

    template <class _Tp, class _Derived, class _Alloc>
    void _state_deleter<_Tp, _Derived, _Alloc>::_delete(
        _associated_state<_Tp>* _state) 
    { // delete _state and this using stored allocator
        delete _state;
    }

    template <class _Tp>
    class _state_manager {
        // class for managing possibly non-existent associated asynchronous state object
    public:
        _state_manager() : _assoc_state(nullptr) 
        { // construct with no associated asynchronous state object
            _get_only_once = false;
        }

        _state_manager(_associated_state<_Tp>* _New_state, bool _get_once)
            : _assoc_state(_New_state) 
        { // construct with _New_state
            _get_only_once = _get_once;
        }

        _state_manager(const _state_manager& other, bool _get_once = false) : 
            _assoc_state(nullptr) 
        {
            _copy_from(other);
            _get_only_once = _get_once;
        }

        //_state_manager(_state_manager&& other, bool _Get_once = false) : _assoc_state(nullptr) {
        //    _move_from(other);
        //    _get_only_once = _Get_once;
        //}
        _state_manager(detail::future_detail::_rv_reference<_state_manager>& other, bool _Get_once = false)
            : _assoc_state(nullptr)
        {
            _move_from(reinterpret_cast<_state_manager&>(other));
            _get_only_once = _Get_once;
        }

        ~_state_manager() _STDEX_NOEXCEPT_FUNCTION 
        {
            if (_assoc_state) 
                _assoc_state->_release();
        }

        _state_manager& operator=(const _state_manager& other) 
        {
            _copy_from(other);
            return *this;
        }

        //_state_manager& operator=(_state_manager&& other) {
        //    _move_from(other);
        //    return *this;
        //}

        bool valid() const _STDEX_NOEXCEPT_FUNCTION 
        {
            return _assoc_state && !(_get_only_once && _assoc_state->_already_retrieved());
        }

        void wait() const 
        { // wait for signal
            if (!valid()) {
                throw future_error(make_error_code(future_errc::no_state));
            }

            _assoc_state->_wait();
        }

        template <class _Rep, class _Per>
        future_status wait_for(const chrono::duration<_Rep, _Per>& _rel_time) const 
        { // wait for duration
            if (!valid()) {
                throw future_error(make_error_code(future_errc::no_state));
            }

            return _assoc_state->_wait_for(_rel_time);
        }

        template <class _Clock, class _Dur>
        future_status wait_until(const chrono::time_point<_Clock, _Dur>& _abs_time) const 
        { // wait until time point
            if (!valid()) {
                throw future_error(make_error_code(future_errc::no_state));
            }

            return _assoc_state->_wait_until(_abs_time);
        }

        _Tp& _get_value() const 
        {
            if (!valid()) {
                throw future_error(make_error_code(future_errc::no_state));
            }

            return _assoc_state->_get_value(_get_only_once);
        }

        void _set_value(const _Tp& _value, bool _defer) 
        { // store a result
            if (!valid()) {
                throw future_error(make_error_code(future_errc::no_state));
            }

            _assoc_state->_set_value(_value, _defer);
        }

        //void _set_value(_Tp&& _value, bool _Defer) { // store a result
        //    if (!valid()) {
        //        throw future_error(make_error_code(future_errc::no_state));
        //    }
        //
        //    _assoc_state->_set_value(::stdex::forward<_Tp>(_value), _Defer);
        //}

        void _abandon() 
        { // abandon shared state
            if (_assoc_state) {
                _assoc_state->_abandon();
            }
        }

        void _set_exception(_nullable_exception _excpt, bool _Defer) 
        { // store a result
            if (!valid()) {
                throw future_error(make_error_code(future_errc::no_state));
            }

            _assoc_state->_set_exception(_excpt, _Defer);
        }

        void _swap(_state_manager& other) 
        { // exchange with other
            ::stdex::swap(_assoc_state, other._assoc_state);
        }

        _associated_state<_Tp>* _ptr() const 
        {
            return _assoc_state;
        }

        void _copy_from(const _state_manager& other) 
        { // copy stored associated asynchronous state object from other
            if (this != ::stdex::detail::future_detail::addressof(other)) {
                if (_assoc_state) {
                    _assoc_state->_release();
                }

                if (other._assoc_state) { // do the copy
                    other._assoc_state->_retain();
                    _assoc_state   = other._assoc_state;
                    _get_only_once = other._get_only_once;
                } else {
                    _assoc_state = nullptr;
                }
            }
        }

        void _move_from(_state_manager& other) 
        { // move stored associated asynchronous state object from other
            if (this != ::stdex::detail::future_detail::addressof(other)) 
            {
                if (_assoc_state) 
                    _assoc_state->_release();

                _assoc_state        = other._assoc_state;
                other._assoc_state = nullptr;
                _get_only_once      = other._get_only_once;
            }
        }

        bool _is_ready() const 
        {
            return _assoc_state && _assoc_state->_is_ready();
        }

        bool _is_ready_at_thread_exit() const 
        {
            return _assoc_state && _assoc_state->_is_ready_at_thread_exit();
        }

    private:
        _associated_state<_Tp>* _assoc_state;
        bool _get_only_once;
    };

    // CLASS TEMPLATE future
    template <class _Tp>
    class shared_future;

    template <class _Tp>
    class future : 
        public _state_manager<_Tp> 
    {
        // class that defines a non-copyable asynchronous return object that holds a value
        typedef _state_manager<_Tp> base_type;

    public:
        future() _STDEX_NOEXCEPT_FUNCTION {}

        //future(future&& other) _STDEX_NOEXCEPT_FUNCTION : base_type(::stdex::move(other), true) {}
        future(detail::future_detail::_rv_reference <future>& other) _STDEX_NOEXCEPT_FUNCTION :
            base_type(reinterpret_cast<detail::future_detail::_rv_reference<base_type>&>(other), true) {}

        //future& operator=(future&& other) _STDEX_NOEXCEPT_FUNCTION {
        //    base_type::operator=(::stdex::move(other));
        //    return *this;
        //}

        future(const base_type& _state, stdex::detail::future_detail::_nil_type) : base_type(_state, true) {}

        ~future() _STDEX_NOEXCEPT_FUNCTION {}

        _Tp get() 
        {
            // block until ready then return the stored result or throw the stored exception
            return this->_get_value();
        }

        shared_future<_Tp> share() _STDEX_NOEXCEPT_FUNCTION 
        {
            return shared_future<_Tp>(*this);
        }

    private:
        future(const future&) _STDEX_DELETED_FUNCTION;
        future& operator=(const future&) _STDEX_DELETED_FUNCTION;
    };

    template <class _Tp>
    class future<_Tp&> : 
        public _state_manager<_Tp*> 
    {
        // class that defines a non-copyable asynchronous return object that holds a reference
        typedef _state_manager<_Tp*> base_type;

    public:
        future() _STDEX_NOEXCEPT_FUNCTION {}

        //future(future&& other) _STDEX_NOEXCEPT_FUNCTION : base_type(::stdex::move(other), true) {}
        future(detail::future_detail::_rv_reference<future>& other) _STDEX_NOEXCEPT_FUNCTION :
            base_type(reinterpret_cast<detail::future_detail::_rv_reference<base_type>&>(other), true) {}

        //future& operator=(future&& other) _STDEX_NOEXCEPT_FUNCTION 
        //{
        //    base_type::operator=(::stdex::move(other));
        //    return *this;
        //}

        future(const base_type& _state, detail::future_detail::_nil_type) : base_type(_state, true) {}

        ~future() _STDEX_NOEXCEPT_FUNCTION {}

        _Tp& get() 
        {
            // block until ready then return the stored result or throw the stored exception
            return *(this->_get_value());
        }

        shared_future<_Tp&> share() _STDEX_NOEXCEPT_FUNCTION 
        {
            return shared_future<_Tp&>(*this);
        }

    private:
        future(const future&) _STDEX_DELETED_FUNCTION;
        future& operator=(const future&) _STDEX_DELETED_FUNCTION;
    };

    template <>
    class future<void> : 
        public _state_manager<int> 
    {
        // class that defines a non-copyable asynchronous return object that does not hold a value
        typedef _state_manager<int> base_type;

    public:
        future() _STDEX_NOEXCEPT_FUNCTION {}

        //future(future&& other) _STDEX_NOEXCEPT_FUNCTION : _base(::stdex::move(other), true) {}
        future(detail::future_detail::_rv_reference<future>& other) _STDEX_NOEXCEPT_FUNCTION :
            base_type(reinterpret_cast<detail::future_detail::_rv_reference<base_type>&>(other), true) {}

        //future& operator=(future&& other) _STDEX_NOEXCEPT_FUNCTION {
        //    _base::operator=(::stdex::move(other));
        //    return *this;
        //}

        future(const base_type& _state, detail::future_detail::_nil_type) : base_type(_state, true) {}

        ~future() _STDEX_NOEXCEPT_FUNCTION {}

        void get() 
        {
            // block until ready then return or throw the stored exception
            this->_get_value();
        }

        shared_future<void> share() _STDEX_NOEXCEPT_FUNCTION;

    private:
        future(const future&) _STDEX_DELETED_FUNCTION;
        future& operator=(const future&) _STDEX_DELETED_FUNCTION;
    };

    // CLASS TEMPLATE shared_future
    template <class _Tp>
    class shared_future : 
        public _state_manager<_Tp> 
    {
        // class that defines a copyable asynchronous return object that holds a value
        typedef _state_manager<_Tp> base_type;

    public:
        shared_future() _STDEX_NOEXCEPT_FUNCTION {}

        shared_future(const shared_future& other) _STDEX_NOEXCEPT_FUNCTION : base_type(other) {}

        shared_future& operator=(const shared_future& other) _STDEX_NOEXCEPT_FUNCTION 
        {
            base_type::operator=(other);
            return *this;
        }

        //shared_future(future<_Tp>&& other) _STDEX_NOEXCEPT_FUNCTION : base_type(::stdex::forward<base_type>(other)) {}

        //shared_future(shared_future&& other) _STDEX_NOEXCEPT_FUNCTION : base_type(::stdex::move(other)) {}

        //shared_future& operator=(shared_future&& other) _STDEX_NOEXCEPT_FUNCTION {
        //    base_type::operator=(::stdex::move(other));
        //    return *this;
        //}

        ~shared_future() _STDEX_NOEXCEPT_FUNCTION {}

        const _Tp& get() const 
        {
            // block until ready then return the stored result or throw the stored exception
            return this->_get_value();
        }
    };

    template <class _Tp>
    class shared_future<_Tp&> : public _state_manager<_Tp*> 
    {
        // class that defines a copyable asynchronous return object that holds a reference
        typedef _state_manager<_Tp*> base_type;

    public:
        shared_future() _STDEX_NOEXCEPT_FUNCTION {}

        shared_future(const shared_future& other) _STDEX_NOEXCEPT_FUNCTION : base_type(other) {}

        shared_future& operator=(const shared_future& other) _STDEX_NOEXCEPT_FUNCTION 
        {
            base_type::operator=(other);
            return *this;
        }

        //shared_future(future<_Tp&>&& other) _STDEX_NOEXCEPT_FUNCTION : base_type(::stdex::forward<base_type>(other)) {}

        //shared_future(shared_future&& other) _STDEX_NOEXCEPT_FUNCTION : base_type(::stdex::move(other)) {}

        //shared_future& operator=(shared_future&& other) _STDEX_NOEXCEPT_FUNCTION {
        //    base_type::operator=(::stdex::move(other));
        //    return *this;
        //}

        ~shared_future() _STDEX_NOEXCEPT_FUNCTION {}

        _Tp& get() const 
        {
            // block until ready then return the stored result or throw the stored exception
            return *this->_get_value();
        }
    };

    template <>
    class shared_future<void> : 
        public _state_manager<int> 
    {
        // class that defines a copyable asynchronous return object that does not hold a value
        typedef _state_manager<int> base_type;

    public:
        shared_future() _STDEX_NOEXCEPT_FUNCTION {}

        shared_future(const shared_future& other) _STDEX_NOEXCEPT_FUNCTION : base_type(other) {}

        shared_future& operator=(const shared_future& other) _STDEX_NOEXCEPT_FUNCTION 
        {
            base_type::operator=(other);
            return *this;
        }

        //shared_future(shared_future&& other) _STDEX_NOEXCEPT_FUNCTION : base_type(::stdex::move(other)) {}

        //shared_future(future<void>&& other) _STDEX_NOEXCEPT_FUNCTION : base_type(::stdex::forward<base_type>(other)) {}

        //shared_future& operator=(shared_future&& other) {
        //    base_type::operator=(::stdex::move(other));
        //    return *this;
        //}

        ~shared_future() _STDEX_NOEXCEPT_FUNCTION {}

        void get() const 
        { // block until ready then return or throw the stored exception
            this->_get_value();
        }
    };

    //inline shared_future<void> future<void>::share() _STDEX_NOEXCEPT_FUNCTION {
    //    return shared_future<void>(::stdex::move(*this));
    //}

    template <class _Tp>
    class _promise_base
    {
    public:
        _promise_base(_associated_state<_Tp>* _state_ptr) : _state(_state_ptr, false), _future_retrieved(false) {}

        //_promise(_promise&& other) : _state(::stdex::move(other._state)), _future_retrieved(other._future_retrieved) {}

        //_promise& operator=(_promise&& other) {
        //    _state            = ::stdex::move(other._state);
        //    _future_retrieved = other._future_retrieved;
        //    return *this;
        //}

        ~_promise_base() _STDEX_NOEXCEPT_FUNCTION {}

        void _swap(_promise_base& other) {
            _state._swap(other._state);
            ::stdex::swap(_future_retrieved, other._future_retrieved);
        }

        const _state_manager<_Tp>& _get_state() const 
        {
            return _state;
        }
        _state_manager<_Tp>& _get_state() {
            return _state;
        }

        _state_manager<_Tp>& _get_state_for_set() 
        {
            if (!_state.valid()) {
                throw future_error(make_error_code(future_errc::no_state));
            }

            return _state;
        }

        _state_manager<_Tp>& _get_state_for_future() 
        {
            if (!_state.valid()) {
                throw future_error(make_error_code(future_errc::no_state));
            }

            if (_future_retrieved) {
                throw future_error(make_error_code(future_errc::future_already_retrieved));
            }

            _future_retrieved = true;
            return _state;
        }

        bool _is_valid() const _STDEX_NOEXCEPT_FUNCTION 
        {
            return _state.valid();
        }

        bool _is_ready() const 
        {
            return _state._is_ready();
        }

        bool _is_ready_at_thread_exit() const 
        {
            return _state._is_ready_at_thread_exit();
        }

    private:
        _state_manager<_Tp> _state;
        bool _future_retrieved;

        _promise_base(const _promise_base&) _STDEX_DELETED_FUNCTION;
        _promise_base& operator=(const _promise_base&) _STDEX_DELETED_FUNCTION;
    };

    template <class _Tp>
    class promise 
    { // class that defines an asynchronous provider that holds a value
    public:
        promise() : _promise_impl(new _associated_state<_Tp>) {}

        //template <class _AllocT>
        //promise(allocator_arg_t, const _AllocT& _aloc_value) : _promise_impl(_Make_associated_state<_Tp>(_aloc_value)) {}

        //promise(promise&& other) _STDEX_NOEXCEPT_FUNCTION : _promise(::stdex::move(other._promise)) {}

        //promise& operator=(promise&& other) _STDEX_NOEXCEPT_FUNCTION {
        //    promise(::stdex::move(other)).swap(*this);
        //    return *this;
        //}

        ~promise() _STDEX_NOEXCEPT_FUNCTION 
        {
            if (_promise_impl._is_valid() && !_promise_impl._is_ready() && !_promise_impl._is_ready_at_thread_exit()) {
                // exception if destroyed before function object returns
                future_error _fut_err(make_error_code(future_errc::broken_promise));
                _promise_impl._get_state()._set_exception(_nullable_exception(_fut_err), false);
            }
        }

        void swap(promise& other) _STDEX_NOEXCEPT_FUNCTION 
        {
            _promise_impl._swap(other._promise_impl);
        }

        future<_Tp> get_future() 
        {
            future<_Tp> result(
                _promise_impl._get_state_for_future(),
                detail::future_detail::_nil_type()
            );

            return reinterpret_cast<detail::future_detail::_rv_reference<future<_Tp>/**/>&>(result);
        }

        void set_value(const _Tp& _value) 
        {
            _promise_impl._get_state_for_set()._set_value(_value, false);
        }

        void set_value_at_thread_exit(const _Tp& _value) 
        {
            _promise_impl._get_state_for_set()._set_value(_value, true);
        }

        //void set_value(_Tp&& _value) 
        //{
        //    _promise_impl._get_state_for_set()._set_value(::stdex::forward<_Tp>(_value), false);
        //}

        //void set_value_at_thread_exit(_Tp&& _value) 
        //{
        //    _promise_impl._get_state_for_set()._set_value(::stdex::forward<_Tp>(_value), true);
        //}

        void set_exception(_nullable_exception _excpt) 
        {
            _promise_impl._get_state_for_set()._set_exception(_excpt, false);
        }

        void set_exception_at_thread_exit(_nullable_exception _excpt) {
            _promise_impl._get_state_for_set()._set_exception(_excpt, true);
        }

    private:
        _promise_base<_Tp> _promise_impl;

        promise(const promise&) _STDEX_DELETED_FUNCTION;
        promise& operator=(const promise&) _STDEX_DELETED_FUNCTION;
    };

    template <class _Tp>
    class promise<_Tp&> 
    { // class that defines an asynchronous provider that holds a reference
    public:
        promise() : _promise_impl(new _associated_state<_Tp*>) {}

        //template <class _AllocT>
        //promise(allocator_arg_t, const _AllocT& _aloc_value) : _promise_impl(_Make_associated_state<_Tp*>(_aloc_value)) {}

        //promise(promise&& other) _STDEX_NOEXCEPT_FUNCTION : _promise(::stdex::move(other._promise)) {}

        //promise& operator=(promise&& other) _STDEX_NOEXCEPT_FUNCTION {
        //    promise(::stdex::move(other)).swap(*this);
        //    return *this;
        //}

        ~promise() _STDEX_NOEXCEPT_FUNCTION {
            if (_promise_impl._is_valid() && !_promise_impl._is_ready() && !_promise_impl._is_ready_at_thread_exit()) {
                // exception if destroyed before function object returns
                future_error _fut_err(make_error_code(future_errc::broken_promise));
                _promise_impl._get_state()._set_exception(_nullable_exception(_fut_err), false);
            }
        }

        void swap(promise& other) _STDEX_NOEXCEPT_FUNCTION {
            _promise_impl._swap(other._promise_impl);
        }

        future<_Tp&> get_future() {
            future<_Tp&> result(
                _promise_impl._get_state_for_future(),
                detail::future_detail::_nil_type()
            );

            return reinterpret_cast<detail::future_detail::_rv_reference<future<_Tp&>/**/>&>(result);
        }

        void set_value(_Tp& _value) {
            _promise_impl._get_state_for_set()._set_value(::stdex::detail::future_detail::addressof(_value), false);
        }

        void set_value_at_thread_exit(_Tp& _value) {
            _promise_impl._get_state_for_set()._set_value(::stdex::detail::future_detail::addressof(_value), true);
        }

        void set_exception(_nullable_exception _excpt) {
            _promise_impl._get_state_for_set()._set_exception(_excpt, false);
        }

        void set_exception_at_thread_exit(_nullable_exception _excpt) {
            _promise_impl._get_state_for_set()._set_exception(_excpt, true);
        }

    private:
        _promise_base<_Tp*> _promise_impl;

        promise(const promise&) _STDEX_DELETED_FUNCTION;
        promise& operator=(const promise&) _STDEX_DELETED_FUNCTION;
    };

    template <>
    class promise<void> 
    { // defines an asynchronous provider that does not hold a value
    public:
        promise() : _promise_impl(new _associated_state<int>) {}

        //template <class _AllocT>
        //promise(allocator_arg_t, const _AllocT& _aloc_value) : _promise_impl(_associated_state<int>(_aloc_value)) {}

        //promise(promise&& other) _STDEX_NOEXCEPT_FUNCTION : _promise(::stdex::move(other._promise)) {}

        //promise& operator=(promise&& other) _STDEX_NOEXCEPT_FUNCTION {
        //    promise(::stdex::move(other)).swap(*this);
        //    return *this;
        //}

        ~promise() _STDEX_NOEXCEPT_FUNCTION {
            if (_promise_impl._is_valid() && !_promise_impl._is_ready() && !_promise_impl._is_ready_at_thread_exit()) {
                // exception if destroyed before function object returns
                future_error _fut_err(make_error_code(future_errc::broken_promise));
                _promise_impl._get_state()._set_exception(_nullable_exception(_fut_err), false);
            }
        }

        void swap(promise& other) _STDEX_NOEXCEPT_FUNCTION {
            _promise_impl._swap(other._promise_impl);
        }

        future<void> get_future() {
            // moving
            future<void> result(
                _promise_impl._get_state_for_future(),
                detail::future_detail::_nil_type()
            );

            return reinterpret_cast<detail::future_detail::_rv_reference<future<void>/**/>&>(result);
        }

        void set_value() {
            _promise_impl._get_state_for_set()._set_value(1, false);
        }

        void set_value_at_thread_exit() {
            _promise_impl._get_state_for_set()._set_value(1, true);
        }

        void set_exception(_nullable_exception _excpt) {
            _promise_impl._get_state_for_set()._set_exception(_excpt, false);
        }

        void set_exception_at_thread_exit(_nullable_exception _excpt) {
            _promise_impl._get_state_for_set()._set_exception(_excpt, true);
        }

    private:
        _promise_base<int> _promise_impl;

        promise(const promise&) _STDEX_DELETED_FUNCTION;
        promise& operator=(const promise&) _STDEX_DELETED_FUNCTION;
    };

    template <class _Tp>
    void swap(promise<_Tp>& _lhs, promise<_Tp>& _rhs) _STDEX_NOEXCEPT_FUNCTION {
        _lhs.swap(_rhs);
    }

} // namespace stdex

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_FUTURE_H
