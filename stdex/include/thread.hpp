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

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define DELETED_FUNCTION =delete
#define NOEXCEPT_FUNCTION noexcept 

#else

#define DELETED_FUNCTION 
#define NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
	namespace detail
	{

		template<class _T0 = detail::void_type, class _T1 = detail::void_type, class _T2 = detail::void_type, class _T3 = detail::void_type, class _T4 = detail::void_type, class _T5 = detail::void_type, class _T6 = detail::void_type, class _T7 = detail::void_type, class _T8 = detail::void_type, class _T9 = detail::void_type, class _T10 = detail::void_type, class _T11 = detail::void_type, class _T12 = detail::void_type, class _T13 = detail::void_type, class _T14 = detail::void_type, class _T15 = detail::void_type, class _T16 = detail::void_type, class _T17 = detail::void_type, class _T18 = detail::void_type, class _T19 = detail::void_type, class _T20 = detail::void_type, class _T21 = detail::void_type, class _T22 = detail::void_type, class _T23 = detail::void_type, class _T24 = detail::void_type>
		struct _thread_args_helper;

		enum eTypeNullptr { _type_nullptr };
		enum eTypeNotNullptr { _type_not_nullptr };

		template<class _FuncT> struct _function_traits;

		template<class R>
		struct _function_traits<R(*)(void)>
		{
			typedef R result_type;
		};

		template<class R, class T1>
		struct _function_traits<R(*)(T1)>
		{
			typedef R result_type;
			typedef T1 arg1_type;
			typedef T1 argument_type;
		};

		template<class R, class T1, class T2>
		struct _function_traits<R(*)(T1, T2)>
		{
			typedef R result_type;
			typedef T1 arg1_type;
			typedef T2 arg2_type;
			
			
		};

		template<class R, class T1, class T2, class T3>
		struct _function_traits<R(*)(T1, T2, T3)>
		{
			typedef R result_type;
			typedef T1 arg1_type;
			typedef T2 arg2_type;
			typedef T3 arg3_type;
		};

		template<class R, class T1, class T2, class T3, class T4>
		struct _function_traits<R(*)(T1, T2, T3, T4)>
		{
			typedef R result_type;
			typedef T1 arg1_type;
			typedef T2 arg2_type;
			typedef T3 arg3_type;
			typedef T4 arg4_type;
		};

		template<class R, class T1, class T2, class T3, class T4,
			class T5>
		struct _function_traits<R(*)(T1, T2, T3, T4, T5)>
		{
			typedef R result_type;
			typedef T1 arg1_type;
			typedef T2 arg2_type;
			typedef T3 arg3_type;
			typedef T4 arg4_type;
			typedef T5 arg5_type;
		};

		template<class R, class T1, class T2, class T3, class T4,
			class T5, class T6>
		struct _function_traits<R(*)(T1, T2, T3, T4, T5, T6)>
		{
			typedef R result_type;
			typedef T1 arg1_type;
			typedef T2 arg2_type;
			typedef T3 arg3_type;
			typedef T4 arg4_type;
			typedef T5 arg5_type;
			typedef T6 arg6_type;
		};

		template<class R, class T1, class T2, class T3, class T4,
			class T5, class T6, class T7>
		struct _function_traits<R(*)(T1, T2, T3, T4, T5, T6, T7)>
		{
			typedef R result_type;
			typedef T1 arg1_type;
			typedef T2 arg2_type;
			typedef T3 arg3_type;
			typedef T4 arg4_type;
			typedef T5 arg5_type;
			typedef T6 arg6_type;
			typedef T7 arg7_type;
		};

		template<class R, class T1, class T2, class T3, class T4,
			class T5, class T6, class T7, class T8>
		struct _function_traits<R(*)(T1, T2, T3, T4, T5, T6, T7, T8)>
		{
			typedef R result_type;
			typedef T1 arg1_type;
			typedef T2 arg2_type;
			typedef T3 arg3_type;
			typedef T4 arg4_type;
			typedef T5 arg5_type;
			typedef T6 arg6_type;
			typedef T7 arg7_type;
			typedef T8 arg8_type;
		};


		template<bool>
		struct _type_is_nullptr_helper
		{ 
			typedef eTypeNullptr type;
			static const type value = _type_nullptr;
		};

		template<>
		struct _type_is_nullptr_helper<false>
		{ 
			typedef eTypeNotNullptr type;
			static const type value = _type_not_nullptr;
		};

		template<class _T, class _ArgT = void*>
		struct _type_is_nullptr
		{
			typedef _type_is_nullptr_helper<_is_nullptr_t<_T>::value == (true) && is_pointer<_ArgT>::value == (true)> _check_type;
			static const typename _check_type::type value = _check_type::value;
		};

		template<>
		struct _thread_args_helper<>
		{
			struct _arguments
			{
				template<class _FuncT> void call(_FuncT &fp) { fp(); }

				_arguments()
				{ }

				template<class _FuncT>
				void push(_FuncT &fp)
				{
					call(fp);
				}

				template<class _ObjectT, class _FuncT>
				void push(_ObjectT &fp, _FuncT mp)
				{
					push(fp);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1>
		struct _thread_args_helper<_Arg1>
		{
			struct _arguments
			{
				_Arg1 arg1;

				_arguments(_Arg1 arg1_) :
					arg1(arg1_)
				{ }

				template<class _FuncT> void call(_FuncT &fp, eTypeNotNullptr) { fp(arg1); }
				template<class _FuncT> void call(_FuncT &fp, eTypeNullptr) { fp(nullptr); }

				template<class _FuncT>
				void push(_FuncT &fp)
				{
					call(fp,
						_type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1))
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1) const)
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2>
		struct _thread_args_helper<_Arg1, _Arg2>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;

				_arguments(_Arg1 arg1_, _Arg2 arg2_) :
					arg1(arg1_), arg2(arg2_)
				{ }

template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr); }

				template<class _FuncT>
				void push(_FuncT &fp)
				{
					call(fp,
						_type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
						_type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2))
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2) const)
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2, class _Arg3>
		struct _thread_args_helper<_Arg1, _Arg2, _Arg3>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;
				_Arg3 arg3;

				_arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_) :
					arg1(arg1_), arg2(arg2_), arg3(arg3_)
				{ }

