#ifndef _STDEX_NULLPTR_H
#define _STDEX_NULLPTR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cstddef>
#include <climits>

#ifdef NULL
#define STDEX_NULL NULL
#else
#define STDEX_NULL 0
#endif

namespace stdex
{
	namespace detail
	{
		namespace nullptr_detail
		{
			typedef char _yes_type;
			struct _no_type
			{
				char padding[8];
			};

			struct _dummy_class {};

			_yes_type _is_convertable_to_void_ptr_tester(void*);
			_no_type _is_convertable_to_void_ptr_tester(...);

			typedef void(nullptr_detail::_dummy_class::*_dummy_class_f)(int);
			typedef int (nullptr_detail::_dummy_class::*_dummy_class_f_const)(double&) const;

			_yes_type _is_convertable_to_member_function_ptr_tester(_dummy_class_f);
			_no_type _is_convertable_to_member_function_ptr_tester(...);

			_yes_type _is_convertable_to_const_member_function_ptr_tester(_dummy_class_f_const);
			_no_type _is_convertable_to_const_member_function_ptr_tester(...);

			template<class _Tp>
			_yes_type _is_convertable_to_ptr_tester(_Tp*);
			template<class>
			_no_type _is_convertable_to_ptr_tester(...);

			/*template<int> struct sfinae_true
			{
				typedef _yes_type type;
			};

			template<class T>
			typename sfinae_true<((T)(STDEX_NULL) == (T)(STDEX_NULL))>::type _nullptr_can_be_ct_constant(int);
			template<class>
			_no_type _nullptr_can_be_ct_constant(...);*/

			class _nullptr_t_as_class_impl {
			public:
				// Required in order to create const nullptr_t objects without an
				// explicit initializer in GCC 4.5, a la:
				//
				// const std::nullptr_t nullptr;
				_nullptr_t_as_class_impl() { }
				_nullptr_t_as_class_impl(int) { }

				// Make nullptr convertible to any pointer type.
				template<class _Tp> operator _Tp*() const { return 0; }
				// Make nullptr convertible to any member pointer type.
				template<class _Cp, class _Tp> operator _Tp _Cp::*() { return 0; }
				bool operator==(_nullptr_t_as_class_impl) const { return true; }
				bool operator!=(_nullptr_t_as_class_impl) const { return false; }
			private:
				// Do not allow taking the address of nullptr.
				void operator&();

				void *_padding;
			};

			class _nullptr_t_as_class_impl1 {
			public:
				// Required in order to create const nullptr_t objects without an
				// explicit initializer in GCC 4.5, a la:
				//
				// const std::nullptr_t nullptr;
				_nullptr_t_as_class_impl1() { }
				_nullptr_t_as_class_impl1(int) { }

				// Make nullptr convertible to any pointer type.
				template<class _Tp> operator _Tp*() const { return 0; }

				bool operator==(_nullptr_t_as_class_impl1) const { return true; }
				bool operator!=(_nullptr_t_as_class_impl1) const { return false; }
			private:
				// Do not allow taking the address of nullptr.
				void operator&();



				void *_padding;
			};

			namespace ptrdiff_detail
			{
				using namespace std;
			}

			template<bool>
			struct _pointer_as_long_type { typedef long type; };
			template<>
			struct _pointer_as_long_type<false> { typedef long type; };
			template<bool>
			struct _pointer_as_short_type { typedef short type; };
			template<>
			struct _pointer_as_short_type<false> { typedef _pointer_as_long_type<sizeof(long) == sizeof(void*)>::type type; };
			template<bool>
			struct _pointer_as_int_type { typedef int type; };
			template<>
			struct _pointer_as_int_type<false> { typedef _pointer_as_short_type<sizeof(short) == sizeof(void*)>::type type; };

			template<bool>
			struct _pointer_as_ulong_type { typedef unsigned long type; };
			template<>
			struct _pointer_as_ulong_type<false> { typedef unsigned long type; };
			template<bool>
			struct _pointer_as_ushort_type { typedef unsigned short type; };
			template<>
			struct _pointer_as_ushort_type<false> { typedef _pointer_as_long_type<sizeof(unsigned long) == sizeof(void*)>::type type; };
			template<bool>
			struct _pointer_as_uint_type { typedef unsigned int type; };
			template<>
			struct _pointer_as_uint_type<false> { typedef _pointer_as_short_type<sizeof(unsigned short) == sizeof(void*)>::type type; };

