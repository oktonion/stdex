#ifndef _STDEX_NULLPTR_H
#define _STDEX_NULLPTR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdint.h>
#include <climits>

namespace stdex
{

	/*class nullptr_t
	{
	private:
		void *_padding;
		struct _nat { int _for_bool_; };

		template<typename T>
		struct identity { typedef T type; };

	public:
		nullptr_t() : _padding(0) { }
		nullptr_t(int _nat::*) : _padding(0) { }

		//operator int _nat::*() const { return 0; }

		template<class T>
		inline operator T*() const // convertible to any type of null non-member pointer...
		{
			return 0;
		}

		template<class T, class C>
		inline operator T C::*() const   // or any type of null member pointer...
		{
			return 0;
		}

		template<typename T>
		inline bool operator==(T *val) const { return equal(val); }

		template<typename T>
		inline bool operator!=(T *val) const { return !equal(val); }

		template<typename T>
		inline bool operator<(T *val) const { return less(val); }

		template<typename T>
		inline bool operator<=(T *val) const { return less_equal(val); }

		template<typename T>
		inline bool operator>(T *val) const { return false; }//nullptr always less or equal (can't be greater than)

		template<typename T>
		inline bool operator>=(T *val) const { return greater_equal(val); }

		inline bool operator==(bool val) const { return false == val; }

		inline bool operator!=(bool val) const { return false != val; }

		//friends:
		template<typename T>
		friend inline bool operator==(T *val, nullptr_t np) { return equal(val, np); }

		template<typename T>
		friend inline bool operator!=(T *val, nullptr_t np) { return !equal(val, np); }

		template<typename T>
		friend inline bool operator<(T *val, nullptr_t np) { return false; }//value is always more or equal (can't be less than)

		template<typename T>
		friend inline bool operator<=(T *val, nullptr_t np) { return less_equal(val, np); }

		template<typename T>
		friend inline bool operator>(T *val, nullptr_t np) { return greater(val, np); }

		template<typename T>
		friend inline bool operator>=(T *val, nullptr_t np) { return greater_equal(val, np); }

		friend inline bool operator==(bool val, nullptr_t np) { return false == val; }

		friend inline bool operator!=(bool val, nullptr_t np) { return false != val; }


	private: //template overloads


		template<typename T>
		inline bool equal(T *val) { return val == static_cast<T*>(*this); }

		inline bool equal(nullptr_t) { return true; }

		template<typename T>
		inline bool less(T *val) { return static_cast<T*>(*this) < val; }

		inline bool less(nullptr_t) { return false; }

		template<typename T>
		inline bool less_equal(T *val) { return  static_cast<T*>(*this) <= val; }

		inline bool less_equal(nullptr_t) { return true; }

		template<typename T>
		inline bool greater_equal(T *val) { return static_cast<T*>(*this) >= val; }

		inline bool greater_equal(nullptr_t) { return true; }

		//friends:
		template<typename T>
		static inline bool equal(T *val, nullptr_t np) { return val == static_cast<T*>(np); }

		static inline bool equal(nullptr_t, nullptr_t np) { return true; }

		template<typename T>
		static inline bool less_equal(T *val, nullptr_t np) { return val <= static_cast<T*>(np); }

		static inline bool less_equal(nullptr_t, nullptr_t) { return true; }

		template<typename T>
		static inline bool greater(T *val, nullptr_t np) { return val > static_cast<T*>(np); }

		static inline bool greater(nullptr_t, nullptr_t) { return false; }

		template<typename T>
		static inline bool greater_equal(T *val, nullptr_t np) { return val >= static_cast<T*>(np); }

		static inline bool greater_equal(nullptr_t, nullptr_t) { return true; }
	};*/

	namespace detail
	{
		namespace nullptr_detail
		{
			#ifdef LLONG_MAX
				typedef ::uintmax_t uintmax_t;
			#else
				typedef uint32_t uintmax_t;
			#endif

			typedef char _yes_type;
			struct _no_type
			{
				char padding[8];
			};

			_no_type _is_convertable_to_void_ptr(...);
			_yes_type _is_convertable_to_void_ptr(void*);

			_no_type _is_convertable_to_any_ptr(...);
			template<class _T>
			_yes_type _is_convertable_to_any_ptr(_T*);

			template<int> struct sfinae_true
			{
				typedef _yes_type type;
			};

