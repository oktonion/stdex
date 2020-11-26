// this header imitates C++11 template parameter pack
// usage: define _STDEX_PARAMETER_PACK_INCLUDE as path to your header file and include this header right after define
// 
// f.e.:
//
// #define _STDEX_PARAMETER_PACK_DEFINE
// #include "./parameter_pack.h"
// #undef _STDEX_PARAMETER_PACK_DEFINE
// 
// #define MY_FUNC(count)
//     template<_STDEX_TMPL_ARGS##count(/**/, /**/) /*class _Arg0T, ...class _ArgNT*/> 
//     void my_func( _STDEX_PARAMS_MAX(/**/, /**/) /*_Arg0T arg0, ..._ArgNT argN*/)
//     { }
//
// MY_FUNC(0) /*template<class _Arg0T> void my_func(_Arg0T arg0) { }*/
//
// #define _STDEX_PARAMETER_PACK_UNDEF
// #include "./parameter_pack.h"
// #undef _STDEX_PARAMETER_PACK_UNDEF

#ifdef _STDEX_PARAMETER_PACK_DEFINE

#define _STDEX_TMPL_ARGS_IMPL(prefix, count, postfix, params_type) class prefix params_type(count) postfix
#define _STDEX_TYPES_IMPL(prefix, count, postfix, params_type) prefix params_type(count) postfix
#define _STDEX_PARAMS_IMPL(prefix1, prefix2, count, postfix1, postfix2, params_type, params_args) prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(count) _STDEX_PARAMS_TYPE_POSTFIX(count) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(count) params_args(count) _STDEX_PARAMS_ARG_POSTFIX(count) postfix2
#define _STDEX_ARGS_IMPL(prefix, count, postfix, params_args) prefix params_args(count) postfix
#define _STDEX_REPEAT_TOKEN_IMPL(token) token

#define _STDEX_DELIM_DEFAULT ,
#define _STDEX_PARAMS_TYPE_DEFAULT(count) _Arg##count##T
#define _STDEX_PARAMS_ARG_DEFAULT(count) arg##count
#define _STDEX_PARAMS_TYPE_PREFIX_DEFAULT(count)
#define _STDEX_PARAMS_TYPE_POSTFIX_DEFAULT(count) 
#define _STDEX_PARAMS_ARG_PREFIX_DEFAULT(count) 
#define _STDEX_PARAMS_ARG_POSTFIX_DEFAULT(count)

//#define /**/ /**/
#define _STDEX_DELIM _STDEX_DELIM_DEFAULT
#define _STDEX_PARAMS_TYPE_PREFIX(count)  _STDEX_PARAMS_TYPE_PREFIX_DEFAULT(count) 
#define _STDEX_PARAMS_TYPE_POSTFIX(count) _STDEX_PARAMS_TYPE_POSTFIX_DEFAULT(count) 
#define _STDEX_PARAMS_ARG_PREFIX(count)   _STDEX_PARAMS_ARG_PREFIX_DEFAULT(count) 
#define _STDEX_PARAMS_ARG_POSTFIX(count)  _STDEX_PARAMS_ARG_POSTFIX_DEFAULT(count)

#define _STDEX_TMPL_ARGS(prefix, count, postfix, params_type) _STDEX_TMPL_ARGS_IMPL(prefix, count, postfix, params_type)
#define _STDEX_TYPES(prefix, count, postfix, params_type) _STDEX_TYPES_IMPL(prefix, count, postfix, params_type)
#define _STDEX_PARAMS(prefix1, prefix2, count, postfix1, postfix2, params_type, params_args) _STDEX_PARAMS_IMPL(prefix1, prefix2, count, postfix1, postfix2, params_type, params_args)
#define _STDEX_ARGS(prefix, count, postfix, params_args) _STDEX_ARGS_IMPL(prefix, count, postfix, params_args)