			template<bool>
			struct _pointer_as_integral_type_impl { typedef _pointer_as_int_type<sizeof(int) == sizeof(void*)>::type type; };
			template<>
			struct _pointer_as_integral_type_impl<false> { typedef _pointer_as_uint_type<sizeof(int) == sizeof(void*)>::type type; };

			struct _ptrdiff_is_signed
			{
				static const bool value = (ptrdiff_detail::ptrdiff_t)(-1) < (ptrdiff_detail::ptrdiff_t)(0);
			};

			struct _pointer_is_signed
			{
				static const bool value = true;
			};

			template<bool>
			struct _pointer_as_ptrdiff_type { typedef ptrdiff_detail::ptrdiff_t type; };
			template<>
			struct _pointer_as_ptrdiff_type<false>:
				_pointer_as_integral_type_impl<_ptrdiff_is_signed::value>
			{ };


			template<bool>
			struct _nullptr_as_ptrdiff_type:
				_pointer_as_ptrdiff_type<sizeof(ptrdiff_detail::ptrdiff_t) == sizeof(void*)>
			{ };

			template<>
			struct _nullptr_as_ptrdiff_type<false>:
				_pointer_as_integral_type_impl<_pointer_is_signed::value>
			{ };

			typedef _pointer_as_integral_type_impl<false>::type _nullptr_t_as_uint;
			typedef _pointer_as_integral_type_impl<true>::type _nullptr_t_as_int;
			typedef _nullptr_as_ptrdiff_type<_ptrdiff_is_signed::value == _pointer_is_signed::value>::type _nullptr_t_as_integral;
			
			template<bool>
			struct _nullptr_t_as_enum_chooser
			{
				enum type
				{
					_nullptr_val = _nullptr_t_as_integral(STDEX_NULL),
					_max_nullptr = _nullptr_t_as_integral((_nullptr_t_as_uint(1) << (CHAR_BIT * sizeof(void*) - 1)) / 2)
				};
			};

			template<>
			struct _nullptr_t_as_enum_chooser<false>
			{
				enum type
				{
					_nullptr_val = _nullptr_t_as_integral(STDEX_NULL),
					_max_nullptr = _nullptr_t_as_uint(1) << (CHAR_BIT * sizeof(void*) - 1)
				};
			};

			typedef _nullptr_t_as_enum_chooser<_pointer_is_signed::value>::type _nullptr_t_as_enum;

			typedef void* _nullptr_t_as_void;
		}

		namespace nullptr_comparison_detail
		{
			template<class _T1, class _T2>
			nullptr_detail::_no_type operator==(_T1, _T2);

			template<class _Tp>
			struct _nullptr_can_be_compared_to_ptr
			{
				static const int *_ptr;
				static const bool value = sizeof(((_Tp) (STDEX_NULL)) == _ptr) != sizeof(nullptr_detail::_no_type);
			};
		}

		template<class _Tp>
		struct _nullptr_can_be_ct_constant_impl
		{
			// idk how to check for compile time constantness of type in general for any c++98 compiler, so...
			static const bool value = false;// (sizeof(nullptr_detail::_nullptr_can_be_ct_constant<T>(0)) == sizeof(nullptr_detail::_yes_type));
		};

		template<class _Tp>
		struct _is_convertable_to_void_ptr_impl
		{
			static const bool value = (sizeof(nullptr_detail::_is_convertable_to_void_ptr_tester((_Tp) (STDEX_NULL))) == sizeof(nullptr_detail::_yes_type));
		};

		template<class _Tp>
		struct _is_convertable_to_member_function_ptr_impl
		{
			static const bool value = 
				(sizeof(nullptr_detail::_is_convertable_to_member_function_ptr_tester((_Tp) (STDEX_NULL))) == sizeof(nullptr_detail::_yes_type)) &&
				(sizeof(nullptr_detail::_is_convertable_to_const_member_function_ptr_tester((_Tp) (STDEX_NULL))) == sizeof(nullptr_detail::_yes_type));
		};

		template<class NullPtrType, class _Tp>
		struct _is_convertable_to_any_ptr_impl_helper
		{
			static const bool value = (sizeof(nullptr_detail::_is_convertable_to_ptr_tester<_Tp>((NullPtrType) (STDEX_NULL))) == sizeof(nullptr_detail::_yes_type));
		};

		template<class _Tp>
		struct _is_convertable_to_any_ptr_impl
		{