			#ifdef NULL
			template<class T>
			typename sfinae_true<(T(NULL), 0)>::type _nullptr_can_be_ct_constant(int);
			#else
			template<class T>
			typename sfinae_true<(T(0), 0)>::type _nullptr_can_be_ct_constant(int);
			#endif
			template<class>
			_no_type _nullptr_can_be_ct_constant(...);

			enum nullptr_t_as_enum
			{
				#ifdef NULL
				__nullptr_val = NULL,
				#else
				__nullptr_val = 0,
				#endif
				__max_nullptr = uintmax_t(1) << (CHAR_BIT * sizeof(void*) - 1)
			};

			class nullptr_t_as_class_impl {
			public:
				// Required in order to create const nullptr_t objects without an
				// explicit initializer in GCC 4.5, a la:
				//
				// const std::nullptr_t nullptr;
				nullptr_t_as_class_impl() { }
				nullptr_t_as_class_impl(int) { }

				// Make nullptr convertible to any pointer type.
				template<typename T> operator T*() const { return 0; }
				// Make nullptr convertible to any member pointer type.
				template<typename C, typename T> operator T C::*() { return 0; }
				bool operator==(nullptr_t_as_class_impl) const { return true; }
				bool operator!=(nullptr_t_as_class_impl) const { return false; }
			private:
				// Do not allow taking the address of nullptr.
				void operator&();

				void *_padding;
			};

			class nullptr_t_as_class_impl1 {
			public:
				// Required in order to create const nullptr_t objects without an
				// explicit initializer in GCC 4.5, a la:
				//
				// const std::nullptr_t nullptr;
				nullptr_t_as_class_impl1() { }
				nullptr_t_as_class_impl1(int) { }

				// Make nullptr convertible to any pointer type.
				template<typename T> operator T*() const { return 0; }
				bool operator==(nullptr_t_as_class_impl1) const { return true; }
				bool operator!=(nullptr_t_as_class_impl1) const { return false; }
			private:
				// Do not allow taking the address of nullptr.
				void operator&();

				void *_padding;
			};

			typedef int nullptr_t_as_int;

			typedef void* nullptr_t_as_void;
		}

		template<class T>
		struct _nullptr_can_be_ct_constant_impl
		{
			static const bool value = (sizeof(nullptr_detail::_nullptr_can_be_ct_constant<T>(0)) == sizeof(nullptr_detail::_yes_type));
		};

		template<class _T>
		struct _is_convertable_to_void_ptr_impl
		{
			static const _T *p;
			static const bool value = (sizeof(nullptr_detail::_is_convertable_to_void_ptr(*_is_convertable_to_void_ptr_impl::p)) == sizeof(nullptr_detail::_yes_type));
		};

		template<class _T>
		struct _is_convertable_to_any_ptr_impl
		{
			static const _T *p;
			static const bool value = (sizeof(nullptr_detail::_is_convertable_to_any_ptr(*_is_convertable_to_any_ptr_impl::p)) == sizeof(nullptr_detail::_yes_type));
		};

		template<>
		struct _is_convertable_to_void_ptr_impl<nullptr_detail::nullptr_t_as_int>
		{
			#ifdef NULL
			static const bool value = (sizeof(nullptr_detail::_is_convertable_to_void_ptr(NULL)) == sizeof(nullptr_detail::_yes_type));
			#else
			static const bool value = (sizeof(nullptr_detail::_is_convertable_to_void_ptr(0)) == sizeof(nullptr_detail::_yes_type));
			#endif
		};

		template<>
		struct _is_convertable_to_any_ptr_impl<nullptr_detail::nullptr_t_as_int>
		{
			#ifdef NULL
			static const bool value = (sizeof(nullptr_detail::_is_convertable_to_any_ptr(NULL)) == sizeof(nullptr_detail::_yes_type));
			#else
			static const bool value = (sizeof(nullptr_detail::_is_convertable_to_any_ptr(0)) == sizeof(nullptr_detail::_yes_type));
			#endif
		};

		template<class _T>
		struct _is_convertable_to_ptr_impl
		{
			static const bool value = (_is_convertable_to_void_ptr_impl<_T>::value == bool(true) && _is_convertable_to_any_ptr_impl<_T>::value == bool(true));
		};

		template<class _T>
		struct _is_equal_size_to_void_ptr
		{
			static const bool value = (sizeof(_T) == sizeof(void*));
		};

