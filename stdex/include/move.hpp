#ifndef _STDEX_MOVE_H
#define _STDEX_MOVE_H
#ifdef _MSC_VER
#pragma warning (push)
	#pragma warning (disable : 4512) // assignment operator could not be generated
#endif

namespace stdex
{
	namespace detail
	{
		template<class T, T val>
		class deleted_implicit_copy_constructor_impl
		{
			const T dummy:1;
		public:
			deleted_implicit_copy_constructor_impl(const T &):
				dummy(val) { (void)(dummy); }
		};
	}

	typedef stdex::detail::deleted_implicit_copy_constructor_impl<char, 1> deleted_implicit_copy_constructor;
	
	class delete_implicit_copy_constructor:
		deleted_implicit_copy_constructor
	{
	public:
		delete_implicit_copy_constructor(const bool &):
			deleted_implicit_copy_constructor(1)
		{ }
	};

	template<class T>
	struct remove_const
	{
		typedef T type;
	};

	template<class T>
	struct remove_const<const T>
	{
		typedef T type;
	};

	template<class T>
	struct make_const
	{
		typedef const T type;
	};

	template<class T>
	struct make_const<const T>
	{
		typedef const T type;
	};
	
    template<class T>
    class rvalue_ref:
		public remove_const<T>::type
    {
	protected:
		typedef typename remove_const<T>::type type;
		const type& _ref;

        explicit rvalue_ref(const type &ref) 
			: type(ref),
            _ref((ref)) 
		{ }
    public:
		template<class TT>
        rvalue_ref(rvalue_ref<TT> &other)
			: type(other._ref),
			_ref(other._ref) 
		{ }
        //T& get() const {return _ref;}
		//operator T&() const {return _ref;}
		//T* operator->() const {return &get();}
		//T& operator*() const {return get();}
		static rvalue_ref<type> move(type &value) 
		{
			return rvalue_ref<type>(value);
		}
		static rvalue_ref<const type> move(const type &value) 
		{
			return rvalue_ref<const type>(value);
		}
		static rvalue_ref<type> move(const rvalue_ref<type>& value)
		{
			return rvalue_ref<type>(value._ref);
		}
    };

	namespace move_for_const_impl
	{
		template<class T>
		rvalue_ref<const T> move(const T& value)
		{
			return rvalue_ref<const T>::move(value);
		}
	} // namespace move_impl

	using move_for_const_impl::move;

	template<class T>
	rvalue_ref<T> move(T& value)
	{
		return rvalue_ref<T>::move(value);
	}

	template<class T>
	rvalue_ref<T> move(rvalue_ref<T>& value)
	{
		return rvalue_ref<T>::move(value);
	}

	template<class T>
	rvalue_ref<T> move(const rvalue_ref<T>& value)
	{
		return rvalue_ref<T>::move(value);
	}
}

#define STDEX_NOT_COPYABLE \
	stdex::delete_implicit_copy_constructor stdex_icc_deleter;

#define STDEX_DELETE_ICC() stdex_icc_deleter(true)

#define STDEX_RV_REF(Type) stdex::rvalue_ref<Type>
#define STDEX_RV_REF_CONST(Type) stdex::rvalue_ref<Type const>

#ifdef _MSC_VER
	#pragma warning (pop)
#endif

#endif // _STDEX_MOVE_H
