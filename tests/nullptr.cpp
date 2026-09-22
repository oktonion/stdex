#include <iostream>

#include "../stdex/include/nullptr.h"

int main()
{
    std::cout << 
        "pointer is signed: " << (stdex::detail::nullptr_detail::_pointer_is_signed::value ? "true" : "false") << 
    std::endl;

    std::cout << 
        "nullptr as enum sizeof(" << sizeof(stdex::detail::nullptr_detail::_nullptr_t_as_enum) << 
        "), is convertable to ptr: " <<
            (stdex::detail::_nullptr_choose_as_class<false>::_as_enum::_is_convertable_to_ptr ? "true" : "false") <<
        ", is equal to void ptr sizeof(" << sizeof(void*) << "): " <<
            (stdex::detail::_nullptr_choose_as_class<false>::_as_enum::_equal_void_ptr ? "true" : "false") <<
        ", can be ct constant: " <<
            (stdex::detail::_nullptr_choose_as_class<false>::_as_enum::_can_be_ct_constant ? "true" : "false") <<
    std::endl;

    std::cout << 
        "nullptr as int sizeof(" << sizeof(stdex::detail::_nullptr_choose_as_enum<false>::type) <<
        "), nullptr as int is convertable to ptr: " <<
            (stdex::detail::_nullptr_choose_as_enum<false>::_as_int::_is_convertable_to_ptr ? "true" : "false") <<
        ", is equal to void ptr: " <<
            (stdex::detail::_nullptr_choose_as_enum<false>::_as_int::_equal_void_ptr ? "true" : "false") <<
        ", can be compared to ptr: " <<
            (stdex::detail::_nullptr_choose_as_enum<false>::_as_int::_can_be_compared_to_ptr ? "true" : "false") <<
    std::endl;
    
    return 0;
}