template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr); }

				template<class _FuncT>
				void push(_FuncT &fp)
				{
					call(fp,
						_type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
						_type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value,
						_type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3))
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3) const)
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2, class _Arg3, class _Arg4>
		struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;
				_Arg3 arg3;
				_Arg4 arg4;

				_arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg4 arg4_) :
					arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_)
				{ }
				
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr, nullptr, nullptr, nullptr); }
				

				template<class _FuncT>
				void push(_FuncT &fp)
				{
					call(fp,
						_type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
						_type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value,
						_type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value,
						_type_is_nullptr<_Arg4, typename _function_traits<_FuncT>::arg4_type>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3, _FArg4))
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value,
						_type_is_nullptr<_Arg4, _FArg4>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3, _FArg4) const)
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value,
						_type_is_nullptr<_Arg4, _FArg4>::value);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5>
		struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4, _Arg5>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;
				_Arg3 arg3;
				_Arg4 arg4;
				_Arg5 arg5;

				_arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg3 arg4_, _Arg5 arg5_) :
					arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_)
				{ }

template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr); }

				template<class _FuncT>
				void push(_FuncT &fp)
				{
					call(fp,
						_type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
						_type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value,
						_type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value,
						_type_is_nullptr<_Arg4, typename _function_traits<_FuncT>::arg4_type>::value,
						_type_is_nullptr<_Arg5, typename _function_traits<_FuncT>::arg5_type>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5))
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value,
						_type_is_nullptr<_Arg4, _FArg4>::value,
						_type_is_nullptr<_Arg5, _FArg5>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5) const)
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value,
						_type_is_nullptr<_Arg4, _FArg4>::value,
						_type_is_nullptr<_Arg5, _FArg5>::value);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6>
		struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;
				_Arg3 arg3;
				_Arg4 arg4;
				_Arg5 arg5;
				_Arg6 arg6;

				_arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg3 arg4_, _Arg5 arg5_, _Arg6 arg6_) :
					arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), arg6(arg6_)
				{ }

