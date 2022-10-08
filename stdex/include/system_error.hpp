#ifndef _STDEX_SYSTEM_ERROR_H
#define _STDEX_SYSTEM_ERROR_H
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./type_traits.hpp" // stdex::enable_if, stdex::integral_constant

// POSIX includes
/*none*/

// std includes


#ifdef __STDC_WANT_LIB_EXT1__

#define __STDC_WANT_LIB_EXT1__ 1
// As with all bounds-checked functions, strerror_s and strerrorlen_s are only guaranteed to be available 
// if __STDC_LIB_EXT1__ is defined by the implementation and 
// if the user defines __STDC_WANT_LIB_EXT1__ to the integer constant 1 before including string.h.
#include <string.h>

#endif

#include <errno.h>
#include <cerrno>
#include <cstdlib>        // std::strerror 
#include <cstring>        // std::strerror
#include <stdexcept>    // std::runtime_error
#include <string>         // std::string
#include <functional>


#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept

#else

#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif 

namespace stdex
{
    struct errc
    {
        typedef
        enum errc_t {    // names for generic error codes
#ifdef EAFNOSUPPORT
            address_family_not_supported = EAFNOSUPPORT,
#else
            address_family_not_supported = 102,
#endif
#ifdef EADDRINUSE
            address_in_use = EADDRINUSE,
#else
            address_in_use = 100,
#endif
#ifdef EADDRNOTAVAIL
            address_not_available = EADDRNOTAVAIL,
#else
            address_not_available = 101,
#endif
#ifdef EISCONN
            already_connected = EISCONN,
#else
            already_connected = 113,
#endif
            argument_list_too_long = E2BIG,
            argument_out_of_domain = EDOM,
            bad_address = EFAULT,
            bad_file_descriptor = EBADF,
#ifdef EBADMSG
            bad_message = EBADMSG,
#else
            bad_message = 103,
#endif
            broken_pipe = EPIPE,
#ifdef ECONNABORTED
            connection_aborted = ECONNABORTED,
#else
            connection_aborted = 106,
#endif
#ifdef EALREADY
            connection_already_in_progress = EALREADY,
#else
            connection_already_in_progress = 103,
#endif
#ifdef ECONNREFUSED
            connection_refused = ECONNREFUSED,
#else
            connection_refused = 107,
#endif
#ifdef ECONNRESET
            connection_reset = ECONNRESET,
#else
            connection_reset = 108,
#endif
            cross_device_link = EXDEV,
#ifdef EDESTADDRREQ
            destination_address_required = EDESTADDRREQ,
#else
            destination_address_required = 109,
#endif
            device_or_resource_busy = EBUSY,
            directory_not_empty = ENOTEMPTY,
            executable_format_error = ENOEXEC,
            file_exists = EEXIST,
            file_too_large = EFBIG,
            filename_too_long = ENAMETOOLONG,
#ifdef ENOSYS
            function_not_supported = ENOSYS,
#else
            function_not_supported = 40,
#endif
#ifdef EHOSTUNREACH
            host_unreachable = EHOSTUNREACH,
#else
            host_unreachable = 110,
#endif
#ifdef EIDRM
            identifier_removed = EIDRM,
#else
            identifier_removed = 111,
#endif
#ifdef EILSEQ
            illegal_byte_sequence = EILSEQ,
#else
            illegal_byte_sequence = 42,
#endif
            inappropriate_io_control_operation = ENOTTY,
            interrupted = EINTR,
            invalid_argument = EINVAL,
            invalid_seek = ESPIPE,
            io_error = EIO,
            is_a_directory = EISDIR,
#ifdef EMSGSIZE
            message_size = EMSGSIZE,
#else
            message_size = 115,
#endif
#ifdef ENETDOWN
            network_down = ENETDOWN,
#else
            network_down = 116,
#endif
#ifdef ENETRESET
            network_reset = ENETRESET,
#else
            network_reset = 117,
#endif
#ifdef ENETUNREACH
            network_unreachable = ENETUNREACH,
#else
            network_unreachable = 118,
#endif
#ifdef ENOBUFS
            no_buffer_space = ENOBUFS,
#else
            no_buffer_space = 119,
#endif
            no_child_process = ECHILD,
#ifdef ENOLINK
            no_link = ENOLINK,
#else
            no_link = 121,
#endif
#ifdef ENOLCK
            no_lock_available = ENOLCK,
#else
            no_lock_available = 39,
#endif
#ifdef ENODATA
            no_message_available = ENODATA,
#else
            no_message_available = 120,
#endif
#ifdef ENOMSG
            no_message = ENOMSG,
#else
            no_message = 122,
#endif
#ifdef ENOPROTOOPT
            no_protocol_option = ENOPROTOOPT,
#else
            no_protocol_option = 123,
#endif
            no_space_on_device = ENOSPC,
#ifdef ENOSR
            no_stream_resources = ENOSR,
#else
            no_stream_resources = 124,
#endif
            no_such_device_or_address = ENXIO,
        no_such_device = ENODEV,
        no_such_file_or_directory = ENOENT,
        no_such_process = ESRCH,
        not_a_directory = ENOTDIR,
#ifdef ENOTSOCK
            not_a_socket = ENOTSOCK,
#else
            not_a_socket = 128,
#endif
#ifdef ENOSTR
            not_a_stream = ENOSTR,
#else
            not_a_stream = 125,
#endif
#ifdef ENOTCONN
            not_connected = ENOTCONN,
#else
            not_connected = 126,
#endif
            not_enough_memory = ENOMEM,
#ifdef ENOTSUP
            not_supported = ENOTSUP,
#else
            not_supported = 129,
#endif
#ifdef ECANCELED
            operation_canceled = ECANCELED,
#else
            operation_canceled = 105,
#endif
#ifdef EINPROGRESS
            operation_in_progress = EINPROGRESS,
#else
            operation_in_progress = 112,
#endif
            operation_not_permitted = EPERM,
#ifdef EOPNOTSUPP
            operation_not_supported = EOPNOTSUPP,
#else
            operation_not_supported = 130,
#endif
#ifdef EWOULDBLOCK
            operation_would_block = EWOULDBLOCK,
#else
            operation_would_block = 140,
#endif
#ifdef EOWNERDEAD
            owner_dead = EOWNERDEAD,
#else
            owner_dead = 133,
#endif
            permission_denied = EACCES,
#ifdef EPROTO
            protocol_error = EPROTO,
#else
            protocol_error = 134,
#endif
#ifdef EPROTONOSUPPORT
            protocol_not_supported = EPROTONOSUPPORT,
#else
            protocol_not_supported = 135,
#endif
            read_only_file_system = EROFS,
#ifdef EDEADLK
            resource_deadlock_would_occur = EDEADLK,
#else
            resource_deadlock_would_occur = 36,
#endif
            resource_unavailable_try_again = EAGAIN,
            result_out_of_range = ERANGE,
#ifdef ENOTRECOVERABLE
            state_not_recoverable = ENOTRECOVERABLE,
#else
            state_not_recoverable = 127,
#endif
#ifdef ETIME
            stream_timeout = ETIME,
#else
            stream_timeout = 137,
#endif
#ifdef ETXTBSY
            text_file_busy = ETXTBSY,
#else
            text_file_busy = 139,
#endif
#ifdef ETIMEDOUT
            timed_out = ETIMEDOUT,
#else
            timed_out = 138,
#endif
            too_many_files_open_in_system = ENFILE,
            too_many_files_open = EMFILE,
            too_many_links = EMLINK,
#ifdef ELOOP
            too_many_symbolic_link_levels = ELOOP,
#else
            too_many_symbolic_link_levels = 114,
#endif
#ifdef EOVERFLOW
            value_too_large = EOVERFLOW,
#else
            value_too_large = 132,
#endif
#ifdef EPROTOTYPE
            wrong_protocol_type = EPROTOTYPE
#else
            wrong_protocol_type = 136
#endif
        } type;