#define _STDEX_TMPL_ARGS0_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix
#define _STDEX_TYPES0_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix
#define _STDEX_PARAMS0_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2
#define _STDEX_ARGS0_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix
#define _STDEX_TMPL_ARGS0(prefix, postfix) _STDEX_TMPL_ARGS0_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES0(prefix, postfix) _STDEX_TYPES0_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS0(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS0_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS0(prefix, postfix) _STDEX_ARGS0_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN0(token) _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS1_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix
#define _STDEX_TYPES1_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix
#define _STDEX_PARAMS1_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2
#define _STDEX_ARGS1_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix
#define _STDEX_TMPL_ARGS1(prefix, postfix) _STDEX_TMPL_ARGS1_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES1(prefix, postfix) _STDEX_TYPES1_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS1(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS1_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS1(prefix, postfix) _STDEX_ARGS1_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN1(token) _STDEX_REPEAT_TOKEN0(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS2_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix
#define _STDEX_TYPES2_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix
#define _STDEX_PARAMS2_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2
#define _STDEX_ARGS2_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix
#define _STDEX_TMPL_ARGS2(prefix, postfix) _STDEX_TMPL_ARGS2_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES2(prefix, postfix) _STDEX_TYPES2_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS2(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS2_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS2(prefix, postfix) _STDEX_ARGS2_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN2(token) _STDEX_REPEAT_TOKEN1(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS3_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix
#define _STDEX_TYPES3_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix
#define _STDEX_PARAMS3_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2
#define _STDEX_ARGS3_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix
#define _STDEX_TMPL_ARGS3(prefix, postfix) _STDEX_TMPL_ARGS3_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES3(prefix, postfix) _STDEX_TYPES3_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS3(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS3_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS3(prefix, postfix) _STDEX_ARGS3_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN3(token) _STDEX_REPEAT_TOKEN2(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS4_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix
#define _STDEX_TYPES4_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix
#define _STDEX_PARAMS4_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2
#define _STDEX_ARGS4_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix
#define _STDEX_TMPL_ARGS4(prefix, postfix) _STDEX_TMPL_ARGS4_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES4(prefix, postfix) _STDEX_TYPES4_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS4(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS4_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS4(prefix, postfix) _STDEX_ARGS4_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN4(token) _STDEX_REPEAT_TOKEN3(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS5_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix
#define _STDEX_TYPES5_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix
#define _STDEX_PARAMS5_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2
#define _STDEX_ARGS5_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix
#define _STDEX_TMPL_ARGS5(prefix, postfix) _STDEX_TMPL_ARGS5_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES5(prefix, postfix) _STDEX_TYPES5_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS5(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS5_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS5(prefix, postfix) _STDEX_ARGS5_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN5(token) _STDEX_REPEAT_TOKEN4(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS6_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix
#define _STDEX_TYPES6_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix
#define _STDEX_PARAMS6_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2
#define _STDEX_ARGS6_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix
#define _STDEX_TMPL_ARGS6(prefix, postfix) _STDEX_TMPL_ARGS6_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES6(prefix, postfix) _STDEX_TYPES6_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS6(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS6_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS6(prefix, postfix) _STDEX_ARGS6_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN6(token) _STDEX_REPEAT_TOKEN5(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS7_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix
#define _STDEX_TYPES7_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix
#define _STDEX_PARAMS7_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2
#define _STDEX_ARGS7_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix
#define _STDEX_TMPL_ARGS7(prefix, postfix) _STDEX_TMPL_ARGS7_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES7(prefix, postfix) _STDEX_TYPES7_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS7(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS7_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS7(prefix, postfix) _STDEX_ARGS7_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN7(token) _STDEX_REPEAT_TOKEN6(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS8_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix
#define _STDEX_TYPES8_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix
#define _STDEX_PARAMS8_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2
#define _STDEX_ARGS8_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix
#define _STDEX_TMPL_ARGS8(prefix, postfix) _STDEX_TMPL_ARGS8_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES8(prefix, postfix) _STDEX_TYPES8_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS8(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS8_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS8(prefix, postfix) _STDEX_ARGS8_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN8(token) _STDEX_REPEAT_TOKEN7(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS9_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix
#define _STDEX_TYPES9_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix
#define _STDEX_PARAMS9_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2
#define _STDEX_ARGS9_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix
#define _STDEX_TMPL_ARGS9(prefix, postfix) _STDEX_TMPL_ARGS9_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES9(prefix, postfix) _STDEX_TYPES9_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS9(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS9_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS9(prefix, postfix) _STDEX_ARGS9_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN9(token) _STDEX_REPEAT_TOKEN8(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS10_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix
#define _STDEX_TYPES10_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix
#define _STDEX_PARAMS10_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2
#define _STDEX_ARGS10_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix
#define _STDEX_TMPL_ARGS10(prefix, postfix) _STDEX_TMPL_ARGS10_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES10(prefix, postfix) _STDEX_TYPES10_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS10(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS10_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS10(prefix, postfix) _STDEX_ARGS10_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN10(token) _STDEX_REPEAT_TOKEN9(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS11_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix
#define _STDEX_TYPES11_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix
#define _STDEX_PARAMS11_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2
#define _STDEX_ARGS11_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix
#define _STDEX_TMPL_ARGS11(prefix, postfix) _STDEX_TMPL_ARGS11_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES11(prefix, postfix) _STDEX_TYPES11_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS11(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS11_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS11(prefix, postfix) _STDEX_ARGS11_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN11(token) _STDEX_REPEAT_TOKEN10(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS12_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix
#define _STDEX_TYPES12_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix
#define _STDEX_PARAMS12_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2
#define _STDEX_ARGS12_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix
#define _STDEX_TMPL_ARGS12(prefix, postfix) _STDEX_TMPL_ARGS12_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES12(prefix, postfix) _STDEX_TYPES12_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS12(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS12_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS12(prefix, postfix) _STDEX_ARGS12_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN12(token) _STDEX_REPEAT_TOKEN11(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS13_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix
#define _STDEX_TYPES13_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix
#define _STDEX_PARAMS13_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2
#define _STDEX_ARGS13_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix
#define _STDEX_TMPL_ARGS13(prefix, postfix) _STDEX_TMPL_ARGS13_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES13(prefix, postfix) _STDEX_TYPES13_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS13(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS13_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS13(prefix, postfix) _STDEX_ARGS13_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN13(token) _STDEX_REPEAT_TOKEN12(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS14_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix
#define _STDEX_TYPES14_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix
#define _STDEX_PARAMS14_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2
#define _STDEX_ARGS14_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix
#define _STDEX_TMPL_ARGS14(prefix, postfix) _STDEX_TMPL_ARGS14_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES14(prefix, postfix) _STDEX_TYPES14_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS14(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS14_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS14(prefix, postfix) _STDEX_ARGS14_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN14(token) _STDEX_REPEAT_TOKEN13(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS15_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix
#define _STDEX_TYPES15_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix
#define _STDEX_PARAMS15_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2
#define _STDEX_ARGS15_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix
#define _STDEX_TMPL_ARGS15(prefix, postfix) _STDEX_TMPL_ARGS15_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES15(prefix, postfix) _STDEX_TYPES15_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS15(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS15_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS15(prefix, postfix) _STDEX_ARGS15_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN15(token) _STDEX_REPEAT_TOKEN14(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS16_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix
#define _STDEX_TYPES16_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix
#define _STDEX_PARAMS16_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2
#define _STDEX_ARGS16_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix
#define _STDEX_TMPL_ARGS16(prefix, postfix) _STDEX_TMPL_ARGS16_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES16(prefix, postfix) _STDEX_TYPES16_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS16(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS16_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS16(prefix, postfix) _STDEX_ARGS16_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN16(token) _STDEX_REPEAT_TOKEN15(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS17_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix
#define _STDEX_TYPES17_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix
#define _STDEX_PARAMS17_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2
#define _STDEX_ARGS17_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix
#define _STDEX_TMPL_ARGS17(prefix, postfix) _STDEX_TMPL_ARGS17_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES17(prefix, postfix) _STDEX_TYPES17_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS17(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS17_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS17(prefix, postfix) _STDEX_ARGS17_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN17(token) _STDEX_REPEAT_TOKEN16(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS18_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix
#define _STDEX_TYPES18_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix
#define _STDEX_PARAMS18_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2
#define _STDEX_ARGS18_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix
#define _STDEX_TMPL_ARGS18(prefix, postfix) _STDEX_TMPL_ARGS18_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES18(prefix, postfix) _STDEX_TYPES18_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS18(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS18_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS18(prefix, postfix) _STDEX_ARGS18_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN18(token) _STDEX_REPEAT_TOKEN17(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS19_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix
#define _STDEX_TYPES19_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix
#define _STDEX_PARAMS19_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2
#define _STDEX_ARGS19_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix
#define _STDEX_TMPL_ARGS19(prefix, postfix) _STDEX_TMPL_ARGS19_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES19(prefix, postfix) _STDEX_TYPES19_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS19(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS19_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS19(prefix, postfix) _STDEX_ARGS19_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN19(token) _STDEX_REPEAT_TOKEN18(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS20_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix
#define _STDEX_TYPES20_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix
#define _STDEX_PARAMS20_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2
#define _STDEX_ARGS20_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix
#define _STDEX_TMPL_ARGS20(prefix, postfix) _STDEX_TMPL_ARGS20_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES20(prefix, postfix) _STDEX_TYPES20_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS20(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS20_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS20(prefix, postfix) _STDEX_ARGS20_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN20(token) _STDEX_REPEAT_TOKEN19(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS21_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix
#define _STDEX_TYPES21_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix
#define _STDEX_PARAMS21_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2
#define _STDEX_ARGS21_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix
#define _STDEX_TMPL_ARGS21(prefix, postfix) _STDEX_TMPL_ARGS21_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES21(prefix, postfix) _STDEX_TYPES21_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS21(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS21_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS21(prefix, postfix) _STDEX_ARGS21_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN21(token) _STDEX_REPEAT_TOKEN20(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS22_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix _STDEX_DELIM  class prefix params_type(22) postfix
#define _STDEX_TYPES22_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix _STDEX_DELIM  prefix params_type(22) postfix
#define _STDEX_PARAMS22_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(22) _STDEX_PARAMS_TYPE_POSTFIX(22) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(22) params_args(22) _STDEX_PARAMS_ARG_POSTFIX(22) postfix2
#define _STDEX_ARGS22_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix _STDEX_DELIM  prefix params_args(22) postfix
#define _STDEX_TMPL_ARGS22(prefix, postfix) _STDEX_TMPL_ARGS22_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES22(prefix, postfix) _STDEX_TYPES22_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS22(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS22_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS22(prefix, postfix) _STDEX_ARGS22_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN22(token) _STDEX_REPEAT_TOKEN21(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS23_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix _STDEX_DELIM  class prefix params_type(22) postfix _STDEX_DELIM \
        class prefix params_type(23) postfix
#define _STDEX_TYPES23_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix _STDEX_DELIM  prefix params_type(22) postfix _STDEX_DELIM \
        prefix params_type(23) postfix
#define _STDEX_PARAMS23_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(22) _STDEX_PARAMS_TYPE_POSTFIX(22) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(22) params_args(22) _STDEX_PARAMS_ARG_POSTFIX(22) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(23) _STDEX_PARAMS_TYPE_POSTFIX(23) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(23) params_args(23) _STDEX_PARAMS_ARG_POSTFIX(23) postfix2
#define _STDEX_ARGS23_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix _STDEX_DELIM  prefix params_args(22) postfix _STDEX_DELIM \
        prefix params_args(23) postfix