		struct _member_ptr_is_same_as_ptr
		{
			struct test {};
			typedef void(test::*member_ptr_type)(void);
			static const bool value = _is_convertable_to_void_ptr_impl<member_ptr_type>::value;
		};

		template<bool>
		struct _nullptr_t_as_class_chooser
		{
			typedef nullptr_detail::nullptr_t_as_class_impl type;
		};

		template<>
		struct _nullptr_t_as_class_chooser<false>
		{
			typedef nullptr_detail::nullptr_t_as_class_impl1 type;
		};

		template<bool>
		struct _nullptr_choose_as_int
		{
			typedef nullptr_detail::nullptr_t_as_int type;
		};

		template<bool>
		struct _nullptr_choose_as_enum
		{
			typedef nullptr_detail::nullptr_t_as_enum type;
		};

		template<bool>
		struct _nullptr_choose_as_class
		{
			typedef _nullptr_t_as_class_chooser<_member_ptr_is_same_as_ptr::value>::type type;
		};

		template<>
		struct _nullptr_choose_as_int<false>
		{
			typedef nullptr_detail::nullptr_t_as_void type;
		};

		template<>
		struct _nullptr_choose_as_enum<false>
		{
			struct as_int
			{
				typedef nullptr_detail::nullptr_t_as_int nullptr_t_as_int;

				static const bool _is_convertable_to_ptr = _is_convertable_to_ptr_impl<nullptr_t_as_int>::value;
				static const bool _equal_void_ptr = _is_equal_size_to_void_ptr<nullptr_t_as_int>::value;
			};

			typedef _nullptr_choose_as_int<as_int::_is_convertable_to_ptr == bool(true) && as_int::_equal_void_ptr == bool(true)>::type type;
		};

		template<>
		struct _nullptr_choose_as_class<false>
		{
			struct as_enum
			{
				typedef nullptr_detail::nullptr_t_as_enum nullptr_t_as_enum;

				static const bool _is_convertable_to_ptr = _is_convertable_to_ptr_impl<nullptr_t_as_enum>::value;
				static const bool _equal_void_ptr = _is_equal_size_to_void_ptr<nullptr_t_as_enum>::value;
				static const bool _can_be_ct_constant = _nullptr_can_be_ct_constant_impl<nullptr_t_as_enum>::value;
			};

			typedef _nullptr_choose_as_enum<as_enum::_is_convertable_to_ptr == bool(true) && as_enum::_equal_void_ptr == bool(true) && as_enum::_can_be_ct_constant == bool(true)>::type type;
		};

		struct _nullptr_chooser
		{


			struct as_class
			{
				typedef _nullptr_t_as_class_chooser<_member_ptr_is_same_as_ptr::value>::type nullptr_t_as_class;

				static const bool _equal_void_ptr = _is_equal_size_to_void_ptr<nullptr_t_as_class>::value;
				static const bool _can_be_ct_constant = _nullptr_can_be_ct_constant_impl<nullptr_t_as_class>::value;
			};

			typedef _nullptr_choose_as_class<as_class::_equal_void_ptr == bool(true) && as_class::_can_be_ct_constant == bool(true)>::type type;
		};

		template<class T>
		struct _nullptr_is_same_as
		{
			static const bool value = false;
		};

		template<>
		struct _nullptr_is_same_as<_nullptr_chooser::type>
		{
			static const bool value = true;
		};
	}

typedef detail::_nullptr_chooser::type nullptr_t;

}


#ifdef NULL
	#define nullptr stdex::detail::_nullptr_is_same_as<stdex::detail::nullptr_detail::nullptr_t_as_enum>::value ? \
					stdex::detail::nullptr_detail::__nullptr_val : \
					stdex::detail::_nullptr_is_same_as<stdex::detail::nullptr_detail::nullptr_t_as_int>::value ? \
					NULL : \
					(stdex::nullptr_t)(NULL)
#else
	#define nullptr stdex::detail::_nullptr_is_same_as<stdex::detail::nullptr_detail::nullptr_t_as_enum>::value ? \
					stdex::detail::nullptr_detail::__nullptr_val : \
					stdex::detail::_nullptr_is_same_as<stdex::detail::nullptr_detail::nullptr_t_as_int>::value ? \
					0 : \
					(stdex::nullptr_t)(0)
#endif

//#define nullptr stdex::detail::_nullptr_chooser::value


#endif // _STDEX_NULLPTR_H