        errc(type code):
            _code(code)
        {

        }

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

    typedef errc generic_errno;

    struct io_errc
    {
        typedef
        enum io_errc_t
        {
            stream = 1
        } type;

        io_errc(type code):
            _code(code)
        {

        }

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

    // TEMPLATE CLASS is_error_code_enum
    template<class _Enum>
    struct is_error_code_enum
    {    // tests for error_code enumeration
        static const bool value = false;
    };

    template<>
    struct is_error_code_enum<io_errc>
    {    // tests for error_code enumeration
        static const bool value = true;
    };

    template<>
    struct is_error_code_enum<io_errc::type>
    {    // tests for error_code enumeration
        static const bool value = true;
    };

    // TEMPLATE CLASS is_error_condition_enum
    template<class _Enum>
    struct is_error_condition_enum
    {    // tests for error_condition enumeration
        static const bool value = false;
    };

    template<>
    struct is_error_condition_enum<errc::type>
    {
        static const bool value = true;
    };

    template<>
    struct is_error_condition_enum<generic_errno>
    {    // tests for error_condition enumeration
        static const bool value = true;
    };

    class error_code;
    class error_condition;


    error_code make_error_code(errc::errc_t) _STDEX_NOEXCEPT_FUNCTION;
    error_code make_error_code(io_errc) _STDEX_NOEXCEPT_FUNCTION;
    error_condition make_error_condition(errc::errc_t) _STDEX_NOEXCEPT_FUNCTION;
    error_condition make_error_condition(io_errc) _STDEX_NOEXCEPT_FUNCTION;