			static const bool value = _is_convertable_to_any_ptr_impl_helper<_Tp, int>::value &&
									  _is_convertable_to_any_ptr_impl_helper<_Tp, float>::value &&
									  _is_convertable_to_any_ptr_impl_helper<_Tp, bool>::value &&
									  _is_convertable_to_any_ptr_impl_helper<_Tp, const bool>::value &&
									  _is_convertable_to_any_ptr_impl_helper<_Tp, volatile float>::value &&
									  _is_convertable_to_any_ptr_impl_helper<_Tp, volatile const double>::value &&
									  _is_convertable_to_any_ptr_impl_helper<_Tp, nullptr_detail::_dummy_class>::value;
		};

		template<class _Tp>
		struct _is_convertable_to_ptr_impl
		{
			static const bool value = (
				_is_convertable_to_void_ptr_impl<_Tp>::value == bool(true) && 
				_is_convertable_to_any_ptr_impl<_Tp>::value == bool(true) &&
				_is_convertable_to_member_function_ptr_impl<_Tp>::value == bool(true)
				);
		};

		template<class _Tp>
		struct _is_equal_size_to_void_ptr
		{
			static const bool value = (sizeof(_Tp) == sizeof(void*));
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
			typedef nullptr_detail::_nullptr_t_as_class_impl type;
		};

		template<>
		struct _nullptr_t_as_class_chooser<false>
		{
			typedef nullptr_detail::_nullptr_t_as_class_impl1 type;
		};

		template<bool>
		struct _nullptr_choose_as_int
		{
			typedef nullptr_detail::_nullptr_t_as_integral type;
		};

		template<bool>
		struct _nullptr_choose_as_enum
		{
			typedef nullptr_detail::_nullptr_t_as_enum type;
		};

		template<bool>
		struct _nullptr_choose_as_class
		{
			typedef _nullptr_t_as_class_chooser<_member_ptr_is_same_as_ptr::value>::type type;
		};

		template<>
		struct _nullptr_choose_as_int<false>
		{
			typedef nullptr_detail::_nullptr_t_as_void type;
		};

		template<>
		struct _nullptr_choose_as_enum<false>
		{
			struct _as_int
			{
				typedef nullptr_detail::_nullptr_t_as_integral _nullptr_t_as_integral;

				static const bool _is_convertable_to_ptr = _is_convertable_to_ptr_impl<_nullptr_t_as_integral>::value;
				static const bool _equal_void_ptr = _is_equal_size_to_void_ptr<_nullptr_t_as_integral>::value;
				static const bool _can_be_compared_to_ptr = nullptr_comparison_detail::_nullptr_can_be_compared_to_ptr<_nullptr_t_as_integral>::value;
			};

			typedef _nullptr_choose_as_int<_as_int::_is_convertable_to_ptr == bool(true) && _as_int::_equal_void_ptr == bool(true) && _as_int::_can_be_compared_to_ptr == bool(true)>::type type;
		};

		template<>
		struct _nullptr_choose_as_class<false>
		{
			struct _as_enum
			{
				typedef nullptr_detail::_nullptr_t_as_enum _nullptr_t_as_enum;

				static const bool _is_convertable_to_ptr = _is_convertable_to_ptr_impl<_nullptr_t_as_enum>::value;
				static const bool _equal_void_ptr = _is_equal_size_to_void_ptr<_nullptr_t_as_enum>::value;
				static const bool _can_be_ct_constant = true;//_nullptr_can_be_ct_constant_impl<_nullptr_t_as_enum>::value;
			};

			typedef _nullptr_choose_as_enum<_as_enum::_is_convertable_to_ptr == bool(true) && _as_enum::_equal_void_ptr == bool(true) && _as_enum::_can_be_ct_constant == bool(true)>::type type;
		};

		struct _nullptr_chooser
		{


			struct _as_class
			{
				typedef _nullptr_t_as_class_chooser<_member_ptr_is_same_as_ptr::value>::type _nullptr_t_as_class;

				static const bool _equal_void_ptr = _is_equal_size_to_void_ptr<_nullptr_t_as_class>::value;
				static const bool _can_be_ct_constant = _nullptr_can_be_ct_constant_impl<_nullptr_t_as_class>::value;
			};

			typedef _nullptr_choose_as_class<_as_class::_equal_void_ptr == bool(true) && _as_class::_can_be_ct_constant == bool(true)>::type type;
		};
	}

typedef detail::_nullptr_chooser::type nullptr_t;

}

#ifdef nullptr
	#undef nullptr
#endif

#define nullptr (stdex::nullptr_t)(STDEX_NULL)


#endif // _STDEX_NULLPTR_H
