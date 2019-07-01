#ifndef _STDEX_SYSTEM_ERROR_H
#define _STDEX_SYSTEM_ERROR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./type_traits.hpp" // stdex::enable_if

// POSIX includes
/*none*/

// std includes
#include <errno.h>
#include <cerrno>
#include <cstdlib>		// std::strerror
#include <cstring>		// std::strerror
#include <stdexcept>	// std::runtime_error
#include <string> 		// std::string
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
		enum errc_t {	// names for generic error codes
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
		};

		errc(errc_t code):
			_code(code)
		{

		}

		operator errc_t&()
		{
			return _code;
		}

		operator const errc_t&() const
		{
			return _code;
		}

		private:
			errc_t _code;
	};

	typedef errc generic_errno;

	// ENUM io_errc

	enum io_errc {	// error codes for ios_base::failure
		stream = 1
	};

	// TEMPLATE CLASS is_error_code_enum
	template<class _Enum>
	struct is_error_code_enum
	{	// tests for error_code enumeration
		static const bool value = false;
	};

	template<>
	struct is_error_code_enum<io_errc>
	{	// tests for error_code enumeration
		static const bool value = true;
	};

	template<>
	struct is_error_code_enum<errc::errc_t>
	{
		static const bool value = true;
	};

	template<>
	struct is_error_code_enum<errc>
	{
		static const bool value = true;
	};

	// TEMPLATE CLASS is_error_condition_enum
	template<class _Enum>
	struct is_error_condition_enum
	{	// tests for error_condition enumeration
		static const bool value = false;
	};

	template<>
	struct is_error_condition_enum<errc::errc_t>
	{
		static const bool value = true;
	};

	template<>
	struct is_error_condition_enum<generic_errno>
	{	// tests for error_condition enumeration
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

	class error_condition
	{
	private:
		int _value;
		const error_category *_cat;

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
		explicit error_condition(const _ErrorCondEnum& val) _STDEX_NOEXCEPT_FUNCTION
		{
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
			return (*this = make_error_condition(val));
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

		typedef void(*unspecified_bool_type)();
		static void unspecified_bool_true() {}

		operator unspecified_bool_type() const _STDEX_NOEXCEPT_FUNCTION  // true if error
		{
			return _value == 0 ? 0 : unspecified_bool_true;
		}

		bool operator!() const _STDEX_NOEXCEPT_FUNCTION  // true if no error
		{
			return _value == 0;
		}
	};

	class error_code
	{
	private:

		int _value;
		const error_category *_cat;

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
			return (*this = make_error_code(val));
		}

		int value() const _STDEX_NOEXCEPT_FUNCTION { return _value; }

		const error_category& category() const _STDEX_NOEXCEPT_FUNCTION { return *_cat; }

		error_condition default_error_condition() const _STDEX_NOEXCEPT_FUNCTION;

		std::string message() const
		{
			return category().message(value());
		}

		typedef void(*unspecified_bool_type)();
		static void unspecified_bool_true() {}

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
		error_code 	_code;

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

	inline bool
		operator<(const error_code& _lhs, const error_code& _rhs) _STDEX_NOEXCEPT_FUNCTION
	{
		return (_lhs.category() < _rhs.category()
			|| (_lhs.category() == _rhs.category()
				&& _lhs.value() < _rhs.value()));
	}

	inline bool
		operator<(const error_condition& _lhs,
			const error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
	{
		return (_lhs.category() < _rhs.category()
			|| (_lhs.category() == _rhs.category()
				&& _lhs.value() < _rhs.value()));
	}

	// VIRTUALS FOR error_category
	inline error_condition
		error_category::default_error_condition(int _Errval) const _STDEX_NOEXCEPT_FUNCTION
	{	// make error_condition for error code
		return (error_condition(_Errval, *this));
	}

	inline bool
		error_category::equivalent(int _Errval,
			const error_condition& _Cond) const _STDEX_NOEXCEPT_FUNCTION
	{	// test if error code same condition
		return (default_error_condition(_Errval) == _Cond);
	}

	inline bool
		error_category::equivalent(const error_code& _Code,
			int _Errval) const _STDEX_NOEXCEPT_FUNCTION
	{	// test if conditions same for this category
		return (*this == _Code.category() && _Code.value() == _Errval);
	}

	// MEMBER FUNCTIONS for error_code
	inline error_condition error_code::default_error_condition() const _STDEX_NOEXCEPT_FUNCTION
	{	// make error_condition for error code
		return (category().default_error_condition(value()));
	}

	// OPERATOR== FOR error_code/error_condition
	inline bool
		operator==(const error_code& _lhs, const error_code& _rhs) _STDEX_NOEXCEPT_FUNCTION
	{
		return (_lhs.category() == _rhs.category()
			&& _lhs.value() == _rhs.value());
	}

	inline bool operator==(
		const error_code& _lhs,
		const error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
	{	// test errors for equality
		return (_lhs.category().equivalent(_lhs.value(), _rhs)
			|| _rhs.category().equivalent(_lhs, _rhs.value()));
	}

	inline bool operator==(
		const error_condition& _lhs,
		const error_code& _rhs) _STDEX_NOEXCEPT_FUNCTION
	{	// test errors for equality
		return (_rhs.category().equivalent(_rhs.value(), _lhs)
			|| _lhs.category().equivalent(_rhs, _lhs.value()));
	}

	inline bool
		operator==(const error_condition& _lhs,
			const error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
	{
		return (_lhs.category() == _rhs.category()
			&& _lhs.value() == _rhs.value());
	}

	// OPERATOR!= FOR error_code/error_condition
	inline bool
		operator!=(const error_code& _lhs, const error_code& _rhs) _STDEX_NOEXCEPT_FUNCTION
	{
		return (!(_lhs == _rhs));
	}

	inline bool operator!=(
		const error_code& _lhs,
		const error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
	{	// test errors for inequality
		return (!(_lhs == _rhs));
	}

	inline bool operator!=(
		const error_condition& _lhs,
		const error_code& _rhs) _STDEX_NOEXCEPT_FUNCTION
	{	// test errors for inequality
		return (!(_lhs == _rhs));
	}

	inline bool
		operator!=(const error_condition& _lhs,
			const error_condition& _rhs) _STDEX_NOEXCEPT_FUNCTION
	{
		return (!(_lhs == _rhs));
	}

	// FUNCTION make_error_code
	inline error_code make_error_code(errc::errc_t _Errno) _STDEX_NOEXCEPT_FUNCTION
	{	// make an error_code
		return (error_code((int) _Errno, generic_category()));
	}

	inline error_code make_error_code(io_errc _Errno) _STDEX_NOEXCEPT_FUNCTION
	{	// make an error_code
		return (error_code((int) _Errno, iostream_category()));
	}

	// FUNCTION make_error_condition
	inline error_condition make_error_condition(errc::errc_t _Errno) _STDEX_NOEXCEPT_FUNCTION
	{	// make an error_condition
		return (error_condition((int) _Errno, generic_category()));
	}

	inline error_condition make_error_condition(io_errc _Errno) _STDEX_NOEXCEPT_FUNCTION
	{	// make an error_condition
		return (error_condition((int) _Errno, iostream_category()));
	}

	namespace detail
	{
		class _generic_error_category
			: public error_category
		{	// categorize a generic error
		public:
			_generic_error_category()
			{	// default constructor
			}

			virtual const char *name() const _STDEX_NOEXCEPT_FUNCTION
			{	// get name of category
				return ("generic");
			}

			virtual std::string message(int _Errcode) const
			{	// convert to name of error
				const char *_Name = std::strerror(_Errcode);
				return (std::string(_Name != 0 ? _Name : "unknown error"));
			}
		};

		class _io_stream_error_category
			: public _generic_error_category
		{	// categorize a Posix error
		public:
			_io_stream_error_category()
			{	// default constructor
			}

			virtual const char *name() const _STDEX_NOEXCEPT_FUNCTION
			{	// get name of category
				return ("iostream");
			}

			virtual std::string message(int _Errcode) const
			{	// convert to name of error
				if (_Errcode == (int) stream)
					return ("iostream stream error");
				else
					return (_generic_error_category::message(_Errcode));
			}
		};

		class _system_error_category
			: public _generic_error_category
		{	// categorize a generic error
		public:
			_system_error_category()
			{	// default constructor
			}

			virtual const char *name() const _STDEX_NOEXCEPT_FUNCTION
			{	// get name of category
				return ("system");
			}

			virtual std::string message(int _Errcode) const
			{	// convert to name of error
				const char *_Name = std::strerror(_Errcode);
				return (std::string(_Name != 0 ? _Name : "unknown error"));
			}

			virtual error_condition
				default_error_condition(int _Errval) const _STDEX_NOEXCEPT_FUNCTION
			{	// make error_condition for error code (generic if possible)
				if (std::strerror(_Errval))
					return (error_condition(_Errval, generic_category()));
				else
					return (error_condition(_Errval, system_category()));
			}
		};

		template<class _Cat>
		struct _error_objects
		{	// wraps category objects
			_error_objects()
			{	// default constructor
			}

			static _generic_error_category &_generic_object()
			{
				static _generic_error_category _obj;
				return _obj;
			}
			static _io_stream_error_category &_io_stream_object()
			{
				static _io_stream_error_category _obj;
				return _obj;
			}
			static _system_error_category &_system_object()
			{
				static _system_error_category _obj;
				return _obj;
			}
		};
	}

	inline const error_category& generic_category() _STDEX_NOEXCEPT_FUNCTION
	{	// get generic_category
		return (detail::_error_objects<int>::_generic_object());
	}

	inline const error_category& iostream_category() _STDEX_NOEXCEPT_FUNCTION
	{	// get iostream_category
		return (detail::_error_objects<int>::_io_stream_object());
	}

	inline const error_category& system_category() _STDEX_NOEXCEPT_FUNCTION
	{	// get system_category
		return (detail::_error_objects<int>::_system_object());
	}
} // namespace stdex

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_SYSTEM_ERROR_H