#define _STDEX_TMPL_ARGS23(prefix, postfix) _STDEX_TMPL_ARGS23_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES23(prefix, postfix) _STDEX_TYPES23_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS23(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS23_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS23(prefix, postfix) _STDEX_ARGS23_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN23(token) _STDEX_REPEAT_TOKEN22(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS24_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix _STDEX_DELIM  class prefix params_type(22) postfix _STDEX_DELIM \
        class prefix params_type(23) postfix _STDEX_DELIM  class prefix params_type(24) postfix
#define _STDEX_TYPES24_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix _STDEX_DELIM  prefix params_type(22) postfix _STDEX_DELIM \
        prefix params_type(23) postfix _STDEX_DELIM  prefix params_type(24) postfix
#define _STDEX_PARAMS24_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(22) _STDEX_PARAMS_TYPE_POSTFIX(22) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(22) params_args(22) _STDEX_PARAMS_ARG_POSTFIX(22) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(23) _STDEX_PARAMS_TYPE_POSTFIX(23) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(23) params_args(23) _STDEX_PARAMS_ARG_POSTFIX(23) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(24) _STDEX_PARAMS_TYPE_POSTFIX(24) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(24) params_args(24) _STDEX_PARAMS_ARG_POSTFIX(24) postfix2
#define _STDEX_ARGS24_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix _STDEX_DELIM  prefix params_args(22) postfix _STDEX_DELIM \
        prefix params_args(23) postfix _STDEX_DELIM  prefix params_args(24) postfix
#define _STDEX_TMPL_ARGS24(prefix, postfix) _STDEX_TMPL_ARGS24_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES24(prefix, postfix) _STDEX_TYPES24_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS24(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS24_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS24(prefix, postfix) _STDEX_ARGS24_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN24(token) _STDEX_REPEAT_TOKEN23(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS25_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix _STDEX_DELIM  class prefix params_type(22) postfix _STDEX_DELIM \
        class prefix params_type(23) postfix _STDEX_DELIM  class prefix params_type(24) postfix _STDEX_DELIM  class prefix params_type(25) postfix
#define _STDEX_TYPES25_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix _STDEX_DELIM  prefix params_type(22) postfix _STDEX_DELIM \
        prefix params_type(23) postfix _STDEX_DELIM  prefix params_type(24) postfix _STDEX_DELIM  prefix params_type(25) postfix
#define _STDEX_PARAMS25_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(22) _STDEX_PARAMS_TYPE_POSTFIX(22) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(22) params_args(22) _STDEX_PARAMS_ARG_POSTFIX(22) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(23) _STDEX_PARAMS_TYPE_POSTFIX(23) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(23) params_args(23) _STDEX_PARAMS_ARG_POSTFIX(23) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(24) _STDEX_PARAMS_TYPE_POSTFIX(24) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(24) params_args(24) _STDEX_PARAMS_ARG_POSTFIX(24) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(25) _STDEX_PARAMS_TYPE_POSTFIX(25) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(25) params_args(25) _STDEX_PARAMS_ARG_POSTFIX(25) postfix2
#define _STDEX_ARGS25_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix _STDEX_DELIM  prefix params_args(22) postfix _STDEX_DELIM \
        prefix params_args(23) postfix _STDEX_DELIM  prefix params_args(24) postfix _STDEX_DELIM  prefix params_args(25) postfix
#define _STDEX_TMPL_ARGS25(prefix, postfix) _STDEX_TMPL_ARGS25_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES25(prefix, postfix) _STDEX_TYPES25_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS25(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS25_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS25(prefix, postfix) _STDEX_ARGS25_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN25(token) _STDEX_REPEAT_TOKEN24(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS26_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix _STDEX_DELIM  class prefix params_type(22) postfix _STDEX_DELIM \
        class prefix params_type(23) postfix _STDEX_DELIM  class prefix params_type(24) postfix _STDEX_DELIM  class prefix params_type(25) postfix _STDEX_DELIM  class prefix params_type(26) postfix
#define _STDEX_TYPES26_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix _STDEX_DELIM  prefix params_type(22) postfix _STDEX_DELIM \
        prefix params_type(23) postfix _STDEX_DELIM  prefix params_type(24) postfix _STDEX_DELIM  prefix params_type(25) postfix _STDEX_DELIM  prefix params_type(26) postfix
#define _STDEX_PARAMS26_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(22) _STDEX_PARAMS_TYPE_POSTFIX(22) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(22) params_args(22) _STDEX_PARAMS_ARG_POSTFIX(22) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(23) _STDEX_PARAMS_TYPE_POSTFIX(23) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(23) params_args(23) _STDEX_PARAMS_ARG_POSTFIX(23) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(24) _STDEX_PARAMS_TYPE_POSTFIX(24) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(24) params_args(24) _STDEX_PARAMS_ARG_POSTFIX(24) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(25) _STDEX_PARAMS_TYPE_POSTFIX(25) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(25) params_args(25) _STDEX_PARAMS_ARG_POSTFIX(25) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(26) _STDEX_PARAMS_TYPE_POSTFIX(26) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(26) params_args(26) _STDEX_PARAMS_ARG_POSTFIX(26) postfix2
#define _STDEX_ARGS26_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix _STDEX_DELIM  prefix params_args(22) postfix _STDEX_DELIM \
        prefix params_args(23) postfix _STDEX_DELIM  prefix params_args(24) postfix _STDEX_DELIM  prefix params_args(25) postfix _STDEX_DELIM  prefix params_args(26) postfix
#define _STDEX_TMPL_ARGS26(prefix, postfix) _STDEX_TMPL_ARGS26_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES26(prefix, postfix) _STDEX_TYPES26_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS26(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS26_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS26(prefix, postfix) _STDEX_ARGS26_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN26(token) _STDEX_REPEAT_TOKEN25(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS27_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix _STDEX_DELIM  class prefix params_type(22) postfix _STDEX_DELIM \
        class prefix params_type(23) postfix _STDEX_DELIM  class prefix params_type(24) postfix _STDEX_DELIM  class prefix params_type(25) postfix _STDEX_DELIM  class prefix params_type(26) postfix _STDEX_DELIM \
        class prefix params_type(27) postfix
#define _STDEX_TYPES27_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix _STDEX_DELIM  prefix params_type(22) postfix _STDEX_DELIM \
        prefix params_type(23) postfix _STDEX_DELIM  prefix params_type(24) postfix _STDEX_DELIM  prefix params_type(25) postfix _STDEX_DELIM  prefix params_type(26) postfix _STDEX_DELIM \
        prefix params_type(27) postfix
#define _STDEX_PARAMS27_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(22) _STDEX_PARAMS_TYPE_POSTFIX(22) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(22) params_args(22) _STDEX_PARAMS_ARG_POSTFIX(22) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(23) _STDEX_PARAMS_TYPE_POSTFIX(23) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(23) params_args(23) _STDEX_PARAMS_ARG_POSTFIX(23) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(24) _STDEX_PARAMS_TYPE_POSTFIX(24) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(24) params_args(24) _STDEX_PARAMS_ARG_POSTFIX(24) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(25) _STDEX_PARAMS_TYPE_POSTFIX(25) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(25) params_args(25) _STDEX_PARAMS_ARG_POSTFIX(25) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(26) _STDEX_PARAMS_TYPE_POSTFIX(26) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(26) params_args(26) _STDEX_PARAMS_ARG_POSTFIX(26) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(27) _STDEX_PARAMS_TYPE_POSTFIX(27) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(27) params_args(27) _STDEX_PARAMS_ARG_POSTFIX(27) postfix2
#define _STDEX_ARGS27_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix _STDEX_DELIM  prefix params_args(22) postfix _STDEX_DELIM \
        prefix params_args(23) postfix _STDEX_DELIM  prefix params_args(24) postfix _STDEX_DELIM  prefix params_args(25) postfix _STDEX_DELIM  prefix params_args(26) postfix _STDEX_DELIM \
        prefix params_args(27) postfix
