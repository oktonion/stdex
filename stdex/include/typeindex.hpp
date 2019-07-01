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

#define DELETED_FUNCTION =delete
#define NOEXCEPT_FUNCTION noexcept

#else

#define DELETED_FUNCTION 
#define NOEXCEPT_FUNCTION throw()

#endif 

namespace stdex
{
    struct type_index
    {
        type_index(const type_info& _tinfo) NOEXCEPT_FUNCTION
            : _type_info_obj(&_tinfo) 
        { }

        bool
        operator==(const type_index& _other) const NOEXCEPT_FUNCTION
            { return *_type_info_obj == *_other._type_info_obj; }

        bool
        operator!=(const type_index& _other) const NOEXCEPT_FUNCTION
            { return *_type_info_obj != *_other._type_info_obj; }

        bool
        operator<(const type_index& _other) const NOEXCEPT_FUNCTION
            { return _type_info_obj->before(*_other._type_info_obj); }

        bool
        operator<=(const type_index& _other) const NOEXCEPT_FUNCTION
            { return !_other._type_info_obj->before(*_type_info_obj); }

        bool
        operator>(const type_index& _other) const NOEXCEPT_FUNCTION
            { return _other._type_info_obj->before(*_type_info_obj); }

        bool
        operator>=(const type_index& _other) const NOEXCEPT_FUNCTION
            { return !_type_info_obj->before(*_other._type_info_obj); }

        const char*
        name() const NOEXCEPT_FUNCTION
            { return _type_info_obj->name(); }
        
        /*std::size_t
        hash_code() const NOEXCEPT_FUNCTION
            { return _type_info_obj->hash_code(); }*/

    private:
        const type_info* _type_info_obj;
    };

    //template<class _Tp> struct hash;

    // stdex::hash specialization for type_index.
    /*template<>
    struct hash<stdex::type_index>
    {
        typedef std::size_t        result_type;
        typedef stdex::type_index  argument_type;

        std::size_t
        operator()(const type_index& _tinfo) const NOEXCEPT_FUNCTION
        { return _tinfo.hash_code(); }
    };*/

} // namespace stdex

#undef DELETED_FUNCTION
#undef NOEXCEPT_FUNCTION

#endif // _STDEX_TYPEINDEX_H