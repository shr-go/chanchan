// Author: shr-go

#ifndef CHANCHAN_REFLECTION_H
#define CHANCHAN_REFLECTION_H

#include "Util.h"
#include <array>
#include <tuple>
#include <functional>

// Copy from https://stackoverflow.com/a/11742317
#define PP_ARG_N(\
          _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
         _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
         _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
         _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
         _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
         _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
         _61, _62, _63, N, ...) N

#define PP_RSEQ_N()                                        \
         63, 62, 61, 60,                                   \
         59, 58, 57, 56, 55, 54, 53, 52, 51, 50,           \
         49, 48, 47, 46, 45, 44, 43, 42, 41, 40,           \
         39, 38, 37, 36, 35, 34, 33, 32, 31, 30,           \
         29, 28, 27, 26, 25, 24, 23, 22, 21, 20,           \
         19, 18, 17, 16, 15, 14, 13, 12, 11, 10,           \
          9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define PP_NARG_(...)    PP_ARG_N(__VA_ARGS__)

#define PP_COMMASEQ_N()                                    \
          1,  1,  1,  1,                                   \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  0,  0

#define PP_COMMA(...)    ,

#define PP_HASCOMMA(...)                                   \
          PP_NARG_(__VA_ARGS__, PP_COMMASEQ_N())

#define PP_NARG(...)                                       \
          PP_NARG_HELPER1(                                 \
              PP_HASCOMMA(__VA_ARGS__),                    \
              PP_HASCOMMA(PP_COMMA __VA_ARGS__ ()),        \
              PP_NARG_(__VA_ARGS__, PP_RSEQ_N()))

#define PP_NARG_HELPER1(a, b, N)    PP_NARG_HELPER2(a, b, N)
#define PP_NARG_HELPER2(a, b, N)    PP_NARG_HELPER3_ ## a ## b(N)
#define PP_NARG_HELPER3_01(N)    0
#define PP_NARG_HELPER3_00(N)    1
#define PP_NARG_HELPER3_11(N)    N

#define SEPERATOR ,

#define PROCESS_ARGS_1(op, ra, arg, ...) op(ra, arg)
#define PROCESS_ARGS_2(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_1(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_3(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_2(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_4(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_3(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_5(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_4(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_6(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_5(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_7(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_6(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_8(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_7(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_9(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_8(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_10(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_9(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_11(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_10(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_12(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_11(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_13(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_12(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_14(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_13(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_15(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_14(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_16(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_15(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_17(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_16(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_18(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_17(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_19(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_18(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_20(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_19(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_21(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_20(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_22(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_21(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_23(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_22(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_24(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_23(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_25(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_24(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_26(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_25(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_27(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_26(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_28(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_27(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_29(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_28(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_30(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_29(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_31(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_30(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_32(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_31(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_33(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_32(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_34(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_33(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_35(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_34(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_36(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_35(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_37(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_36(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_38(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_37(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_39(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_38(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_40(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_39(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_41(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_40(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_42(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_41(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_43(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_42(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_44(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_43(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_45(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_44(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_46(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_45(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_47(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_46(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_48(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_47(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_49(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_48(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_50(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_49(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_51(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_50(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_52(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_51(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_53(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_52(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_54(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_53(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_55(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_54(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_56(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_55(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_57(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_56(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_58(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_57(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_59(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_58(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_60(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_59(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_61(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_60(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_62(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_61(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_63(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_62(op,  ra, __VA_ARGS__)
#define PROCESS_ARGS_64(op, ra, arg, ...) op(ra, arg) SEPERATOR PROCESS_ARGS_63(op,  ra, __VA_ARGS__)


#define MACRO_CONCAT(A, B) A##_##B

#define OPTOSTR(ra, arg) #arg
#define OPTOFIELD(ra, arg) &ra::arg