#define _STDEX_TMPL_ARGS27(prefix, postfix) _STDEX_TMPL_ARGS27_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES27(prefix, postfix) _STDEX_TYPES27_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS27(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS27_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS27(prefix, postfix) _STDEX_ARGS27_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN27(token) _STDEX_REPEAT_TOKEN26(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS28_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix _STDEX_DELIM  class prefix params_type(22) postfix _STDEX_DELIM \
        class prefix params_type(23) postfix _STDEX_DELIM  class prefix params_type(24) postfix _STDEX_DELIM  class prefix params_type(25) postfix _STDEX_DELIM  class prefix params_type(26) postfix _STDEX_DELIM \
        class prefix params_type(27) postfix _STDEX_DELIM  class prefix params_type(28) postfix
#define _STDEX_TYPES28_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix _STDEX_DELIM  prefix params_type(22) postfix _STDEX_DELIM \
        prefix params_type(23) postfix _STDEX_DELIM  prefix params_type(24) postfix _STDEX_DELIM  prefix params_type(25) postfix _STDEX_DELIM  prefix params_type(26) postfix _STDEX_DELIM \
        prefix params_type(27) postfix _STDEX_DELIM  prefix params_type(28) postfix
#define _STDEX_PARAMS28_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(22) _STDEX_PARAMS_TYPE_POSTFIX(22) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(22) params_args(22) _STDEX_PARAMS_ARG_POSTFIX(22) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(23) _STDEX_PARAMS_TYPE_POSTFIX(23) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(23) params_args(23) _STDEX_PARAMS_ARG_POSTFIX(23) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(24) _STDEX_PARAMS_TYPE_POSTFIX(24) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(24) params_args(24) _STDEX_PARAMS_ARG_POSTFIX(24) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(25) _STDEX_PARAMS_TYPE_POSTFIX(25) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(25) params_args(25) _STDEX_PARAMS_ARG_POSTFIX(25) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(26) _STDEX_PARAMS_TYPE_POSTFIX(26) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(26) params_args(26) _STDEX_PARAMS_ARG_POSTFIX(26) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(27) _STDEX_PARAMS_TYPE_POSTFIX(27) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(27) params_args(27) _STDEX_PARAMS_ARG_POSTFIX(27) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(28) _STDEX_PARAMS_TYPE_POSTFIX(28) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(28) params_args(28) _STDEX_PARAMS_ARG_POSTFIX(28) postfix2
#define _STDEX_ARGS28_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix _STDEX_DELIM  prefix params_args(22) postfix _STDEX_DELIM \
        prefix params_args(23) postfix _STDEX_DELIM  prefix params_args(24) postfix _STDEX_DELIM  prefix params_args(25) postfix _STDEX_DELIM  prefix params_args(26) postfix _STDEX_DELIM \
        prefix params_args(27) postfix _STDEX_DELIM  prefix params_args(28) postfix
#define _STDEX_TMPL_ARGS28(prefix, postfix) _STDEX_TMPL_ARGS28_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES28(prefix, postfix) _STDEX_TYPES28_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS28(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS28_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS28(prefix, postfix) _STDEX_ARGS28_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN28(token) _STDEX_REPEAT_TOKEN27(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS29_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix _STDEX_DELIM  class prefix params_type(22) postfix _STDEX_DELIM \
        class prefix params_type(23) postfix _STDEX_DELIM  class prefix params_type(24) postfix _STDEX_DELIM  class prefix params_type(25) postfix _STDEX_DELIM  class prefix params_type(26) postfix _STDEX_DELIM \
        class prefix params_type(27) postfix _STDEX_DELIM  class prefix params_type(28) postfix _STDEX_DELIM  class prefix params_type(29) postfix
#define _STDEX_TYPES29_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix _STDEX_DELIM  prefix params_type(22) postfix _STDEX_DELIM \
        prefix params_type(23) postfix _STDEX_DELIM  prefix params_type(24) postfix _STDEX_DELIM  prefix params_type(25) postfix _STDEX_DELIM  prefix params_type(26) postfix _STDEX_DELIM \
        prefix params_type(27) postfix _STDEX_DELIM  prefix params_type(28) postfix _STDEX_DELIM  prefix params_type(29) postfix
#define _STDEX_PARAMS29_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(22) _STDEX_PARAMS_TYPE_POSTFIX(22) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(22) params_args(22) _STDEX_PARAMS_ARG_POSTFIX(22) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(23) _STDEX_PARAMS_TYPE_POSTFIX(23) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(23) params_args(23) _STDEX_PARAMS_ARG_POSTFIX(23) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(24) _STDEX_PARAMS_TYPE_POSTFIX(24) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(24) params_args(24) _STDEX_PARAMS_ARG_POSTFIX(24) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(25) _STDEX_PARAMS_TYPE_POSTFIX(25) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(25) params_args(25) _STDEX_PARAMS_ARG_POSTFIX(25) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(26) _STDEX_PARAMS_TYPE_POSTFIX(26) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(26) params_args(26) _STDEX_PARAMS_ARG_POSTFIX(26) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(27) _STDEX_PARAMS_TYPE_POSTFIX(27) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(27) params_args(27) _STDEX_PARAMS_ARG_POSTFIX(27) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(28) _STDEX_PARAMS_TYPE_POSTFIX(28) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(28) params_args(28) _STDEX_PARAMS_ARG_POSTFIX(28) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(29) _STDEX_PARAMS_TYPE_POSTFIX(29) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(29) params_args(29) _STDEX_PARAMS_ARG_POSTFIX(29) postfix2
#define _STDEX_ARGS29_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix _STDEX_DELIM  prefix params_args(22) postfix _STDEX_DELIM \
        prefix params_args(23) postfix _STDEX_DELIM  prefix params_args(24) postfix _STDEX_DELIM  prefix params_args(25) postfix _STDEX_DELIM  prefix params_args(26) postfix _STDEX_DELIM \
        prefix params_args(27) postfix _STDEX_DELIM  prefix params_args(28) postfix _STDEX_DELIM  prefix params_args(29) postfix
#define _STDEX_TMPL_ARGS29(prefix, postfix) _STDEX_TMPL_ARGS29_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES29(prefix, postfix) _STDEX_TYPES29_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS29(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS29_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS29(prefix, postfix) _STDEX_ARGS29_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN29(token) _STDEX_REPEAT_TOKEN28(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS30_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix _STDEX_DELIM  class prefix params_type(22) postfix _STDEX_DELIM \
        class prefix params_type(23) postfix _STDEX_DELIM  class prefix params_type(24) postfix _STDEX_DELIM  class prefix params_type(25) postfix _STDEX_DELIM  class prefix params_type(26) postfix _STDEX_DELIM \
        class prefix params_type(27) postfix _STDEX_DELIM  class prefix params_type(28) postfix _STDEX_DELIM  class prefix params_type(29) postfix _STDEX_DELIM  class prefix params_type(30) postfix
#define _STDEX_TYPES30_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix _STDEX_DELIM  prefix params_type(22) postfix _STDEX_DELIM \
        prefix params_type(23) postfix _STDEX_DELIM  prefix params_type(24) postfix _STDEX_DELIM  prefix params_type(25) postfix _STDEX_DELIM  prefix params_type(26) postfix _STDEX_DELIM \
        prefix params_type(27) postfix _STDEX_DELIM  prefix params_type(28) postfix _STDEX_DELIM  prefix params_type(29) postfix _STDEX_DELIM  prefix params_type(30) postfix
