// testing common names that should not be defined in stdex lib
#define A (
#define B (
#define C (
#define D (
#define E (
#define F (
#define G (
#define H (
#define I (
#define J (
#define K (
#define L (
#define M (
#define N (
#define O (
#define P (
#define Q (
#define R (
#define S (
#define T (
#define U (
#define V (
#define W (
#define X (
#define Y (
#define Z (
#if __cplusplus >= 201103L
// <random> defines member functions called a() and b()
#else
#define a (
#define b (
#endif
// <queue> and <stack> defined data members called c
#define d (
#define e (
#define f (
#define g (
#if __cplusplus >= 201402L
// <chrono> defines operator ""h in C++14
// <complex> defines operator ""i in C++14
#else
#define h (
#define i (
#endif
#define j (
#if __cplusplus >= 201103L
// <random> defines member functions called k()
#else
#define k (
#endif
#define l (
#if __cplusplus >= 201103L
// <random> defines member functions called m() and n()
#else
#define m (
#define n (
#endif
#define o (
#if __cplusplus >= 201103L
// <random> defines member functions called p()
#else
#define p (
#endif
#define q (
#define r (
#if __cplusplus >= 201103L
// <random> defines member functions called s() and t()
// <chrono> and <string> define operator ""s in C++14
#else
#define s (
#define t (
#endif
#define u (
#define v (
#define w (
#define x (
#define y (
#define z (

#define tmp (

#if __cplusplus < 201703L
// <charconv> defines to_chars_result::ptr and to_chars_result::ec
#define ec (
#define ptr (
#endif

// Common template parameter names
#define OutputIterator		OutputIterator should not be a reserved name
#define InputIterator		InputIterator should not be a reserved name
#define ForwardIterator		ForwardIterator should not be a reserved name
#define BidirectionalIterator	BidirectionalIterator should not be a reserved name
#define RandomAccessIterator	RandomAccessIterator should not be a reserved name
#define RandomAccessOutputIterator	RandomAccessOutputIterator should not be a reserved name
#define RAIter			RAIter should not be a reserved name
#define FwdIter			FwdIter should not be a reserved name
#define OutIter			OutIter should not be a reserved name
#define InIter			InIter should not be a reserved name

#define Alloc			Alloc should not be a reserved name
#define BinaryFunction1		BinaryFunction1 should not be a reserved name
#define BinaryFunction2		BinaryFunction2 should not be a reserved name
#define Char			Char should not be a reserved name
#define CharT			CharT should not be a reserved name
#define Cmp			Cmp should not be a reserved name
#define Compare			Compare should not be a reserved name
#define Const_Iterator		Const_Iterator should not be a reserved name
#define Const_Key_Reference	Const_Key_Reference should not be a reserved name
#define Const_Node_Iter		Const_Node_Iter should not be a reserved name
#define Const_Pointer		Const_Pointer should not be a reserved name
#define Const_Reference		Const_Reference should not be a reserved name
#define Data			Data should not be a reserved name
#define DiffType		DiffType should not be a reserved name
#define Eq			Eq should not be a reserved name
#define ExecPolicy		ExecPolicy should not be a reserved name
#define Expected		Expected should not be a reserved name
#define External_Load_Access	External_Load_Access should not be a reserved name
#define External_Size_Access	External_Size_Access should not be a reserved name
#define Fn			Fn should not be a reserved name
#define Function		Function should not be a reserved name
#define Functor			Functor should not be a reserved name
#define Hash			Hash should not be a reserved name
#define H1			H1 should not be a reserved name
#define H2			H2 should not be a reserved name
#define Head			Head should not be a reserved name
#define It			It should not be a reserved name
#define Iter			Iter should not be a reserved name
#define Iterator		Iterator should not be a reserved name
#define Key			Key should not be a reserved name
#define Mapped			Mapped should not be a reserved name
#define Node			Node should not be a reserved name
#define Node_iter		Node_iter should not be a reserved name
#define Node_ptr		Node_ptr should not be a reserved name
#define Overflow		Overflow should not be a reserved name
#define Pointer			Pointer should not be a reserved name
#define Policy			Policy should not be a reserved name
#define Pred			Pred should not be a reserved name
#define Ptr			Ptr should not be a reserved name
#define Reference		Reference should not be a reserved name
#define Seq			Seq should not be a reserved name
#define Seq_RAIter		Seq_RAIter should not be a reserved name
#define Series			Series should not be a reserved name
#define Set			Set should not be a reserved name
#define String			String should not be a reserved name
#define Tp			Tp should not be a reserved name
#define Traits			Traits should not be a reserved name
#define Type			Type should not be a reserved name
#define Value			Value should not be a reserved name
#define ValueT			ValueT should not be a reserved name
#define ValueType		ValueType should not be a reserved name

#ifdef _AIX
#undef f
#undef r
#undef x
#undef y
#endif

#ifdef __hpux__
#undef d
#undef r
#endif

#if defined (__linux__) && defined (__sparc__)
#undef y
#endif

#ifdef __sun__
#undef ptr
#endif

#include "../stdex/include/core.h"
#include "../stdex/include/algorithm.hpp"
#include "../stdex/include/chrono.hpp"
#include "../stdex/include/condition_variable.hpp"
#include "../stdex/include/cstdint.hpp"
#include "../stdex/include/iterator.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/ratio.hpp"
#include "../stdex/include/sstream.hpp"
#include "../stdex/include/string.hpp"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/thread.hpp"
#include "../stdex/include/type_traits.hpp"