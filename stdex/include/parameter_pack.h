// this header imitates C++11 template parameter pack
// usage: define _STDEX_PARAMETER_PACK_INCLUDE as path to your header file and include this header right after define
// 
// f.e.:
//
// #define _STDEX_PARAMETER_PACK_DEFINE
// #include "./parameter_pack.h"
// #undef _STDEX_PARAMETER_PACK_DEFINE
// 
// #define MY_FUNC(N) \
//     template<_STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK) /*class _Arg0T, ...class _ArgNT*/> 
//     void my_func( _STDEX_PARAMS_MAX(_STDEX_BLANK, _STDEX_BLANK) /*_Arg0T arg0, ..._ArgNT argN*/)
//     { }
//
// MY_FUNC(0) /*template<class _Arg0T> void my_func(_Arg0T arg0) { }*/
//
// #define _STDEX_PARAMETER_PACK_UNDEF
// #include "./parameter_pack.h"
// #undef _STDEX_PARAMETER_PACK_UNDEF

#ifdef _STDEX_PARAMETER_PACK_DEFINE

#define _STDEX_TMPL_ARGS_IMPL(prefix, N, postfix) class prefix _Arg##N##T postfix
#define _STDEX_TYPES_IMPL(prefix, N, postfix) prefix _Arg##N##T postfix
#define _STDEX_PARAMS_IMPL(prefix1, prefix2, N, postfix1, postfix2) prefix1 _STDEX_PARAMS_TYPE_PREFIX(N) _Arg##N##T _STDEX_PARAMS_TYPE_POSTFIX(N) postfix1 prefix2 _STDEX_PARAMS_ARG_PREFIX(N) arg##N _STDEX_PARAMS_ARG_POSTFIX(N) postfix2
#define _STDEX_ARGS_IMPL(prefix, N, postfix) prefix arg##N postfix
#define _STDEX_BLANK  
#define _STDEX_PARAMS_TYPE_PREFIX(N) 
#define _STDEX_PARAMS_TYPE_POSTFIX(N) 
#define _STDEX_PARAMS_ARG_PREFIX(N) 
#define _STDEX_PARAMS_ARG_POSTFIX(N)

#define _STDEX_TMPL_ARGS(prefix, N, postfix) _STDEX_TMPL_ARGS_IMPL(prefix, N, postfix)
#define _STDEX_TYPES(prefix, N, postfix) _STDEX_TYPES_IMPL(prefix, N, postfix)
#define _STDEX_PARAMS(prefix1, prefix2, N, postfix1, postfix2) _STDEX_PARAMS_IMPL(prefix1, prefix2, N, postfix1, postfix2)
#define _STDEX_ARGS(prefix, N, postfix) _STDEX_ARGS_IMPL(prefix, N, postfix)

#define _STDEX_TMPL_ARGS0(prefix, postfix) _STDEX_TMPL_ARGS(prefix, 0, postfix)
#define _STDEX_TYPES0(prefix, postfix) _STDEX_TYPES(prefix, 0, postfix)
#define _STDEX_PARAMS0(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS(prefix1, prefix2,0, postfix1, postfix2)
#define _STDEX_ARGS0(prefix, postfix) _STDEX_ARGS(prefix, 0, postfix)

#define _STDEX_TMPL_ARGS1(prefix, postfix) _STDEX_TMPL_ARGS0(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 1, postfix)
#define _STDEX_TYPES1(prefix, postfix) _STDEX_TYPES0(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 1, postfix)
#define _STDEX_PARAMS1(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS0(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,1, postfix1, postfix2)
#define _STDEX_ARGS1(prefix, postfix) _STDEX_ARGS0(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 1, postfix)

#define _STDEX_TMPL_ARGS2(prefix, postfix) _STDEX_TMPL_ARGS1(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 2, postfix)
#define _STDEX_TYPES2(prefix, postfix) _STDEX_TYPES1(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 2, postfix)
#define _STDEX_PARAMS2(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS1(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,2, postfix1, postfix2)
#define _STDEX_ARGS2(prefix, postfix) _STDEX_ARGS1(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 2, postfix)