#define _STDEX_PARAMS30_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(22) _STDEX_PARAMS_TYPE_POSTFIX(22) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(22) params_args(22) _STDEX_PARAMS_ARG_POSTFIX(22) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(23) _STDEX_PARAMS_TYPE_POSTFIX(23) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(23) params_args(23) _STDEX_PARAMS_ARG_POSTFIX(23) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(24) _STDEX_PARAMS_TYPE_POSTFIX(24) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(24) params_args(24) _STDEX_PARAMS_ARG_POSTFIX(24) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(25) _STDEX_PARAMS_TYPE_POSTFIX(25) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(25) params_args(25) _STDEX_PARAMS_ARG_POSTFIX(25) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(26) _STDEX_PARAMS_TYPE_POSTFIX(26) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(26) params_args(26) _STDEX_PARAMS_ARG_POSTFIX(26) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(27) _STDEX_PARAMS_TYPE_POSTFIX(27) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(27) params_args(27) _STDEX_PARAMS_ARG_POSTFIX(27) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(28) _STDEX_PARAMS_TYPE_POSTFIX(28) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(28) params_args(28) _STDEX_PARAMS_ARG_POSTFIX(28) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(29) _STDEX_PARAMS_TYPE_POSTFIX(29) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(29) params_args(29) _STDEX_PARAMS_ARG_POSTFIX(29) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(30) _STDEX_PARAMS_TYPE_POSTFIX(30) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(30) params_args(30) _STDEX_PARAMS_ARG_POSTFIX(30) postfix2
#define _STDEX_ARGS30_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix _STDEX_DELIM  prefix params_args(22) postfix _STDEX_DELIM \
        prefix params_args(23) postfix _STDEX_DELIM  prefix params_args(24) postfix _STDEX_DELIM  prefix params_args(25) postfix _STDEX_DELIM  prefix params_args(26) postfix _STDEX_DELIM \
        prefix params_args(27) postfix _STDEX_DELIM  prefix params_args(28) postfix _STDEX_DELIM  prefix params_args(29) postfix _STDEX_DELIM  prefix params_args(30) postfix
#define _STDEX_TMPL_ARGS30(prefix, postfix) _STDEX_TMPL_ARGS30_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES30(prefix, postfix) _STDEX_TYPES30_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS30(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS30_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS30(prefix, postfix) _STDEX_ARGS30_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN30(token) _STDEX_REPEAT_TOKEN29(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#define _STDEX_TMPL_ARGS31_IMPL(prefix, postfix, params_type)\
         class prefix params_type(0) postfix _STDEX_DELIM  class prefix params_type(1) postfix _STDEX_DELIM  class prefix params_type(2) postfix _STDEX_DELIM \
        class prefix params_type(3) postfix _STDEX_DELIM  class prefix params_type(4) postfix _STDEX_DELIM  class prefix params_type(5) postfix _STDEX_DELIM  class prefix params_type(6) postfix _STDEX_DELIM \
        class prefix params_type(7) postfix _STDEX_DELIM  class prefix params_type(8) postfix _STDEX_DELIM  class prefix params_type(9) postfix _STDEX_DELIM  class prefix params_type(10) postfix _STDEX_DELIM \
        class prefix params_type(11) postfix _STDEX_DELIM  class prefix params_type(12) postfix _STDEX_DELIM  class prefix params_type(13) postfix _STDEX_DELIM  class prefix params_type(14) postfix _STDEX_DELIM \
        class prefix params_type(15) postfix _STDEX_DELIM  class prefix params_type(16) postfix _STDEX_DELIM  class prefix params_type(17) postfix _STDEX_DELIM  class prefix params_type(18) postfix _STDEX_DELIM \
        class prefix params_type(19) postfix _STDEX_DELIM  class prefix params_type(20) postfix _STDEX_DELIM  class prefix params_type(21) postfix _STDEX_DELIM  class prefix params_type(22) postfix _STDEX_DELIM \
        class prefix params_type(23) postfix _STDEX_DELIM  class prefix params_type(24) postfix _STDEX_DELIM  class prefix params_type(25) postfix _STDEX_DELIM  class prefix params_type(26) postfix _STDEX_DELIM \
        class prefix params_type(27) postfix _STDEX_DELIM  class prefix params_type(28) postfix _STDEX_DELIM  class prefix params_type(29) postfix _STDEX_DELIM  class prefix params_type(30) postfix _STDEX_DELIM \
        class prefix params_type(31) postfix
#define _STDEX_TYPES31_IMPL(prefix, postfix, params_type)\
         prefix params_type(0) postfix _STDEX_DELIM  prefix params_type(1) postfix _STDEX_DELIM  prefix params_type(2) postfix _STDEX_DELIM \
        prefix params_type(3) postfix _STDEX_DELIM  prefix params_type(4) postfix _STDEX_DELIM  prefix params_type(5) postfix _STDEX_DELIM  prefix params_type(6) postfix _STDEX_DELIM \
        prefix params_type(7) postfix _STDEX_DELIM  prefix params_type(8) postfix _STDEX_DELIM  prefix params_type(9) postfix _STDEX_DELIM  prefix params_type(10) postfix _STDEX_DELIM \
        prefix params_type(11) postfix _STDEX_DELIM  prefix params_type(12) postfix _STDEX_DELIM  prefix params_type(13) postfix _STDEX_DELIM  prefix params_type(14) postfix _STDEX_DELIM \
        prefix params_type(15) postfix _STDEX_DELIM  prefix params_type(16) postfix _STDEX_DELIM  prefix params_type(17) postfix _STDEX_DELIM  prefix params_type(18) postfix _STDEX_DELIM \
        prefix params_type(19) postfix _STDEX_DELIM  prefix params_type(20) postfix _STDEX_DELIM  prefix params_type(21) postfix _STDEX_DELIM  prefix params_type(22) postfix _STDEX_DELIM \
        prefix params_type(23) postfix _STDEX_DELIM  prefix params_type(24) postfix _STDEX_DELIM  prefix params_type(25) postfix _STDEX_DELIM  prefix params_type(26) postfix _STDEX_DELIM \
        prefix params_type(27) postfix _STDEX_DELIM  prefix params_type(28) postfix _STDEX_DELIM  prefix params_type(29) postfix _STDEX_DELIM  prefix params_type(30) postfix _STDEX_DELIM \
        prefix params_type(31) postfix
