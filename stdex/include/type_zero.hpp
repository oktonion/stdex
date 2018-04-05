#include <memory.h>
#include <cstddef>

namespace stdex
{
	template<class T>
	struct type_zero :
		public T
	{
		type_zero() { memset(&static_cast<T>(*this), 0, sizeof(T)); }
	};

	template<class T, std::size_t N>
	struct type_zero<T[N]>
	{
        typedef T array_t[N];

        array_t data;

		type_zero() { memset(data, 0, sizeof(data)); }

        T& operator[](int n) { return data[n]; }
        const T& operator[](int n) const { return data[n]; }

		T& operator*() { return *data; }
		const T& operator*() const { return *data; }

		friend T* operator+(type_zero &arr, int n) { return arr.data + n; }
		friend T* operator+(int n, type_zero &arr) { return n + arr.data; }
		friend const T* operator+(const type_zero &arr, int n) { return arr.data + n; }
		friend const T* operator+(int n, const type_zero &arr) { return n + arr.data; }

		friend T* operator-(type_zero &arr, int n) { return arr.data - n; }
		friend T* operator-(int n, type_zero &arr) { return n - arr.data; }
		friend const T* operator-(const type_zero &arr, int n) { return arr.data - n; }
		friend const T* operator-(int n, const type_zero &arr) { return n - arr.data; }

		friend bool operator>(const type_zero &lhs, const type_zero &rhs) { return lhs.data > rhs.data; }
		friend bool operator>(const T *lhs, const type_zero &rhs) { return lhs > rhs.data; }
		friend bool operator>(const type_zero &lhs, const T *rhs) { return lhs.data > rhs; }

		friend bool operator>=(const type_zero &lhs, const type_zero &rhs) { return lhs.data >= rhs.data; }
		friend bool operator>=(const T *lhs, const type_zero &rhs) { return lhs >= rhs.data; }
		friend bool operator>=(const type_zero &lhs, const T *rhs) { return lhs.data >= rhs; }

		friend bool operator<(const type_zero &lhs, const type_zero &rhs) { return lhs.data < rhs.data; }
		friend bool operator<(const T *lhs, const type_zero &rhs) { return lhs < rhs.data; }
		friend bool operator<(const type_zero &lhs, const T *rhs) { return lhs.data < rhs; }

		friend bool operator<=(const type_zero &lhs, const type_zero &rhs) { return lhs.data <= rhs.data; }
		friend bool operator<=(const T *lhs, const type_zero &rhs) { return lhs <= rhs.data; }
		friend bool operator<=(const type_zero &lhs, const T *rhs) { return lhs.data <= rhs; }

		friend bool operator==(const type_zero &lhs, const type_zero &rhs) { return lhs.data == rhs.data; }

        operator array_t&() { return data; }
        operator const array_t&() const { return data; }
	};

    template<class T, std::size_t N>
	struct type_zero<const T[N]>
	{
        typedef T array_t[N];

        array_t data;

		type_zero() { memset(data, 0, sizeof(data)); }

        T& operator[](int n) { return data[n]; }
        const T& operator[](int n) const { return data[n]; }

		T& operator*() { return *data; }
		const T& operator*() const { return *data; }

		friend T* operator+(type_zero &arr, int n) { return arr.data + n; }
		friend T* operator+(int n, type_zero &arr) { return n + arr.data; }
		friend const T* operator+(const type_zero &arr, int n) { return arr.data + n; }
		friend const T* operator+(int n, const type_zero &arr) { return n + arr.data; }

		friend T* operator-(type_zero &arr, int n) { return arr.data - n; }
		friend T* operator-(int n, type_zero &arr) { return n - arr.data; }
		friend const T* operator-(const type_zero &arr, int n) { return arr.data - n; }
		friend const T* operator-(int n, const type_zero &arr) { return n - arr.data; }

        friend bool operator>(const type_zero &lhs, const type_zero &rhs) { return lhs.data > rhs.data; }
		friend bool operator>(const T *lhs, const type_zero &rhs)  { return lhs > rhs.data; }
		friend bool operator>(const type_zero &lhs, const T *rhs)  { return lhs.data > rhs; }

		friend bool operator>=(const type_zero &lhs, const type_zero &rhs) { return lhs.data >= rhs.data; }
		friend bool operator>=(const T *lhs, const type_zero &rhs) { return lhs >= rhs.data; }
		friend bool operator>=(const type_zero &lhs, const T *rhs) { return lhs.data >= rhs; }
        
		friend bool operator<(const type_zero &lhs, const type_zero &rhs) { return lhs.data < rhs.data; }
		friend bool operator<(const T *lhs, const type_zero &rhs) { return lhs < rhs.data; }
		friend bool operator<(const type_zero &lhs, const T *rhs) { return lhs.data < rhs; }

		friend bool operator<=(const type_zero &lhs, const type_zero &rhs) { return lhs.data <= rhs.data; }
		friend bool operator<=(const T *lhs, const type_zero &rhs) { return lhs <= rhs.data; }
		friend bool operator<=(const type_zero &lhs, const T *rhs) { return lhs.data <= rhs; }

		friend bool operator==(const type_zero &lhs, const type_zero &rhs) { return lhs.data == rhs.data; }

        operator array_t&() { return data; }
        operator const array_t&() const { return data; }
	};
}