#define _STDEX_TMPL_ARGS3(prefix, postfix) _STDEX_TMPL_ARGS2(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 3, postfix)
#define _STDEX_TYPES3(prefix, postfix) _STDEX_TYPES2(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 3, postfix)
#define _STDEX_PARAMS3(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS2(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,3, postfix1, postfix2)
#define _STDEX_ARGS3(prefix, postfix) _STDEX_ARGS2(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 3, postfix)

#define _STDEX_TMPL_ARGS4(prefix, postfix) _STDEX_TMPL_ARGS3(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 4, postfix)
#define _STDEX_TYPES4(prefix, postfix) _STDEX_TYPES3(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 4, postfix)
#define _STDEX_PARAMS4(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS3(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,4, postfix1, postfix2)
#define _STDEX_ARGS4(prefix, postfix) _STDEX_ARGS3(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 4, postfix)

#define _STDEX_TMPL_ARGS5(prefix, postfix) _STDEX_TMPL_ARGS4(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 5, postfix)
#define _STDEX_TYPES5(prefix, postfix) _STDEX_TYPES4(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 5, postfix)
#define _STDEX_PARAMS5(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS4(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,5, postfix1, postfix2)
#define _STDEX_ARGS5(prefix, postfix) _STDEX_ARGS4(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 5, postfix)

#define _STDEX_TMPL_ARGS6(prefix, postfix) _STDEX_TMPL_ARGS5(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 6, postfix)
#define _STDEX_TYPES6(prefix, postfix) _STDEX_TYPES5(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 6, postfix)
#define _STDEX_PARAMS6(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS5(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,6, postfix1, postfix2)
#define _STDEX_ARGS6(prefix, postfix) _STDEX_ARGS5(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 6, postfix)

#define _STDEX_TMPL_ARGS7(prefix, postfix) _STDEX_TMPL_ARGS6(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 7, postfix)
#define _STDEX_TYPES7(prefix, postfix) _STDEX_TYPES6(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 7, postfix)
#define _STDEX_PARAMS7(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS6(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,7, postfix1, postfix2)
#define _STDEX_ARGS7(prefix, postfix) _STDEX_ARGS6(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 7, postfix)

#define _STDEX_TMPL_ARGS8(prefix, postfix) _STDEX_TMPL_ARGS7(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 8, postfix)
#define _STDEX_TYPES8(prefix, postfix) _STDEX_TYPES7(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 8, postfix)
#define _STDEX_PARAMS8(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS7(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,8, postfix1, postfix2)
#define _STDEX_ARGS8(prefix, postfix) _STDEX_ARGS7(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 8, postfix)

#define _STDEX_TMPL_ARGS9(prefix, postfix) _STDEX_TMPL_ARGS8(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 9, postfix)
#define _STDEX_TYPES9(prefix, postfix) _STDEX_TYPES8(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 9, postfix)
#define _STDEX_PARAMS9(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS8(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,9, postfix1, postfix2)
#define _STDEX_ARGS9(prefix, postfix) _STDEX_ARGS8(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 9, postfix)

#define _STDEX_TMPL_ARGS10(prefix, postfix) _STDEX_TMPL_ARGS9(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 10, postfix)
#define _STDEX_TYPES10(prefix, postfix) _STDEX_TYPES9(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 10, postfix)
#define _STDEX_PARAMS10(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS9(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,10, postfix1, postfix2)
#define _STDEX_ARGS10(prefix, postfix) _STDEX_ARGS9(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 10, postfix)

#define _STDEX_TMPL_ARGS11(prefix, postfix) _STDEX_TMPL_ARGS10(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 11, postfix)
#define _STDEX_TYPES11(prefix, postfix) _STDEX_TYPES10(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 11, postfix)
#define _STDEX_PARAMS11(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS10(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,11, postfix1, postfix2)
#define _STDEX_ARGS11(prefix, postfix) _STDEX_ARGS10(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 11, postfix)

#define _STDEX_TMPL_ARGS12(prefix, postfix) _STDEX_TMPL_ARGS11(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 12, postfix)
#define _STDEX_TYPES12(prefix, postfix) _STDEX_TYPES11(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 12, postfix)
#define _STDEX_PARAMS12(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS11(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,12, postfix1, postfix2)
#define _STDEX_ARGS12(prefix, postfix) _STDEX_ARGS11(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 12, postfix)