#define _STDEX_PARAMS31_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args)\
         prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(0) _STDEX_PARAMS_TYPE_POSTFIX(0) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(0) params_args(0) _STDEX_PARAMS_ARG_POSTFIX(0) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(1) _STDEX_PARAMS_TYPE_POSTFIX(1) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(1) params_args(1) _STDEX_PARAMS_ARG_POSTFIX(1) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(2) _STDEX_PARAMS_TYPE_POSTFIX(2) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(2) params_args(2) _STDEX_PARAMS_ARG_POSTFIX(2) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(3) _STDEX_PARAMS_TYPE_POSTFIX(3) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(3) params_args(3) _STDEX_PARAMS_ARG_POSTFIX(3) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(4) _STDEX_PARAMS_TYPE_POSTFIX(4) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(4) params_args(4) _STDEX_PARAMS_ARG_POSTFIX(4) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(5) _STDEX_PARAMS_TYPE_POSTFIX(5) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(5) params_args(5) _STDEX_PARAMS_ARG_POSTFIX(5) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(6) _STDEX_PARAMS_TYPE_POSTFIX(6) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(6) params_args(6) _STDEX_PARAMS_ARG_POSTFIX(6) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(7) _STDEX_PARAMS_TYPE_POSTFIX(7) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(7) params_args(7) _STDEX_PARAMS_ARG_POSTFIX(7) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(8) _STDEX_PARAMS_TYPE_POSTFIX(8) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(8) params_args(8) _STDEX_PARAMS_ARG_POSTFIX(8) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(9) _STDEX_PARAMS_TYPE_POSTFIX(9) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(9) params_args(9) _STDEX_PARAMS_ARG_POSTFIX(9) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(10) _STDEX_PARAMS_TYPE_POSTFIX(10) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(10) params_args(10) _STDEX_PARAMS_ARG_POSTFIX(10) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(11) _STDEX_PARAMS_TYPE_POSTFIX(11) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(11) params_args(11) _STDEX_PARAMS_ARG_POSTFIX(11) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(12) _STDEX_PARAMS_TYPE_POSTFIX(12) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(12) params_args(12) _STDEX_PARAMS_ARG_POSTFIX(12) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(13) _STDEX_PARAMS_TYPE_POSTFIX(13) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(13) params_args(13) _STDEX_PARAMS_ARG_POSTFIX(13) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(14) _STDEX_PARAMS_TYPE_POSTFIX(14) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(14) params_args(14) _STDEX_PARAMS_ARG_POSTFIX(14) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(15) _STDEX_PARAMS_TYPE_POSTFIX(15) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(15) params_args(15) _STDEX_PARAMS_ARG_POSTFIX(15) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(16) _STDEX_PARAMS_TYPE_POSTFIX(16) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(16) params_args(16) _STDEX_PARAMS_ARG_POSTFIX(16) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(17) _STDEX_PARAMS_TYPE_POSTFIX(17) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(17) params_args(17) _STDEX_PARAMS_ARG_POSTFIX(17) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(18) _STDEX_PARAMS_TYPE_POSTFIX(18) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(18) params_args(18) _STDEX_PARAMS_ARG_POSTFIX(18) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(19) _STDEX_PARAMS_TYPE_POSTFIX(19) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(19) params_args(19) _STDEX_PARAMS_ARG_POSTFIX(19) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(20) _STDEX_PARAMS_TYPE_POSTFIX(20) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(20) params_args(20) _STDEX_PARAMS_ARG_POSTFIX(20) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(21) _STDEX_PARAMS_TYPE_POSTFIX(21) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(21) params_args(21) _STDEX_PARAMS_ARG_POSTFIX(21) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(22) _STDEX_PARAMS_TYPE_POSTFIX(22) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(22) params_args(22) _STDEX_PARAMS_ARG_POSTFIX(22) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(23) _STDEX_PARAMS_TYPE_POSTFIX(23) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(23) params_args(23) _STDEX_PARAMS_ARG_POSTFIX(23) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(24) _STDEX_PARAMS_TYPE_POSTFIX(24) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(24) params_args(24) _STDEX_PARAMS_ARG_POSTFIX(24) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(25) _STDEX_PARAMS_TYPE_POSTFIX(25) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(25) params_args(25) _STDEX_PARAMS_ARG_POSTFIX(25) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(26) _STDEX_PARAMS_TYPE_POSTFIX(26) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(26) params_args(26) _STDEX_PARAMS_ARG_POSTFIX(26) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(27) _STDEX_PARAMS_TYPE_POSTFIX(27) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(27) params_args(27) _STDEX_PARAMS_ARG_POSTFIX(27) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(28) _STDEX_PARAMS_TYPE_POSTFIX(28) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(28) params_args(28) _STDEX_PARAMS_ARG_POSTFIX(28) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(29) _STDEX_PARAMS_TYPE_POSTFIX(29) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(29) params_args(29) _STDEX_PARAMS_ARG_POSTFIX(29) postfix2 _STDEX_DELIM  prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(30) _STDEX_PARAMS_TYPE_POSTFIX(30) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(30) params_args(30) _STDEX_PARAMS_ARG_POSTFIX(30) postfix2 _STDEX_DELIM \
        prefix1 _STDEX_PARAMS_TYPE_PREFIX(count) params_type(31) _STDEX_PARAMS_TYPE_POSTFIX(31) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(31) params_args(31) _STDEX_PARAMS_ARG_POSTFIX(31) postfix2
#define _STDEX_ARGS31_IMPL(prefix, postfix, params_args)\
         prefix params_args(0) postfix _STDEX_DELIM  prefix params_args(1) postfix _STDEX_DELIM  prefix params_args(2) postfix _STDEX_DELIM \
        prefix params_args(3) postfix _STDEX_DELIM  prefix params_args(4) postfix _STDEX_DELIM  prefix params_args(5) postfix _STDEX_DELIM  prefix params_args(6) postfix _STDEX_DELIM \
        prefix params_args(7) postfix _STDEX_DELIM  prefix params_args(8) postfix _STDEX_DELIM  prefix params_args(9) postfix _STDEX_DELIM  prefix params_args(10) postfix _STDEX_DELIM \
        prefix params_args(11) postfix _STDEX_DELIM  prefix params_args(12) postfix _STDEX_DELIM  prefix params_args(13) postfix _STDEX_DELIM  prefix params_args(14) postfix _STDEX_DELIM \
        prefix params_args(15) postfix _STDEX_DELIM  prefix params_args(16) postfix _STDEX_DELIM  prefix params_args(17) postfix _STDEX_DELIM  prefix params_args(18) postfix _STDEX_DELIM \
        prefix params_args(19) postfix _STDEX_DELIM  prefix params_args(20) postfix _STDEX_DELIM  prefix params_args(21) postfix _STDEX_DELIM  prefix params_args(22) postfix _STDEX_DELIM \
        prefix params_args(23) postfix _STDEX_DELIM  prefix params_args(24) postfix _STDEX_DELIM  prefix params_args(25) postfix _STDEX_DELIM  prefix params_args(26) postfix _STDEX_DELIM \
        prefix params_args(27) postfix _STDEX_DELIM  prefix params_args(28) postfix _STDEX_DELIM  prefix params_args(29) postfix _STDEX_DELIM  prefix params_args(30) postfix _STDEX_DELIM \
        prefix params_args(31) postfix
#define _STDEX_TMPL_ARGS31(prefix, postfix) _STDEX_TMPL_ARGS31_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_TYPES31(prefix, postfix) _STDEX_TYPES31_IMPL(prefix, postfix, _STDEX_PARAMS_TYPE_DEFAULT)
#define _STDEX_PARAMS31(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS31_IMPL(prefix1, postfix1, prefix2, postfix2, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_ARGS31(prefix, postfix) _STDEX_ARGS31_IMPL(prefix, postfix, _STDEX_PARAMS_ARG_DEFAULT)
#define _STDEX_REPEAT_TOKEN31(token) _STDEX_REPEAT_TOKEN30(token) _STDEX_DELIM _STDEX_REPEAT_TOKEN_IMPL(token)

#ifndef STDEX_ARGS_MAX_COUNT
    #define STDEX_ARGS_MAX_COUNT 32
#endif

#define _STDEX_TMPL_ARGS_MAX_IMPL(prefix, postfix, params_type) \
    _STDEX_TMPL_ARGS31_IMPL (prefix, postfix, params_type)
#define _STDEX_TYPES_MAX_IMPL(prefix, postfix, params_type) \
    _STDEX_TYPES31_IMPL (prefix, postfix, params_type)
#define _STDEX_PARAMS_MAX_IMPL(prefix1, postfix1, prefix2, postfix2, params_type, params_args) \
    _STDEX_PARAMS31_IMPL (prefix1, postfix1, prefix2, postfix2, params_type, params_args)
#define _STDEX_ARGS_MAX_IMPL(prefix, postfix, params_args) \
    _STDEX_ARGS31_IMPL (prefix, postfix, params_args)

#define _STDEX_REPEAT_TOKEN_MAX(token) _STDEX_REPEAT_TOKEN31(token)
#define _STDEX_TMPL_ARGS_MAX(prefix, postfix) \
    _STDEX_TMPL_ARGS31(prefix, postfix)
#define _STDEX_TYPES_MAX(prefix, postfix) \
    _STDEX_TYPES31(prefix, postfix)
#define _STDEX_PARAMS_MAX(prefix1, postfix1, prefix2, postfix2) \
    _STDEX_PARAMS31(prefix1, postfix1, prefix2, postfix2)
#define _STDEX_ARGS_MAX(prefix, postfix) \
    _STDEX_ARGS31(prefix, postfix)

#endif // _STDEX_PARAMETER_PACK_DEFINE

#ifdef _STDEX_PARAMETER_PACK_UNDEF

#undef _STDEX_DELIM_DEFAULT
#undef _STDEX_PARAMS_TYPE_DEFAULT
#undef _STDEX_PARAMS_ARG_DEFAULT
#undef _STDEX_PARAMS_TYPE_PREFIX_DEFAULT
#undef _STDEX_PARAMS_TYPE_POSTFIX_DEFAULT
#undef _STDEX_PARAMS_ARG_PREFIX_DEFAULT
#undef _STDEX_PARAMS_ARG_POSTFIX_DEFAULT