template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,arg6); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }


				template<class _FuncT>
				void push(_FuncT &fp)
				{
					call(fp,
						_type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
						_type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value,
						_type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value,
						_type_is_nullptr<_Arg4, typename _function_traits<_FuncT>::arg4_type>::value,
						_type_is_nullptr<_Arg5, typename _function_traits<_FuncT>::arg5_type>::value,
						_type_is_nullptr<_Arg6, typename _function_traits<_FuncT>::arg6_type>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6))
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value,
						_type_is_nullptr<_Arg4, _FArg4>::value,
						_type_is_nullptr<_Arg5, _FArg5>::value,
						_type_is_nullptr<_Arg6, _FArg6>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6) const)
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value,
						_type_is_nullptr<_Arg4, _FArg4>::value,
						_type_is_nullptr<_Arg5, _FArg5>::value,
						_type_is_nullptr<_Arg6, _FArg6>::value);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6, class _Arg7>
		struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;
				_Arg3 arg3;
				_Arg4 arg4;
				_Arg5 arg5;
				_Arg6 arg6;
				_Arg7 arg7;

				_arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg3 arg4_, _Arg5 arg5_, _Arg6 arg6_, _Arg7 arg7_) :
					arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), arg6(arg6_), arg7(arg7_)
				{ }

template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,arg6,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,arg6,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,arg7); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }


				template<class _FuncT>
				void push(_FuncT &fp)
				{
					call(fp,
						_type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
						_type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value,
						_type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value,
						_type_is_nullptr<_Arg4, typename _function_traits<_FuncT>::arg4_type>::value,
						_type_is_nullptr<_Arg5, typename _function_traits<_FuncT>::arg5_type>::value,
						_type_is_nullptr<_Arg6, typename _function_traits<_FuncT>::arg6_type>::value,
						_type_is_nullptr<_Arg7, typename _function_traits<_FuncT>::arg7_type>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6, class _FArg7>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6, _FArg7))
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value,
						_type_is_nullptr<_Arg4, _FArg4>::value,
						_type_is_nullptr<_Arg5, _FArg5>::value,
						_type_is_nullptr<_Arg6, _FArg6>::value,
						_type_is_nullptr<_Arg7, _FArg7>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6, class _FArg7>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6, _FArg7) const)
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value,
						_type_is_nullptr<_Arg4, _FArg4>::value,
						_type_is_nullptr<_Arg5, _FArg5>::value,
						_type_is_nullptr<_Arg6, _FArg6>::value,
						_type_is_nullptr<_Arg7, _FArg7>::value);
				}
			};

			typedef _arguments arguments_type;
		};

		template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6, class _Arg7, class _Arg8>
		struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>
		{
			struct _arguments
			{
				_Arg1 arg1;
				_Arg2 arg2;
				_Arg3 arg3;
				_Arg4 arg4;
				_Arg5 arg5;
				_Arg6 arg6;
				_Arg7 arg7;
				_Arg8 arg8;

				_arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg3 arg4_, _Arg5 arg5_, _Arg6 arg6_, _Arg7 arg7_, _Arg8 arg8_) :
					arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), arg6(arg6_), arg7(arg7_), arg8(arg8_)
				{ }