#define _STDEX_TMPL_ARGS13(prefix, postfix) _STDEX_TMPL_ARGS12(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 13, postfix)
#define _STDEX_TYPES13(prefix, postfix) _STDEX_TYPES12(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 13, postfix)
#define _STDEX_PARAMS13(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS12(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,13, postfix1, postfix2)
#define _STDEX_ARGS13(prefix, postfix) _STDEX_ARGS12(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 13, postfix)

#define _STDEX_TMPL_ARGS14(prefix, postfix) _STDEX_TMPL_ARGS13(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 14, postfix)
#define _STDEX_TYPES14(prefix, postfix) _STDEX_TYPES13(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 14, postfix)
#define _STDEX_PARAMS14(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS13(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,14, postfix1, postfix2)
#define _STDEX_ARGS14(prefix, postfix) _STDEX_ARGS13(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 14, postfix)

#define _STDEX_TMPL_ARGS15(prefix, postfix) _STDEX_TMPL_ARGS14(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 15, postfix)
#define _STDEX_TYPES15(prefix, postfix) _STDEX_TYPES14(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 15, postfix)
#define _STDEX_PARAMS15(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS14(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,15, postfix1, postfix2)
#define _STDEX_ARGS15(prefix, postfix) _STDEX_ARGS14(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 15, postfix)

#define _STDEX_TMPL_ARGS16(prefix, postfix) _STDEX_TMPL_ARGS15(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 16, postfix)
#define _STDEX_TYPES16(prefix, postfix) _STDEX_TYPES15(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 16, postfix)
#define _STDEX_PARAMS16(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS15(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,16, postfix1, postfix2)
#define _STDEX_ARGS16(prefix, postfix) _STDEX_ARGS15(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 16, postfix)

#define _STDEX_TMPL_ARGS17(prefix, postfix) _STDEX_TMPL_ARGS16(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 17, postfix)
#define _STDEX_TYPES17(prefix, postfix) _STDEX_TYPES16(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 17, postfix)
#define _STDEX_PARAMS17(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS16(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,17, postfix1, postfix2)
#define _STDEX_ARGS17(prefix, postfix) _STDEX_ARGS16(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 17, postfix)

#define _STDEX_TMPL_ARGS18(prefix, postfix) _STDEX_TMPL_ARGS17(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 18, postfix)
#define _STDEX_TYPES18(prefix, postfix) _STDEX_TYPES17(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 18, postfix)
#define _STDEX_PARAMS18(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS17(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,18, postfix1, postfix2)
#define _STDEX_ARGS18(prefix, postfix) _STDEX_ARGS17(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 18, postfix)

#define _STDEX_TMPL_ARGS19(prefix, postfix) _STDEX_TMPL_ARGS18(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 19, postfix)
#define _STDEX_TYPES19(prefix, postfix) _STDEX_TYPES18(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 19, postfix)
#define _STDEX_PARAMS19(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS18(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,19, postfix1, postfix2)
#define _STDEX_ARGS19(prefix, postfix) _STDEX_ARGS18(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 19, postfix)

#define _STDEX_TMPL_ARGS20(prefix, postfix) _STDEX_TMPL_ARGS19(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 20, postfix)
#define _STDEX_TYPES20(prefix, postfix) _STDEX_TYPES19(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 20, postfix)
#define _STDEX_PARAMS20(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS19(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,20, postfix1, postfix2)
#define _STDEX_ARGS20(prefix, postfix) _STDEX_ARGS19(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 20, postfix)

#define _STDEX_TMPL_ARGS21(prefix, postfix) _STDEX_TMPL_ARGS20(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 21, postfix)
#define _STDEX_TYPES21(prefix, postfix) _STDEX_TYPES20(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 21, postfix)
#define _STDEX_PARAMS21(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS20(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,21, postfix1, postfix2)
#define _STDEX_ARGS21(prefix, postfix) _STDEX_ARGS20(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 21, postfix)

#define _STDEX_TMPL_ARGS22(prefix, postfix) _STDEX_TMPL_ARGS21(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 22, postfix)
#define _STDEX_TYPES22(prefix, postfix) _STDEX_TYPES21(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 22, postfix)
#define _STDEX_PARAMS22(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS21(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,22, postfix1, postfix2)
#define _STDEX_ARGS22(prefix, postfix) _STDEX_ARGS21(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 22, postfix)