namespace chanchan {
#define REFLECT_NAME(STRUCT_NAME) STRUCT_NAME##Reflect
#define REFLECT(STRUCT_NAME, ...) DOREFLECT(STRUCT_NAME, PP_NARG(__VA_ARGS__), __VA_ARGS__)
#define DOREFLECT(STRUCT_NAME, N, ...) \
    struct REFLECT_NAME(STRUCT_NAME) { \
    typedef STRUCT_NAME type;\
    constexpr static std::array<const char*, N> names = \
    {MACRO_CONCAT(PROCESS_ARGS, N)(OPTOSTR, NULL, __VA_ARGS__)};                   \
    constexpr static auto member =     \
    std::make_tuple(MACRO_CONCAT(PROCESS_ARGS, N)(OPTOFIELD, STRUCT_NAME, __VA_ARGS__)); \
    constexpr static size_t value = N; \
    constexpr static const char* name = #STRUCT_NAME;}; \
    constexpr decltype(REFLECT_NAME(STRUCT_NAME)::names) REFLECT_NAME(STRUCT_NAME)::names; \
    constexpr decltype(REFLECT_NAME(STRUCT_NAME)::member) REFLECT_NAME(STRUCT_NAME)::member; \
    constexpr decltype(REFLECT_NAME(STRUCT_NAME)::value) REFLECT_NAME(STRUCT_NAME)::value; \
    constexpr decltype(REFLECT_NAME(STRUCT_NAME)::name) REFLECT_NAME(STRUCT_NAME)::name; \
    static REFLECT_NAME(STRUCT_NAME) chanchanGetReflect(STRUCT_NAME const&) { \
        return REFLECT_NAME(STRUCT_NAME){}; \
    }

    template<typename T>
    using reflectMeber = decltype(chanchanGetReflect(std::declval<T>()));

    template<typename T, typename = void>
    struct isReflection : std::false_type {
    };

    template<typename T>
    struct isReflection<T, void_t<reflectMeber<T>>> : std::true_type {
    };

    template<typename T1, typename T2>
    using memberType = decltype(std::declval<T1>().*(std::declval<T2>()));

    template<typename T1, typename T2>
    using reflectMemberType = decltype(std::declval<typename T1::type>().*(std::declval<T2>()));


    template<typename T, std::size_t I>
    constexpr inline const char *getName() {
        using R = reflectMeber<T>;
        static_assert(I < R::value, "Index out of range");
        return R::names[I];
    }

    template<typename T, typename F, std::size_t Idx>
    enable_if_t<Idx >= reflectMeber<T>::value> domap(T &&t, F &&f) {}

    template<typename T, typename F, std::size_t Idx>
    enable_if_t<Idx < reflectMeber<T>::value> domap(T &&t, F &&f) {
        using R = reflectMeber<T>;
        std::forward<F>(f)(std::forward<T>(t), std::get<Idx>(R::member), Idx);
        domap<T, F, Idx + 1>(std::forward<T>(t), std::forward<F>(f));
    }

    template<typename T, typename F>
    void map(T &&t, F &&f) {
        static_assert(isReflection<T>::value, "T should be marked reflected");
        domap<T, F, 0>(std::forward<T>(t), std::forward<F>(f));
    }

    template<typename T, typename F, std::size_t Idx>
    enable_if_t<Idx >= reflectMeber<T>::value> doTypeMap(F &&) {}

    template<typename T, typename F, std::size_t Idx>
    enable_if_t<Idx < reflectMeber<T>::value> doTypeMap(F &&f) {
        using R = reflectMeber<T>;
        (std::forward<F>(f))(reflectMeber<T>{}, std::get<Idx>(R::member), Idx);
        doTypeMap < T, F, Idx + 1 > (std::forward<F>(f));
    }

    template<typename T, typename F>
    void typeMap(F &&f) {
        static_assert(isReflection<T>::value, "T should be marked reflected");
        doTypeMap<T, F, 0>(std::forward<F>(f));
    }
}

#endif //CHANCHAN_REFLECTION_H