    class error_category;

    const error_category& generic_category() _STDEX_NOEXCEPT_FUNCTION;
    const error_category& iostream_category() _STDEX_NOEXCEPT_FUNCTION;
    const error_category& system_category() _STDEX_NOEXCEPT_FUNCTION;

} // namespace stdex

// '_stdex_ADL' namespace is used for compilers that have bugged ADL:
// like failing miserably to see other function overloads 
// with 'using' directive or functions in global namespace.
// 
// Usage: we hide there wrapper functions that trigger ADL "properly".
// 
// Looks ugly, pollutes global namespace with extra symbol but  
// g e t s  j o b  d o n e
namespace _stdex_ADL
{
    template<class _Tp>
    inline
    static stdex::error_code _make_error_code(_Tp);

    template<class _Tp>
    inline
    static stdex::error_condition _make_error_condition(_Tp);
} // namespace _stdex_ADL

namespace stdex
{
    class error_category
    {
    public:
        error_category() _STDEX_NOEXCEPT_FUNCTION
        {}

        virtual ~error_category() _STDEX_NOEXCEPT_FUNCTION
        {}

        virtual const char* name() const _STDEX_NOEXCEPT_FUNCTION = 0;

        // We need two different virtual functions here, one returning a
        // COW string and one returning an SSO string. Their positions in the
        // vtable must be consistent for dynamic dispatch to work, but which one
        // the name "message()" finds depends on which ABI the caller is using.

        virtual std::string message(int) const = 0;

    public:
        virtual error_condition default_error_condition(int val) const _STDEX_NOEXCEPT_FUNCTION;

        virtual bool equivalent(int val, const error_condition &cond) const _STDEX_NOEXCEPT_FUNCTION;

        virtual bool equivalent(const error_code& code, int) const _STDEX_NOEXCEPT_FUNCTION;

        inline bool operator<(const error_category &other) const _STDEX_NOEXCEPT_FUNCTION
        {
            return std::less<const error_category*>()(this, &other);
        }

        inline bool operator==(const error_category &other) const _STDEX_NOEXCEPT_FUNCTION
        {
            return this == &other;
        }

        inline bool operator!=(const error_category &other) const _STDEX_NOEXCEPT_FUNCTION
        {
            return this != &other;
        }

    private:
        error_category(const error_category&) _STDEX_DELETED_FUNCTION;
        error_category& operator=(const error_category&) _STDEX_DELETED_FUNCTION;
    };

    namespace detail
    {
        struct _error_condition_compare
        {
            operator
            const stdex::error_condition&() const{
                return reinterpret_cast<const error_condition&>(*this);
            }
        };
    }

    class error_condition:
        public detail::_error_condition_compare
    {
    private:
        int _value;
        const error_category *_cat;

        typedef void(*unspecified_bool_type)();
        static void unspecified_bool_true() {}

    public:
        error_condition() _STDEX_NOEXCEPT_FUNCTION: 
            _value(0), 
            _cat(&generic_category()) 
        {}

        error_condition(int val, const error_category &cat) _STDEX_NOEXCEPT_FUNCTION: 
            _value(val), 
            _cat(&cat) 
        {}

        template<class _ErrorCondEnum>
        error_condition(const _ErrorCondEnum& val) _STDEX_NOEXCEPT_FUNCTION
        {
            STATIC_ASSERT(bool(is_error_condition_enum<_ErrorCondEnum>::value == true), value_should_be_error_condition_enum);
            *this = val;
        }

        void assign(int val, const error_category &cat) _STDEX_NOEXCEPT_FUNCTION
        {
            _value = val;
            _cat = &cat;
        }

        template<class _ErrorCondEnum>
        typename enable_if<detail::_or_<is_error_condition_enum<_ErrorCondEnum>, is_same<error_condition, _ErrorCondEnum> >::value, error_condition&>::type
            operator=(const _ErrorCondEnum& val) _STDEX_NOEXCEPT_FUNCTION
        {   
            return (*this = _stdex_ADL::_make_error_condition(val));
        }

        void clear() _STDEX_NOEXCEPT_FUNCTION
        {
            assign(0, generic_category());
        }

        int value() const _STDEX_NOEXCEPT_FUNCTION { return _value; }

        const error_category& category() const _STDEX_NOEXCEPT_FUNCTION { return *_cat; }

        
        std::string message() const
        {
            return category().message(value());
        }

        operator unspecified_bool_type() const _STDEX_NOEXCEPT_FUNCTION  // true if error
        {
            return _value == 0 ? 0 : unspecified_bool_true;
        }

        bool operator!() const _STDEX_NOEXCEPT_FUNCTION  // true if no error
        {
            return _value == 0;
        }
    };