#define _STDEX_TMPL_ARGS23(prefix, postfix) _STDEX_TMPL_ARGS22(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 23, postfix)
#define _STDEX_TYPES23(prefix, postfix) _STDEX_TYPES22(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 23, postfix)
#define _STDEX_PARAMS23(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS22(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,23, postfix1, postfix2)
#define _STDEX_ARGS23(prefix, postfix) _STDEX_ARGS22(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 23, postfix)

#define _STDEX_TMPL_ARGS24(prefix, postfix) _STDEX_TMPL_ARGS23(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 24, postfix)
#define _STDEX_TYPES24(prefix, postfix) _STDEX_TYPES23(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 24, postfix)
#define _STDEX_PARAMS24(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS23(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,24, postfix1, postfix2)
#define _STDEX_ARGS24(prefix, postfix) _STDEX_ARGS23(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 24, postfix)

#define _STDEX_TMPL_ARGS25(prefix, postfix) _STDEX_TMPL_ARGS24(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 25, postfix)
#define _STDEX_TYPES25(prefix, postfix) _STDEX_TYPES24(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 25, postfix)
#define _STDEX_PARAMS25(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS24(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,25, postfix1, postfix2)
#define _STDEX_ARGS25(prefix, postfix) _STDEX_ARGS24(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 25, postfix)

#define _STDEX_TMPL_ARGS26(prefix, postfix) _STDEX_TMPL_ARGS25(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 26, postfix)
#define _STDEX_TYPES26(prefix, postfix) _STDEX_TYPES25(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 26, postfix)
#define _STDEX_PARAMS26(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS25(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,26, postfix1, postfix2)
#define _STDEX_ARGS26(prefix, postfix) _STDEX_ARGS25(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 26, postfix)

#define _STDEX_TMPL_ARGS27(prefix, postfix) _STDEX_TMPL_ARGS26(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 27, postfix)
#define _STDEX_TYPES27(prefix, postfix) _STDEX_TYPES26(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 27, postfix)
#define _STDEX_PARAMS27(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS26(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,27, postfix1, postfix2)
#define _STDEX_ARGS27(prefix, postfix) _STDEX_ARGS26(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 27, postfix)

#define _STDEX_TMPL_ARGS28(prefix, postfix) _STDEX_TMPL_ARGS27(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 28, postfix)
#define _STDEX_TYPES28(prefix, postfix) _STDEX_TYPES27(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 28, postfix)
#define _STDEX_PARAMS28(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS27(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,28, postfix1, postfix2)
#define _STDEX_ARGS28(prefix, postfix) _STDEX_ARGS27(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 28, postfix)

#define _STDEX_TMPL_ARGS29(prefix, postfix) _STDEX_TMPL_ARGS28(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 29, postfix)
#define _STDEX_TYPES29(prefix, postfix) _STDEX_TYPES28(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 29, postfix)
#define _STDEX_PARAMS29(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS28(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,29, postfix1, postfix2)
#define _STDEX_ARGS29(prefix, postfix) _STDEX_ARGS28(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 29, postfix)

#define _STDEX_TMPL_ARGS30(prefix, postfix) _STDEX_TMPL_ARGS29(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 30, postfix)
#define _STDEX_TYPES30(prefix, postfix) _STDEX_TYPES29(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 30, postfix)
#define _STDEX_PARAMS30(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS29(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,30, postfix1, postfix2)
#define _STDEX_ARGS30(prefix, postfix) _STDEX_ARGS29(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 30, postfix)

#define _STDEX_TMPL_ARGS31(prefix, postfix) _STDEX_TMPL_ARGS30(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 31, postfix)
#define _STDEX_TYPES31(prefix, postfix) _STDEX_TYPES30(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 31, postfix)
#define _STDEX_PARAMS31(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS30(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,31, postfix1, postfix2)
#define _STDEX_ARGS31(prefix, postfix) _STDEX_ARGS30(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 31, postfix)

#ifndef STDEX_ARGS_MAX_COUNT
    #define STDEX_ARGS_MAX_COUNT 32
