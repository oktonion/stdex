#ifndef _STDEX_NULLPTR_H
#define _STDEX_NULLPTR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace stdex
{

	class nullptr_t
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

		/*template<class T, class C>
		inline operator T C::*() const   // or any type of null member pointer...
		{
			return 0;
		}*/

		template<typename T>
		inline bool operator==(T *val) { return equal(val); }

		template<typename T>
		inline bool operator!=(T *val) { return !equal(val); }

		template<typename T>
		inline bool operator<(T *val) { return less(val); }

		template<typename T>
		inline bool operator<=(T *val) { return less_equal(val); }

		template<typename T>
		inline bool operator>(T *val) { return false; }//nullptr always less or equal (can't be greater than)

		template<typename T>
		inline bool operator>=(T *val) { return greater_equal(val); }

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
	};

}


#define nullptr stdex::nullptr_t(0)

#endif