template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,arg4,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,arg4,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,arg2,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,arg4,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,arg4,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(arg1,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,arg4,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,arg4,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,arg2,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,arg4,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,arg4,nullptr,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,arg5,nullptr,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,arg6,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,arg6,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,arg6,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,arg6,nullptr,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,arg7,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,arg7,nullptr); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,arg8); }
template<class _FuncT> void call(_FuncT &fp,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }


				template<class _FuncT>
				void push(_FuncT &fp)
				{
					call(fp, 
						_type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value, 
						_type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value, 
						_type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value,
						_type_is_nullptr<_Arg4, typename _function_traits<_FuncT>::arg4_type>::value,
						_type_is_nullptr<_Arg5, typename _function_traits<_FuncT>::arg5_type>::value,
						_type_is_nullptr<_Arg6, typename _function_traits<_FuncT>::arg6_type>::value,
						_type_is_nullptr<_Arg7, typename _function_traits<_FuncT>::arg7_type>::value,
						_type_is_nullptr<_Arg8, typename _function_traits<_FuncT>::arg8_type>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6, class _FArg7, class _FArg8>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6, _FArg7, _FArg8))
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value,
						_type_is_nullptr<_Arg4, _FArg4>::value,
						_type_is_nullptr<_Arg5, _FArg5>::value,
						_type_is_nullptr<_Arg6, _FArg6>::value,
						_type_is_nullptr<_Arg7, _FArg7>::value,
						_type_is_nullptr<_Arg8, _FArg8>::value);
				}

				template<class _ObjectT, class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6, class _FArg7, class _FArg8>
				void push(_ObjectT &fp, _ReturnT(_ObjectT::*mp)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6, _FArg7, _FArg8) const)
				{
					call(fp,
						_type_is_nullptr<_Arg1, _FArg1>::value,
						_type_is_nullptr<_Arg2, _FArg2>::value,
						_type_is_nullptr<_Arg3, _FArg3>::value,
						_type_is_nullptr<_Arg4, _FArg4>::value,
						_type_is_nullptr<_Arg5, _FArg5>::value,
						_type_is_nullptr<_Arg6, _FArg6>::value,
						_type_is_nullptr<_Arg7, _FArg7>::value,
						_type_is_nullptr<_Arg8, _FArg8>::value);
				}
			};

			typedef _arguments arguments_type;
		};

		enum eTypeIsClass { _type_is_class };
		enum eTypeIsNotClass { _type_is_not_class };

		template<bool>
		struct _is_class_overload
		{
			static const eTypeIsClass value = _type_is_class;
		};

		template<>
		struct _is_class_overload<false>
		{
			static const eTypeIsNotClass value = _type_is_not_class;
		};

		template<class _FuncT, class _ArgT>
		struct _thread_function_proxy
		{
			typedef _ArgT arguments_type;
			typedef _FuncT function_type;

			arguments_type args;
			function_type fp;

			_thread_function_proxy(function_type fp_, arguments_type args_) :
				fp(fp_),
				args(args_)
			{ }

			~_thread_function_proxy()
			{ }

			void call()
			{
				call(_is_class_overload<is_class<_FuncT>::value>::value);
			}

			void call(eTypeIsClass)
			{
				args.push(fp, &_FuncT::operator());
			}

			void call(eTypeIsNotClass)
			{
				args.push(fp);
			}

			static void gproxy(void *data)
			{
				_thread_function_proxy *pf = reinterpret_cast<_thread_function_proxy*>(data);

				pf->call();

				delete pf;
			}
		};
}

	//! Thread class.
	class thread {

		void init(void(*aFunction)(void *), void * aArg);

	public:
		typedef pthread_t native_handle_type;

		//! Thread ID.
		//! The thread ID is a unique identifier for each thread.
		//! @see thread::get_id()
		class id {
		public:
			//! Default constructor.
			//! The default constructed ID is that of thread without a thread of
			//! execution.
			id() NOEXCEPT_FUNCTION :
				_uid()
			{ }

			explicit id(const stdex::uintmax_t &uid) :
				_uid(uid)
			{ }

			id(const id &aId) :
				_uid(aId._uid)
			{ }

			inline id& operator=(const id &aId)
			{
				_uid = aId._uid;

				return *this;
			}

			bool operator==(const id &other) const NOEXCEPT_FUNCTION
			{
				return _uid == other._uid;
			}

			inline friend bool operator!=(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
			{
				return !(aId1 == aId2);
			}

			inline friend bool operator<=(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
			{
				return aId1._uid <= aId2._uid;
			}

			bool operator<(const id &other) const NOEXCEPT_FUNCTION
			{
				return _uid < other._uid;
			}

			inline friend bool operator>=(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
			{
				return aId1._uid >= aId2._uid;
			}

			inline friend bool operator>(const id &aId1, const id &aId2) NOEXCEPT_FUNCTION
			{
				return aId1._uid > aId2._uid;
			}

			::std::ostream& print(::std::ostream &out) const
			{
				if (*this == id())
					return out << "thread::id of a non-executing thread";
				else
					return out << _uid;
			}

		private:
			friend class thread;
			stdex::uintmax_t _uid;
		};

		//! Default constructor.
		//! Construct a @c thread object without an associated thread of execution
		//! (i.e. non-joinable).
		thread() NOEXCEPT_FUNCTION
		{ }

		template<class _FuncT>
		explicit thread(_FuncT fx)
		{
			typedef typename detail::_thread_args_helper<>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t()));
		}

		template<class _FuncT, class _Arg0T>
		explicit thread(_FuncT fx, _Arg0T t0)
		{
			typedef typename detail::_thread_args_helper<_Arg0T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0)));
		}

		template<class _FuncT, class _Arg0T, class _Arg1T>
		explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1)
		{
			typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1)));
		}

		template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T>
		explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2)
		{
			typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2)));
		}

		template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T>
		explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3)
		{
			typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T, _Arg3T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3)));
		}

		template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T>
		explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4)
		{
			typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4)));
		}

		template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T, class _Arg5T>
		explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4, _Arg5T t5)
		{
			typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T, _Arg5T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4, t5)));
		}

		template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T, class _Arg5T, class _Arg6T>
		explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4, _Arg5T t5, _Arg6T t6)
		{
			typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T, _Arg5T, _Arg6T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4, t5, t6)));
		}

		template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T, class _Arg5T, class _Arg6T, class _Arg7T>
		explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4, _Arg5T t5, _Arg6T t6, _Arg7T t7)
		{
			typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T, _Arg5T, _Arg6T, _Arg7T>::arguments_type args_t;
			typedef _FuncT func_t;
			init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4, t5, t6, t7)));
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
		inline native_handle_type native_handle()
		{
			return _handle;
		}

		//! Determine the number of threads which can possibly execute concurrently.
		//! This function is useful for determining the optimal number of threads to
		//! use for a task.
		//! @return The number of hardware thread contexts in the system.
		//! @note If this value is not defined, the function returns zero (0).
		static unsigned hardware_concurrency() NOEXCEPT_FUNCTION;

		void swap(thread &other) NOEXCEPT_FUNCTION;

	private:

		id _id;
		native_handle_type _handle;

		//thread(thread&) DELETED_FUNCTION;
		thread(const thread&) DELETED_FUNCTION;
		thread& operator=(const thread&) DELETED_FUNCTION;

		// This is the internal thread wrapper function.
		static void* wrapper_function(void *aArg);
	};

	inline void swap(stdex::thread &lhs, stdex::thread &rhs) NOEXCEPT_FUNCTION
	{
		lhs.swap(rhs);
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




inline ::std::ostream& operator<<(::std::ostream &out, const stdex::thread::id &id)
{
	return id.print(out);
}

namespace std
{
	template<>
	inline void swap(stdex::thread &lhs, stdex::thread &rhs)
	{
		using namespace std;

		lhs.swap(rhs);
	}
}



#undef DELETED_FUNCTION
#undef NOEXCEPT_FUNCTION

#endif // _STDEX_THREAD_H