#endif

#define _STDEX_TMPL_ARGS_MAX_IMPL(N, prefix, postfix) \
    _STDEX_TMPL_ARGS##N##(prefix, postfix)
#define _STDEX_TYPES_MAX_IMPL(N, prefix, postfix) \
    _STDEX_TYPES##N##(prefix, postfix)
#define _STDEX_PARAMS_MAX_IMPL(N, prefix1, postfix1, prefix2, postfix2) \
    _STDEX_PARAMS##N##(prefix1, postfix1, prefix2, postfix2)
#define _STDEX_ARGS_MAX_IMPL(N, prefix, postfix) \
    _STDEX_ARGS##N##(prefix, postfix)

#define _STDEX_TMPL_ARGS_MAX(prefix, postfix) \
    _STDEX_TMPL_ARGS_MAX_IMPL(31, prefix, postfix)
#define _STDEX_TYPES_MAX(prefix, postfix) \
    _STDEX_TYPES_MAX_IMPL(31, prefix, postfix)
#define _STDEX_PARAMS_MAX(prefix1, postfix1, prefix2, postfix2) \
    _STDEX_PARAMS_MAX_IMPL(31, prefix1, postfix1, prefix2, postfix2)
#define _STDEX_ARGS_MAX(prefix, postfix) \
    _STDEX_ARGS_MAX_IMPL(31, prefix, postfix)

#endif // _STDEX_PARAMETER_PACK_DEFINE

#ifdef _STDEX_PARAMETER_PACK_UNDEF

#undef _STDEX_BLANK
#undef _STDEX_PARAMS_TYPE_PREFIX
#undef _STDEX_PARAMS_TYPE_POSTFIX
#undef _STDEX_PARAMS_ARG_PREFIX
#undef _STDEX_PARAMS_ARG_POSTFIX 

#undef _STDEX_TMPL_ARGS_IMPL
#undef _STDEX_TYPES_IMPL
#undef _STDEX_PARAMS_IMPL
#undef _STDEX_ARGS_IMPL

#undef _STDEX_TMPL_ARGS
#undef _STDEX_TYPES
#undef _STDEX_PARAMS
#undef _STDEX_ARGS

#undef _STDEX_TMPL_ARGS_MAX_IMPL
#undef _STDEX_TYPES_MAX_IMPL
#undef _STDEX_PARAMS_MAX_IMPL
#undef _STDEX_ARGS_MAX_IMPL

#undef _STDEX_TMPL_ARGS_MAX
#undef _STDEX_TYPES_MAX
#undef _STDEX_PARAMS_MAX
#undef _STDEX_ARGS_MAX

#undef _STDEX_INVOKE 
#undef _STDEX_INVOKE_IMPL