//#undef /**/  
#undef _STDEX_DELIM
#undef _STDEX_PARAMS_TYPE_PREFIX
#undef _STDEX_PARAMS_TYPE_POSTFIX
#undef _STDEX_PARAMS_ARG_PREFIX
#undef _STDEX_PARAMS_ARG_POSTFIX 

#undef _STDEX_TMPL_ARGS_IMPL
#undef _STDEX_TYPES_IMPL
#undef _STDEX_PARAMS_IMPL
#undef _STDEX_ARGS_IMPL
#undef _STDEX_REPEAT_TOKEN_IMPL

#undef _STDEX_TMPL_ARGS
#undef _STDEX_TYPES
#undef _STDEX_PARAMS
#undef _STDEX_ARGS

#undef _STDEX_REPEAT_TOKEN_MAX
#undef _STDEX_TMPL_ARGS_MAX_IMPL
#undef _STDEX_TYPES_MAX_IMPL
#undef _STDEX_PARAMS_MAX_IMPL
#undef _STDEX_ARGS_MAX_IMPL

#undef _STDEX_TMPL_ARGS_MAX
#undef _STDEX_TYPES_MAX
#undef _STDEX_PARAMS_MAX
#undef _STDEX_ARGS_MAX

#undef       _STDEX_TMPL_ARGS0_IMPL
#undef           _STDEX_TYPES0_IMPL
#undef          _STDEX_PARAMS0_IMPL
#undef            _STDEX_ARGS0_IMPL
#undef       _STDEX_TMPL_ARGS0
#undef           _STDEX_TYPES0
#undef          _STDEX_PARAMS0
#undef            _STDEX_ARGS0
#undef    _STDEX_REPEAT_TOKEN0
#undef       _STDEX_TMPL_ARGS1_IMPL
#undef           _STDEX_TYPES1_IMPL
#undef          _STDEX_PARAMS1_IMPL
#undef            _STDEX_ARGS1_IMPL
#undef       _STDEX_TMPL_ARGS1
#undef           _STDEX_TYPES1
#undef          _STDEX_PARAMS1
#undef            _STDEX_ARGS1
#undef    _STDEX_REPEAT_TOKEN1
#undef       _STDEX_TMPL_ARGS2_IMPL
#undef           _STDEX_TYPES2_IMPL
#undef          _STDEX_PARAMS2_IMPL
#undef            _STDEX_ARGS2_IMPL
#undef       _STDEX_TMPL_ARGS2
#undef           _STDEX_TYPES2
#undef          _STDEX_PARAMS2
#undef            _STDEX_ARGS2
#undef    _STDEX_REPEAT_TOKEN2
#undef       _STDEX_TMPL_ARGS3_IMPL
#undef           _STDEX_TYPES3_IMPL
#undef          _STDEX_PARAMS3_IMPL
#undef            _STDEX_ARGS3_IMPL
#undef       _STDEX_TMPL_ARGS3
#undef           _STDEX_TYPES3
#undef          _STDEX_PARAMS3
#undef            _STDEX_ARGS3
#undef    _STDEX_REPEAT_TOKEN3
#undef       _STDEX_TMPL_ARGS4_IMPL
#undef           _STDEX_TYPES4_IMPL
#undef          _STDEX_PARAMS4_IMPL
#undef            _STDEX_ARGS4_IMPL
#undef       _STDEX_TMPL_ARGS4
#undef           _STDEX_TYPES4
#undef          _STDEX_PARAMS4
#undef            _STDEX_ARGS4
#undef    _STDEX_REPEAT_TOKEN4
#undef       _STDEX_TMPL_ARGS5_IMPL
#undef           _STDEX_TYPES5_IMPL
#undef          _STDEX_PARAMS5_IMPL
#undef            _STDEX_ARGS5_IMPL
#undef       _STDEX_TMPL_ARGS5
#undef           _STDEX_TYPES5
#undef          _STDEX_PARAMS5
#undef            _STDEX_ARGS5
#undef    _STDEX_REPEAT_TOKEN5
#undef       _STDEX_TMPL_ARGS6_IMPL
#undef           _STDEX_TYPES6_IMPL
#undef          _STDEX_PARAMS6_IMPL
#undef            _STDEX_ARGS6_IMPL
#undef       _STDEX_TMPL_ARGS6
#undef           _STDEX_TYPES6
#undef          _STDEX_PARAMS6
#undef            _STDEX_ARGS6
#undef    _STDEX_REPEAT_TOKEN6
#undef       _STDEX_TMPL_ARGS7_IMPL
#undef           _STDEX_TYPES7_IMPL
#undef          _STDEX_PARAMS7_IMPL
#undef            _STDEX_ARGS7_IMPL
#undef       _STDEX_TMPL_ARGS7
#undef           _STDEX_TYPES7
#undef          _STDEX_PARAMS7
#undef            _STDEX_ARGS7
#undef    _STDEX_REPEAT_TOKEN7
#undef       _STDEX_TMPL_ARGS8_IMPL
#undef           _STDEX_TYPES8_IMPL
#undef          _STDEX_PARAMS8_IMPL
#undef            _STDEX_ARGS8_IMPL
#undef       _STDEX_TMPL_ARGS8
#undef           _STDEX_TYPES8
#undef          _STDEX_PARAMS8
#undef            _STDEX_ARGS8
#undef    _STDEX_REPEAT_TOKEN8
#undef       _STDEX_TMPL_ARGS9_IMPL
#undef           _STDEX_TYPES9_IMPL
#undef          _STDEX_PARAMS9_IMPL
#undef            _STDEX_ARGS9_IMPL
#undef       _STDEX_TMPL_ARGS9
#undef           _STDEX_TYPES9
#undef          _STDEX_PARAMS9
#undef            _STDEX_ARGS9
#undef    _STDEX_REPEAT_TOKEN9
#undef       _STDEX_TMPL_ARGS10_IMPL
#undef           _STDEX_TYPES10_IMPL
#undef          _STDEX_PARAMS10_IMPL
#undef            _STDEX_ARGS10_IMPL
#undef       _STDEX_TMPL_ARGS10
#undef           _STDEX_TYPES10
#undef          _STDEX_PARAMS10
#undef            _STDEX_ARGS10
#undef    _STDEX_REPEAT_TOKEN10
#undef       _STDEX_TMPL_ARGS11_IMPL
#undef           _STDEX_TYPES11_IMPL
#undef          _STDEX_PARAMS11_IMPL
#undef            _STDEX_ARGS11_IMPL
#undef       _STDEX_TMPL_ARGS11
#undef           _STDEX_TYPES11
#undef          _STDEX_PARAMS11
#undef            _STDEX_ARGS11
#undef    _STDEX_REPEAT_TOKEN11
#undef       _STDEX_TMPL_ARGS12_IMPL
#undef           _STDEX_TYPES12_IMPL
#undef          _STDEX_PARAMS12_IMPL
#undef            _STDEX_ARGS12_IMPL
#undef       _STDEX_TMPL_ARGS12
#undef           _STDEX_TYPES12
#undef          _STDEX_PARAMS12
#undef            _STDEX_ARGS12
#undef    _STDEX_REPEAT_TOKEN12
#undef       _STDEX_TMPL_ARGS13_IMPL
#undef           _STDEX_TYPES13_IMPL
#undef          _STDEX_PARAMS13_IMPL
#undef            _STDEX_ARGS13_IMPL
#undef       _STDEX_TMPL_ARGS13
#undef           _STDEX_TYPES13
#undef          _STDEX_PARAMS13
#undef            _STDEX_ARGS13
#undef    _STDEX_REPEAT_TOKEN13
#undef       _STDEX_TMPL_ARGS14_IMPL
#undef           _STDEX_TYPES14_IMPL
#undef          _STDEX_PARAMS14_IMPL
#undef            _STDEX_ARGS14_IMPL
#undef       _STDEX_TMPL_ARGS14
#undef           _STDEX_TYPES14
#undef          _STDEX_PARAMS14
#undef            _STDEX_ARGS14
#undef    _STDEX_REPEAT_TOKEN14
#undef       _STDEX_TMPL_ARGS15_IMPL
#undef           _STDEX_TYPES15_IMPL
#undef          _STDEX_PARAMS15_IMPL
#undef            _STDEX_ARGS15_IMPL
#undef       _STDEX_TMPL_ARGS15
#undef           _STDEX_TYPES15
#undef          _STDEX_PARAMS15
#undef            _STDEX_ARGS15
#undef    _STDEX_REPEAT_TOKEN15
#undef       _STDEX_TMPL_ARGS16_IMPL
#undef           _STDEX_TYPES16_IMPL
#undef          _STDEX_PARAMS16_IMPL
#undef            _STDEX_ARGS16_IMPL
#undef       _STDEX_TMPL_ARGS16
#undef           _STDEX_TYPES16
#undef          _STDEX_PARAMS16
#undef            _STDEX_ARGS16
#undef    _STDEX_REPEAT_TOKEN16
#undef       _STDEX_TMPL_ARGS17_IMPL
#undef           _STDEX_TYPES17_IMPL
#undef          _STDEX_PARAMS17_IMPL
#undef            _STDEX_ARGS17_IMPL
#undef       _STDEX_TMPL_ARGS17
#undef           _STDEX_TYPES17
#undef          _STDEX_PARAMS17
#undef            _STDEX_ARGS17
#undef    _STDEX_REPEAT_TOKEN17
#undef       _STDEX_TMPL_ARGS18_IMPL
#undef           _STDEX_TYPES18_IMPL
#undef          _STDEX_PARAMS18_IMPL
#undef            _STDEX_ARGS18_IMPL
#undef       _STDEX_TMPL_ARGS18
#undef           _STDEX_TYPES18
#undef          _STDEX_PARAMS18
#undef            _STDEX_ARGS18
#undef    _STDEX_REPEAT_TOKEN18
#undef       _STDEX_TMPL_ARGS19_IMPL
#undef           _STDEX_TYPES19_IMPL
#undef          _STDEX_PARAMS19_IMPL
#undef            _STDEX_ARGS19_IMPL
#undef       _STDEX_TMPL_ARGS19
#undef           _STDEX_TYPES19
#undef          _STDEX_PARAMS19
#undef            _STDEX_ARGS19
#undef    _STDEX_REPEAT_TOKEN19
#undef       _STDEX_TMPL_ARGS20_IMPL
#undef           _STDEX_TYPES20_IMPL
#undef          _STDEX_PARAMS20_IMPL
#undef            _STDEX_ARGS20_IMPL
#undef       _STDEX_TMPL_ARGS20
#undef           _STDEX_TYPES20
#undef          _STDEX_PARAMS20
#undef            _STDEX_ARGS20
#undef    _STDEX_REPEAT_TOKEN20
#undef       _STDEX_TMPL_ARGS21_IMPL
#undef           _STDEX_TYPES21_IMPL
#undef          _STDEX_PARAMS21_IMPL
#undef            _STDEX_ARGS21_IMPL
#undef       _STDEX_TMPL_ARGS21
#undef           _STDEX_TYPES21
#undef          _STDEX_PARAMS21
#undef            _STDEX_ARGS21
#undef    _STDEX_REPEAT_TOKEN21
#undef       _STDEX_TMPL_ARGS22_IMPL
#undef           _STDEX_TYPES22_IMPL
#undef          _STDEX_PARAMS22_IMPL
#undef            _STDEX_ARGS22_IMPL
#undef       _STDEX_TMPL_ARGS22
#undef           _STDEX_TYPES22
#undef          _STDEX_PARAMS22
#undef            _STDEX_ARGS22
#undef    _STDEX_REPEAT_TOKEN22
#undef       _STDEX_TMPL_ARGS23_IMPL
#undef           _STDEX_TYPES23_IMPL
#undef          _STDEX_PARAMS23_IMPL
#undef            _STDEX_ARGS23_IMPL
#undef       _STDEX_TMPL_ARGS23
#undef           _STDEX_TYPES23
#undef          _STDEX_PARAMS23
#undef            _STDEX_ARGS23
#undef    _STDEX_REPEAT_TOKEN23
#undef       _STDEX_TMPL_ARGS24_IMPL
#undef           _STDEX_TYPES24_IMPL
#undef          _STDEX_PARAMS24_IMPL
#undef            _STDEX_ARGS24_IMPL
#undef       _STDEX_TMPL_ARGS24
#undef           _STDEX_TYPES24
#undef          _STDEX_PARAMS24
#undef            _STDEX_ARGS24
#undef    _STDEX_REPEAT_TOKEN24
#undef       _STDEX_TMPL_ARGS25_IMPL
#undef           _STDEX_TYPES25_IMPL
#undef          _STDEX_PARAMS25_IMPL
#undef            _STDEX_ARGS25_IMPL
#undef       _STDEX_TMPL_ARGS25
#undef           _STDEX_TYPES25
#undef          _STDEX_PARAMS25
#undef            _STDEX_ARGS25
#undef    _STDEX_REPEAT_TOKEN25
#undef       _STDEX_TMPL_ARGS26_IMPL
#undef           _STDEX_TYPES26_IMPL
#undef          _STDEX_PARAMS26_IMPL
#undef            _STDEX_ARGS26_IMPL
#undef       _STDEX_TMPL_ARGS26
#undef           _STDEX_TYPES26
#undef          _STDEX_PARAMS26
#undef            _STDEX_ARGS26
#undef    _STDEX_REPEAT_TOKEN26
#undef       _STDEX_TMPL_ARGS27_IMPL
#undef           _STDEX_TYPES27_IMPL
#undef          _STDEX_PARAMS27_IMPL
#undef            _STDEX_ARGS27_IMPL
#undef       _STDEX_TMPL_ARGS27
#undef           _STDEX_TYPES27
#undef          _STDEX_PARAMS27
#undef            _STDEX_ARGS27
#undef    _STDEX_REPEAT_TOKEN27
#undef       _STDEX_TMPL_ARGS28_IMPL
#undef           _STDEX_TYPES28_IMPL
#undef          _STDEX_PARAMS28_IMPL
#undef            _STDEX_ARGS28_IMPL
#undef       _STDEX_TMPL_ARGS28
#undef           _STDEX_TYPES28
#undef          _STDEX_PARAMS28
#undef            _STDEX_ARGS28
#undef    _STDEX_REPEAT_TOKEN28
#undef       _STDEX_TMPL_ARGS29_IMPL
#undef           _STDEX_TYPES29_IMPL
#undef          _STDEX_PARAMS29_IMPL
#undef            _STDEX_ARGS29_IMPL
#undef       _STDEX_TMPL_ARGS29
#undef           _STDEX_TYPES29
#undef          _STDEX_PARAMS29
#undef            _STDEX_ARGS29
#undef    _STDEX_REPEAT_TOKEN29
#undef       _STDEX_TMPL_ARGS30_IMPL
#undef           _STDEX_TYPES30_IMPL
#undef          _STDEX_PARAMS30_IMPL
#undef            _STDEX_ARGS30_IMPL
#undef       _STDEX_TMPL_ARGS30
#undef           _STDEX_TYPES30
#undef          _STDEX_PARAMS30
#undef            _STDEX_ARGS30
#undef    _STDEX_REPEAT_TOKEN30
#undef       _STDEX_TMPL_ARGS31_IMPL
#undef           _STDEX_TYPES31_IMPL
#undef          _STDEX_PARAMS31_IMPL
#undef            _STDEX_ARGS31_IMPL
#undef       _STDEX_TMPL_ARGS31
#undef           _STDEX_TYPES31
#undef          _STDEX_PARAMS31
#undef            _STDEX_ARGS31
#undef    _STDEX_REPEAT_TOKEN31



#endif // _STDEX_PARAMETER_PACK_UNDEF