    namespace detail
    {
        struct _error_code_compare
        {
            operator
            const stdex::error_code&() const {
                return reinterpret_cast<const error_code&>(*this);
            }
        };
    }

    class error_code:
        public detail::_error_code_compare
    {
    private:

        int _value;
        const error_category *_cat;

        typedef void(*unspecified_bool_type)();
        static void unspecified_bool_true() {}

    public:
        error_code() _STDEX_NOEXCEPT_FUNCTION: 
            _value(0), 
            _cat(&system_category()) 
        { }

        error_code(int val, const error_category& cat) _STDEX_NOEXCEPT_FUNCTION: 
            _value(val), 
            _cat(&cat) 
        { }

        template<class _ErrorCodeEnum>
        error_code(const _ErrorCodeEnum &val) _STDEX_NOEXCEPT_FUNCTION
        {
            STATIC_ASSERT(bool(is_error_code_enum<_ErrorCodeEnum>::value == true), value_should_be_error_code_enum);
            *this = val;
        }

        void assign(int val, const error_category& cat) _STDEX_NOEXCEPT_FUNCTION
        {
            _value = val;
            _cat = &cat;
        }

        void clear() _STDEX_NOEXCEPT_FUNCTION
        {
            assign(0, system_category());
        }

        template<class _ErrorCodeEnum>
        typename enable_if<detail::_or_<is_error_code_enum<_ErrorCodeEnum>, is_same<error_code, _ErrorCodeEnum> >::value, error_code&>::type
            operator=(const _ErrorCodeEnum& val) _STDEX_NOEXCEPT_FUNCTION
        {
            return (*this = _stdex_ADL::_make_error_code(val));
        }

        int value() const _STDEX_NOEXCEPT_FUNCTION { return _value; }

        const error_category& category() const _STDEX_NOEXCEPT_FUNCTION { return *_cat; }

        error_condition default_error_condition() const _STDEX_NOEXCEPT_FUNCTION;

        std::string message() const
        {
            return category().message(value());
        }

        operator unspecified_bool_type() const  _STDEX_NOEXCEPT_FUNCTION // true if error
        {
            return _value == 0 ? 0 : unspecified_bool_true;
        }

        bool operator!() const  _STDEX_NOEXCEPT_FUNCTION // true if no error
        {
            return _value == 0;
        }
    };

    class system_error : 
        public std::runtime_error
    {
    private:
        error_code     _code;

    public:
        system_error(error_code ecode = error_code()): 
            std::runtime_error(ecode.message()), 
            _code(ecode)
        {}

        system_error(error_code ecode, const std::string &what): 
            std::runtime_error(what + ": " + ecode.message()),
            _code(ecode)
        {}

        system_error(error_code ecode, const char *what): 
            std::runtime_error(std::string(what) + (": " + ecode.message())),
            _code(ecode)
        {}

        system_error(int val, const error_category &ecat, const char *what): 
            std::runtime_error(std::string(what) + (": " + error_code(val, ecat).message())),
            _code(val, ecat)
        {}

        system_error(int val, const error_category &ecat): 
            std::runtime_error(error_code(val, ecat).message()),
            _code(val, ecat) 
        {}

        system_error(int val, const error_category &ecat, const std::string &what): 
            std::runtime_error(what + ": " + error_code(val, ecat).message()),
            _code(val, ecat) 
        {}

        const error_code& code() const _STDEX_NOEXCEPT_FUNCTION { return _code; }
    };

    namespace detail
    {
        template<class _Tp>
        struct _is_error_code
        {
            static const bool value =
                is_same<error_code,
                    typename remove_reference<
                        typename remove_cv<_Tp>::type>::type>::value == bool(true) ||
                is_same<detail::_error_code_compare,
                    typename remove_reference<
                        typename remove_cv<_Tp>::type>::type>::value == bool(true);
        };

        template<class _Tp>
        struct _is_not_error_code:
            _not_<_is_error_code<_Tp>/**/>
        { };

        template<class _Tp>
        struct _is_error_condition
        {
            static const bool value =
                is_same<error_condition,
                    typename remove_reference<
                        typename remove_cv<_Tp>::type>::type>::value == bool(true) ||
                is_same<detail::_error_condition_compare,
                    typename remove_reference<
                        typename remove_cv<_Tp>::type>::type>::value == bool(true);
        };

        template<class _Tp>
        struct _is_not_error_condition :
            _not_<_is_error_condition<_Tp>/**/>
        { };

        template<class _Tp>
        struct _is_error_code_enum_or_class:
                _or_<
                    is_error_code_enum<_Tp>, 
                    _is_error_code<_Tp> 
                >
        { };

        template<class _Tp>
        struct _is_error_code_enum_not_class:
                _and_<
                    is_error_code_enum<_Tp>, 
                    _is_not_error_code<_Tp> 
                >
        { };

