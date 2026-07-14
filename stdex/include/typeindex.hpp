#ifndef _STDEX_TYPEINDEX_H
#define _STDEX_TYPEINDEX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
/*none*/

// POSIX includes
/*none*/

// std includes
#include <typeinfo> // std::typeinfo
#include <cstddef> // std::size_t

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept

#else

#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif 

namespace stdex
{
    using std::type_info;

    struct type_index
    {
        type_index(const stdex::type_info& _tinfo) _STDEX_NOEXCEPT_FUNCTION
            : _type_info_obj(&_tinfo) 
        { }

        bool
        operator==(const type_index& _other) const _STDEX_NOEXCEPT_FUNCTION
            { return *_type_info_obj == *_other._type_info_obj; }

        bool
        operator!=(const type_index& _other) const _STDEX_NOEXCEPT_FUNCTION
            { return *_type_info_obj != *_other._type_info_obj; }

        bool
        operator<(const type_index& _other) const _STDEX_NOEXCEPT_FUNCTION
            { return !!_type_info_obj->before(*_other._type_info_obj); }

        bool
        operator<=(const type_index& _other) const _STDEX_NOEXCEPT_FUNCTION
            { return !_other._type_info_obj->before(*_type_info_obj); }

        bool
        operator>(const type_index& _other) const _STDEX_NOEXCEPT_FUNCTION
            { return !!_other._type_info_obj->before(*_type_info_obj); }

        bool
        operator>=(const type_index& _other) const _STDEX_NOEXCEPT_FUNCTION
            { return !_type_info_obj->before(*_other._type_info_obj); }

        const char*
        name() const _STDEX_NOEXCEPT_FUNCTION
            { return _type_info_obj->name(); }
        
        /*std::size_t
        hash_code() const _STDEX_NOEXCEPT_FUNCTION
            { return _type_info_obj->hash_code(); }*/

    private:
        const stdex::type_info* _type_info_obj;
    };

    //template<class _Tp> struct hash;

    // stdex::hash specialization for type_index.
    /*template<>
    struct hash<stdex::type_index>
    {
        typedef std::size_t        result_type;
        typedef stdex::type_index  argument_type;

        std::size_t
        operator()(const type_index& _tinfo) const _STDEX_NOEXCEPT_FUNCTION
        { return _tinfo.hash_code(); }
    };*/

} // namespace stdex

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_TYPEINDEX_H