#undef       _STDEX_TMPL_ARGS0
#undef           _STDEX_TYPES0
#undef          _STDEX_PARAMS0
#undef            _STDEX_ARGS0
#undef       _STDEX_TMPL_ARGS1
#undef           _STDEX_TYPES1
#undef          _STDEX_PARAMS1
#undef            _STDEX_ARGS1
#undef       _STDEX_TMPL_ARGS2
#undef           _STDEX_TYPES2
#undef          _STDEX_PARAMS2
#undef            _STDEX_ARGS2
#undef       _STDEX_TMPL_ARGS3
#undef           _STDEX_TYPES3
#undef          _STDEX_PARAMS3
#undef            _STDEX_ARGS3
#undef       _STDEX_TMPL_ARGS4
#undef           _STDEX_TYPES4
#undef          _STDEX_PARAMS4
#undef            _STDEX_ARGS4
#undef       _STDEX_TMPL_ARGS5
#undef           _STDEX_TYPES5
#undef          _STDEX_PARAMS5
#undef            _STDEX_ARGS5
#undef       _STDEX_TMPL_ARGS6
#undef           _STDEX_TYPES6
#undef          _STDEX_PARAMS6
#undef            _STDEX_ARGS6
#undef       _STDEX_TMPL_ARGS7
#undef           _STDEX_TYPES7
#undef          _STDEX_PARAMS7
#undef            _STDEX_ARGS7
#undef       _STDEX_TMPL_ARGS8
#undef           _STDEX_TYPES8
#undef          _STDEX_PARAMS8
#undef            _STDEX_ARGS8
#undef       _STDEX_TMPL_ARGS9
#undef           _STDEX_TYPES9
#undef          _STDEX_PARAMS9
#undef            _STDEX_ARGS9
#undef       _STDEX_TMPL_ARGS10
#undef           _STDEX_TYPES10
#undef          _STDEX_PARAMS10
#undef            _STDEX_ARGS10
#undef       _STDEX_TMPL_ARGS11
#undef           _STDEX_TYPES11
#undef          _STDEX_PARAMS11
#undef            _STDEX_ARGS11
#undef       _STDEX_TMPL_ARGS12
#undef           _STDEX_TYPES12
#undef          _STDEX_PARAMS12
#undef            _STDEX_ARGS12
#undef       _STDEX_TMPL_ARGS13
#undef           _STDEX_TYPES13
#undef          _STDEX_PARAMS13
#undef            _STDEX_ARGS13
#undef       _STDEX_TMPL_ARGS14
#undef           _STDEX_TYPES14
#undef          _STDEX_PARAMS14
#undef            _STDEX_ARGS14
#undef       _STDEX_TMPL_ARGS15
#undef           _STDEX_TYPES15
#undef          _STDEX_PARAMS15
#undef            _STDEX_ARGS15
#undef       _STDEX_TMPL_ARGS16
#undef           _STDEX_TYPES16
#undef          _STDEX_PARAMS16
#undef            _STDEX_ARGS16
#undef       _STDEX_TMPL_ARGS17
#undef           _STDEX_TYPES17
#undef          _STDEX_PARAMS17
#undef            _STDEX_ARGS17
#undef       _STDEX_TMPL_ARGS18
#undef           _STDEX_TYPES18
#undef          _STDEX_PARAMS18
#undef            _STDEX_ARGS18
#undef       _STDEX_TMPL_ARGS19
#undef           _STDEX_TYPES19
#undef          _STDEX_PARAMS19
#undef            _STDEX_ARGS19
#undef       _STDEX_TMPL_ARGS20
#undef           _STDEX_TYPES20
#undef          _STDEX_PARAMS20
#undef            _STDEX_ARGS20
#undef       _STDEX_TMPL_ARGS21
#undef           _STDEX_TYPES21
#undef          _STDEX_PARAMS21
#undef            _STDEX_ARGS21
#undef       _STDEX_TMPL_ARGS22
#undef           _STDEX_TYPES22
#undef          _STDEX_PARAMS22
#undef            _STDEX_ARGS22
#undef       _STDEX_TMPL_ARGS23
#undef           _STDEX_TYPES23
#undef          _STDEX_PARAMS23
#undef            _STDEX_ARGS23
#undef       _STDEX_TMPL_ARGS24
#undef           _STDEX_TYPES24
#undef          _STDEX_PARAMS24
#undef            _STDEX_ARGS24
#undef       _STDEX_TMPL_ARGS25
#undef           _STDEX_TYPES25
#undef          _STDEX_PARAMS25
#undef            _STDEX_ARGS25
#undef       _STDEX_TMPL_ARGS26
#undef           _STDEX_TYPES26
#undef          _STDEX_PARAMS26
#undef            _STDEX_ARGS26
#undef       _STDEX_TMPL_ARGS27
#undef           _STDEX_TYPES27
#undef          _STDEX_PARAMS27
#undef            _STDEX_ARGS27
#undef       _STDEX_TMPL_ARGS28
#undef           _STDEX_TYPES28
#undef          _STDEX_PARAMS28
#undef            _STDEX_ARGS28
#undef       _STDEX_TMPL_ARGS29
#undef           _STDEX_TYPES29
#undef          _STDEX_PARAMS29
#undef            _STDEX_ARGS29
#undef       _STDEX_TMPL_ARGS30
#undef           _STDEX_TYPES30
#undef          _STDEX_PARAMS30
#undef            _STDEX_ARGS30
#undef       _STDEX_TMPL_ARGS31
#undef           _STDEX_TYPES31
#undef          _STDEX_PARAMS31
#undef            _STDEX_ARGS31

#endif // _STDEX_PARAMETER_PACK_UNDEF