        template<class _Tp>
        struct _is_error_condition_enum_or_class:
                _or_<
                    is_error_condition_enum<_Tp>,
                    _is_error_condition<_Tp>
                >
        { };

        template<class _Tp>
        struct _is_error_condition_enum_not_class:
                _and_<
                    is_error_condition_enum<_Tp>,
                    _is_not_error_condition<_Tp>
                >
        { };

        template<int>
        class _disabled_system_error_operator;
    }

    template<class _LhsT>
    inline // overload for enum values at left and classes
    bool operator<(
        const _LhsT& _lhs,
        typename conditional<
            bool( detail::_is_error_code_enum_or_class<_LhsT>::value == bool(true) ),
            const detail::_error_code_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        struct lambdas
        {
            static 
            bool compare(
                const error_code& _lhs, const error_code& _rhs) _STDEX_NOEXCEPT_FUNCTION
            {
                return (_lhs.category() < _rhs.category()
                    || (_lhs.category() == _rhs.category()
                        && _lhs.value() < _rhs.value()));
            }
        };
        return lambdas::compare(_lhs, _rhs);
    }

    template<class _RhsT>
    inline // overload for enum values at right
    bool operator<(
        typename conditional<
            bool( detail::_is_error_code_enum_not_class<_RhsT>::value == bool(true) ),
            const detail::_error_code_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _lhs,
        const _RhsT &_rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return stdex::operator<(_lhs, error_code(_rhs));
    }

