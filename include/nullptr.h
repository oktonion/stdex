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
		struct nullptr_chooser
		{
			#ifdef LLONG_MAX
			typedef ::uintmax_t uintmax_t;
			#else
			typedef uint32_t uintmax_t;
			#endif

			enum nullptr_t_as_enum
			{
				#ifdef NULL
				__nullptr_val = NULL,
				#else
				__nullptr_val = 0,
				#endif
				__max_nullptr = uintmax_t(1) << (CHAR_BIT * sizeof(void*) - 1)
			};

			template<bool>
			struct as_int
			{
				typedef int type;
			};

			template<bool>
			struct as_enum
			{
				typedef void* type;
			};

			typedef as_enum<sizeof(nullptr_t_as_enum) == sizeof(void*)>::type type;
		};

		template<>
		struct nullptr_chooser::as_int<false>
		{
			typedef void* type;
		};

		template<>
		struct nullptr_chooser::as_enum<false>
		{
			typedef as_int<sizeof(int) == sizeof(void*)>::type type;
		};
	}

typedef detail::nullptr_chooser::type nullptr_t;

}


#ifdef NULL
	#define nullptr stdex::nullptr_t(NULL)
#else
	#define nullptr stdex::nullptr_t(0)
#endif

#endif // _STDEX_NULLPTR_H