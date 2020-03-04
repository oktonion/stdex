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
    class rvalue_ref:
		public remove_const<T>::type
    {
		delete_implicit_copy_constructor icc_deleter;
		typedef typename remove_const<T>::type type;
	protected:
        explicit rvalue_ref(const type &ref) 
			: type(ref),
			icc_deleter(true),
            _ref(const_cast<type&>(ref)) 
		{ }
		
        type& _ref;
    public:
        explicit rvalue_ref(rvalue_ref &other)
			: type(other._ref),
			icc_deleter(true),
			_ref(other._ref) 
		{ }
        //T& get() const {return _ref;}
		//operator T&() const {return get();}
		//T* operator->() const {return &get();}
		//T& operator*() const {return get();}
		static rvalue_ref<type> move(type &value) 
		{
			return rvalue_ref<type>(value);
		}
		static const rvalue_ref<type> move(const type &value) 
		{
			return rvalue_ref<type>(value);
		}
		static rvalue_ref<type> move(const rvalue_ref<type>& value)
		{
			return rvalue_ref<type>::move(value._ref);
		}
    };

	template<class T>
	struct not_const_ref
	{
		T& ref;
		explicit not_const_ref(T& ref_): ref(ref_)
			{}
	};

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

#ifdef _MSC_VER
	#pragma warning (pop)
#endif

#endif // _STDEX_MOVE_H