    template<class _LhsT>
    inline // overload for enum values at left and classes
    bool operator<(
        const _LhsT& _lhs,
        typename conditional<
            bool( detail::_is_error_condition_enum_or_class<_LhsT>::value == bool(true) ),
            const detail::_error_condition_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        struct lambdas
        {
            static 
            bool compare(
                const error_condition& _lhs, const error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
            {
                return (_lhs.category() < _rhs.category()
                    || (_lhs.category() == _rhs.category()
                        && _lhs.value() < _rhs.value()));
            }
        };
        return lambdas::compare(_lhs, _rhs);
    }

    template<class _RhsT>
    inline // overload for enum values at right
    bool operator<(
        typename conditional<
            bool( detail::_is_error_condition_enum_not_class<_RhsT>::value == bool(true) ),
            const detail::_error_condition_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _lhs,
        const _RhsT &_rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return stdex::operator<(_lhs, error_condition(_rhs));
    }

    /*inline
    bool operator<(const stdex::error_condition& _lhs, const stdex::error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        if (_lhs.category() < _rhs.category())
            return true;
        if (_lhs.value() < _rhs.value())
            return true;
        return false;
    }*/

    // OPERATOR== FOR error_code/error_condition
    template<class _LhsT>
    inline
    bool operator==(
        const _LhsT& _lhs,
        typename conditional<
            bool( detail::_is_error_code_enum_or_class<_LhsT>::value == bool(true) ),
            const detail::_error_code_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        struct lambdas
        {
            static 
            bool compare(
                const error_code& _lhs, const error_code& _rhs) _STDEX_NOEXCEPT_FUNCTION
            {
                return (_lhs.category() == _rhs.category()
                    && _lhs.value() == _rhs.value());
            }
        };
        return lambdas::compare(_lhs, _rhs);
    }

    template<class _RhsT>
    inline
    bool operator==(
        typename conditional<
            bool( detail::_is_error_code_enum_not_class<_RhsT>::value == bool(true) ),
            const detail::_error_code_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _lhs,
        const _RhsT &_rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return stdex::operator==(_lhs, error_code(_rhs));
    }

    template<class _LhsT>
    inline
    bool operator==(
        const _LhsT& _lhs,
        typename conditional<
            bool( detail::_is_error_code_enum_or_class<_LhsT>::value == bool(true) ),
            const detail::_error_condition_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _rhs) _STDEX_NOEXCEPT_FUNCTION
    {    // test errors for equality
        struct lambdas
        {
            static 
            bool compare(
                const error_code& _lhs, const error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
            {
                return (_lhs.category().equivalent(_lhs.value(), _rhs)
                    || _rhs.category().equivalent(_lhs, _rhs.value()));
            }
        };
        return lambdas::compare(_lhs, _rhs);
    }

    template<class _RhsT>
    inline
    bool operator==(
        typename conditional<
            bool( detail::_is_error_condition_enum_not_class<_RhsT>::value == bool(true) ),
            const detail::_error_code_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _lhs,
        const _RhsT &_rhs) _STDEX_NOEXCEPT_FUNCTION
    {    // test errors for equality
        return stdex::operator==(_lhs, error_condition(_rhs));
    }

    template<class _LhsT>
    inline
    bool operator==(
        const _LhsT& _lhs,
        typename conditional<
            bool( detail::_is_error_condition_enum_or_class<_LhsT>::value == bool(true) ),
            const detail::_error_code_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _rhs) _STDEX_NOEXCEPT_FUNCTION
    {    // test errors for equality
        struct lambdas
        {
            static 
            bool compare(
                const error_condition& _lhs, const error_code& _rhs) _STDEX_NOEXCEPT_FUNCTION
            {
                return (_rhs.category().equivalent(_rhs.value(), _lhs)
                    || _lhs.category().equivalent(_rhs, _lhs.value()));
            }
        };
        return lambdas::compare(_lhs, _rhs);
    }

    template<class _RhsT>
    inline
    bool operator==(
        typename conditional<
            bool( detail::_is_error_code_enum_not_class<_RhsT>::value == bool(true) ),
            const detail::_error_condition_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _lhs,
        const _RhsT &_rhs) _STDEX_NOEXCEPT_FUNCTION
    {    // test errors for equality
        return stdex::operator==(_lhs, error_code(_rhs));
    }



    template<class _LhsT>
    inline
    bool operator==(
        const _LhsT& _lhs,
        typename conditional<
            bool( detail::_is_error_condition_enum_or_class<_LhsT>::value == bool(true) ),
            const detail::_error_condition_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        struct lambdas
        {
            static 
            bool compare(
                const error_condition& _lhs, const error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
            {
                return (_lhs.category() == _rhs.category()
                    && _lhs.value() == _rhs.value());
            }
        };
        return lambdas::compare(_lhs, _rhs);
    }

    template<class _RhsT>
    inline
    bool operator==(
        typename conditional<
            bool( detail::_is_error_condition_enum_not_class<_RhsT>::value == bool(true) ),
            const detail::_error_condition_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _lhs,
        const _RhsT &_rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return stdex::operator==(_lhs, error_condition(_rhs));
    }

    //inline
    //bool operator==(const stdex::error_condition& _lhs, const stdex::error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
    //{
    //    return _lhs.category() == _rhs.category() && _lhs.value() == _rhs.value();
    //}

    // OPERATOR!= FOR error_code/error_condition
    template<class _LhsT>
    inline
    bool operator!=(
        const _LhsT& _lhs,
        typename conditional<
            bool( detail::_is_error_code_enum_or_class<_LhsT>::value == bool(true) ),
            const detail::_error_code_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return !(_lhs == _rhs);
    }

    template<class _RhsT>
    inline
    bool operator!=(
        typename conditional<
            bool( detail::_is_error_code_enum_not_class<_RhsT>::value == bool(true) ),
            const detail::_error_code_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _lhs,
        const _RhsT &_rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return !(_lhs == _rhs);
    }

    template<class _LhsT>
    inline
    bool operator!=(
        const _LhsT& _lhs,
        typename conditional<
            bool( detail::_is_error_code_enum_or_class<_LhsT>::value == bool(true) ),
            const detail::_error_condition_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return !(_lhs == _rhs);
    }

    template<class _RhsT>
    inline
    bool operator!=(
        typename conditional<
            bool( detail::_is_error_condition_enum_not_class<_RhsT>::value == bool(true) ),
            const detail::_error_code_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _lhs,
        const _RhsT &_rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return !(_lhs == _rhs);
    }

    template<class _LhsT>
    inline
    bool operator!=(
        const _LhsT& _lhs,
        typename conditional<
            bool( detail::_is_error_condition_enum_or_class<_LhsT>::value == bool(true) ),
            const detail::_error_code_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return !(_lhs == _rhs);
    }

    template<class _RhsT>
    inline
    bool operator!=(
        typename conditional<
            bool( detail::_is_error_code_enum_not_class<_RhsT>::value == bool(true) ),
            const detail::_error_condition_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _lhs,
        const _RhsT &_rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return !(_lhs == _rhs);
    }

    template<class _LhsT>
    inline
    bool operator!=(
        const _LhsT& _lhs,
        typename conditional<
            bool( detail::_is_error_condition_enum_or_class<_LhsT>::value == bool(true) ),
            const detail::_error_condition_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return !(_lhs == _rhs);
    }

    template<class _RhsT>
    inline
    bool operator!=(
        typename conditional<
            bool( detail::_is_error_condition_enum_not_class<_RhsT>::value == bool(true) ),
            const detail::_error_condition_compare&,
            detail::_disabled_system_error_operator<__LINE__>/**/>::type _lhs,
        const _RhsT &_rhs) _STDEX_NOEXCEPT_FUNCTION
    {
        return !(_lhs == _rhs);
    }

    //inline
    //bool operator!=(const stdex::error_condition& _lhs, const stdex::error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
    //{
    //    return !(_lhs == _rhs);
    //}

    
    // VIRTUALS FOR error_category
    inline error_condition
        error_category::default_error_condition(int _Errval) const _STDEX_NOEXCEPT_FUNCTION
    {    // make error_condition for error code
        return (error_condition(_Errval, *this));
    }

    inline bool
        error_category::equivalent(int _Errval,
            const error_condition& _Cond) const _STDEX_NOEXCEPT_FUNCTION
    {    // test if error code same condition
        return (default_error_condition(_Errval) == _Cond);
    }

    inline bool
        error_category::equivalent(const error_code& _Code,
            int _Errval) const _STDEX_NOEXCEPT_FUNCTION
    {    // test if conditions same for this category
        return (*this == _Code.category() && _Code.value() == _Errval);
    }

    // MEMBER FUNCTIONS for error_code
    inline error_condition error_code::default_error_condition() const _STDEX_NOEXCEPT_FUNCTION
    {    // make error_condition for error code
        return (category().default_error_condition(value()));
    }

    // FUNCTION make_error_code
    inline error_code make_error_code(errc::type _Errno) _STDEX_NOEXCEPT_FUNCTION
    {    // make an error_code
        return (error_code((int) _Errno, generic_category()));
    }

    inline error_code make_error_code(io_errc::type _Errno) _STDEX_NOEXCEPT_FUNCTION
    {    // make an error_code
        return (error_code((int) _Errno, iostream_category()));
    }

    // FUNCTION make_error_condition
    inline error_condition make_error_condition(errc::type _Errno) _STDEX_NOEXCEPT_FUNCTION
    {    // make an error_condition
        return (error_condition((int) _Errno, generic_category()));
    }

    inline error_condition make_error_condition(io_errc::type _Errno) _STDEX_NOEXCEPT_FUNCTION
    {    // make an error_condition
        return (error_condition((int) _Errno, iostream_category()));
    }

    namespace detail
    {
        namespace system_error_detail
        {
            template<bool, class>
            struct _has_conforming_strerror_s;
            struct _has_nonconforming_strerror_s;
            struct _has_conforming_strerrorlen_s;

            class _strerror_s_arg;

            class _strerrorlen_s_arg;
        } // namespace system_error_detail
    } // namespace detail
} // namespace stdex


// we need all this mess at global namespace
// to detect if following C11-functions are defined or not
stdex::detail::system_error_detail::_strerror_s_arg 
strerror_s(
    stdex::detail::system_error_detail::_strerror_s_arg, 
    stdex::detail::system_error_detail::_strerror_s_arg
);
stdex::detail::system_error_detail::_strerror_s_arg 
strerror_s(
    stdex::detail::system_error_detail::_strerror_s_arg, 
    stdex::detail::system_error_detail::_strerror_s_arg,
    stdex::detail::system_error_detail::_strerror_s_arg
);
stdex::detail::system_error_detail::_strerrorlen_s_arg
strerrorlen_s(
    stdex::detail::system_error_detail::_strerrorlen_s_arg
);

namespace stdex 
{
    namespace detail
    {
        namespace system_error_detail
        {

            static const char* _unknown_error()
            {return "unknown error";}

            template<bool, bool, class _DummyT>
            struct strerror_impl_helper
            {
                static std::string call(_DummyT _Errcode)
                {
                    using namespace std;

                    const char *result = 
                        strerror(_Errcode); // if you are there U need to disable _CRT_SECURE_NO_WARNINGS bullshit
                    
                    return result ? result : "";
                }
            };

            template<class _DummyT>
            struct strerror_impl_helper<true, true, _DummyT>
            {
                static std::string call(_DummyT _Errcode)
                {
                    using namespace std;
                    
                    std::string result;

                    size_t len = strerrorlen_s(_Errcode);
                    if (len)
                    {
                        struct _RAII{
                            char *buf;
                            _RAII() : buf(0) {}
                            ~_RAII() {delete [] buf;}
                        } _tmp;

                        _tmp.buf = new char[len + 1];
                        if(0 == strerror_s(_tmp.buf, len + 1, _Errcode))
                            result = _tmp.buf;
                    }
                    return result;
                }
            };

            template<class _DummyT>
            struct strerror_impl_helper<true, false, _DummyT>
            { // MS specific
                static std::string call(_DummyT _Errcode)
                {                    
                    std::string result;

                    char buf[2048] = {0};

                    if(0 == strerror_s(buf, _Errcode))
                        result = buf;
                    return result;
                }
            };

            class _strerror_s_arg
            {
                typedef int return_type; // would be errno_t fo C
                char buf[sizeof(return_type) * 14];
                _strerror_s_arg(...);
                operator return_type();

                template<bool, class>
                friend struct _has_conforming_strerror_s;
                friend struct _has_nonconforming_strerror_s;
            };

            class _strerrorlen_s_arg
            {
                typedef std::size_t return_type;
                char buf[sizeof(return_type) * 12];
                _strerrorlen_s_arg(...);
                operator return_type();

                friend struct _has_conforming_strerrorlen_s;
            };

            struct _has_conforming_strerrorlen_s
            {
                static const bool value =
                    sizeof(
                        strerrorlen_s(*_declptr<int>())
                    ) != sizeof(_strerrorlen_s_arg);
            };
                
            template<bool, class _T>
            struct _has_conforming_strerror_s
            {
                static const bool value =
                    sizeof(
                        strerror_s(_declptr<_T>(), *_declptr<std::size_t>(), *_declptr<int>())
                    ) != sizeof(_strerror_s_arg);
            };

            template<class _T>
            struct _has_conforming_strerror_s<true, _T>
                : stdex::false_type {};

            struct _has_nonconforming_strerror_s
            {
                static const bool value =
                    sizeof(
                        strerror_s(_declptr<_strerror_s_arg>()->buf, *_declptr<int>())
                    ) != sizeof(_strerror_s_arg);
            };

            typedef 
            stdex::bool_constant<
                    _has_conforming_strerror_s<_has_nonconforming_strerror_s::value, char>::value ||
                    _has_nonconforming_strerror_s::value
            > _has_strerror_s;

            struct strerror_impl:
                strerror_impl_helper<_has_strerror_s::value, _has_conforming_strerrorlen_s::value, int>
            { };

            static inline std::string _strerror(int _errnum)
            {
                std::string result =
                    strerror_impl::call(_errnum);
                
                if(result.empty())
                    return _unknown_error();
                return result;
            }

            static inline bool _is_valid_errnum(int _errnum)
            {
                return 
                    !strerror_impl::call(_errnum).empty();
            }
        }

        using system_error_detail::_strerror;
        using system_error_detail::_is_valid_errnum;
        

        class _generic_error_category
            : public error_category
        {    // categorize a generic error
        public:
            _generic_error_category()
            {    // default constructor
            }

            virtual const char *name() const _STDEX_NOEXCEPT_FUNCTION
            {    // get name of category
                return ("generic");
            }

            virtual std::string message(int _Errcode) const
            {    // convert to name of error
                return _strerror(_Errcode);
            }
        };

        class _io_stream_error_category
            : public _generic_error_category
        {    // categorize a Posix error
        public:
            _io_stream_error_category()
            {    // default constructor
            }

            virtual const char *name() const _STDEX_NOEXCEPT_FUNCTION
            {    // get name of category
                return ("iostream");
            }

            virtual std::string message(int _Errcode) const
            {    // convert to name of error
                if (_Errcode == (int) io_errc::stream)
                    return ("iostream stream error");
                else
                    return (_generic_error_category::message(_Errcode));
            }
        };

        class _system_error_category
            : public _generic_error_category
        {    // categorize a generic error
        public:
            _system_error_category()
            {    // default constructor
            }

            virtual const char *name() const _STDEX_NOEXCEPT_FUNCTION
            {    // get name of category
                return ("system");
            }

            virtual std::string message(int _Errcode) const
            {    // convert to name of error
                return _strerror(_Errcode);
            }

            virtual error_condition
                default_error_condition(int _Errval) const _STDEX_NOEXCEPT_FUNCTION
            {    // make error_condition for error code (generic if possible)
                if (_is_valid_errnum(_Errval))
                    return (error_condition(_Errval, generic_category()));
                else
                    return (error_condition(_Errval, system_category()));
            }
        };

        template<class _Cat>
        struct _error_objects
        {    // wraps category objects
            static _Cat &_object()
            {
                static _Cat &_obj = *(new _Cat());
                return _obj;
            }
        };
    }

    inline const error_category& generic_category() _STDEX_NOEXCEPT_FUNCTION
    {    // get generic_category
        return (detail::_error_objects<detail::_generic_error_category>::_object());
    }

    inline const error_category& iostream_category() _STDEX_NOEXCEPT_FUNCTION
    {    // get iostream_category
        return (detail::_error_objects<detail::_io_stream_error_category>::_object());
    }

    inline const error_category& system_category() _STDEX_NOEXCEPT_FUNCTION
    {    // get system_category
        return (detail::_error_objects<detail::_system_error_category>::_object());
    }

} // namespace stdex

namespace _stdex_ADL
{
    template<class _Tp>
    inline
    static stdex::error_code _make_error_code(_Tp _val) 
    {
        return make_error_code(_val);
    }

    template<class _Tp>
    inline
    static stdex::error_condition _make_error_condition(_Tp _val) 
    {
        return make_error_condition(_val);
    }
} // namespace _stdex_ADL

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_SYSTEM_ERROR_H
