// Author: shr-go

#ifndef CHANCHAN_UTIL_H
#define CHANCHAN_UTIL_H

#include <type_traits>

namespace chanchan {
    template<typename...>
    using void_t = void;

    template<bool B, class T = void>
    using enable_if_t = typename std::enable_if<B, T>::type;

    class SqlValue;

    template<typename T, typename = void>
    struct needEscape : std::false_type {
    };

    template<typename T>
    struct needEscape<T, void_t<decltype(SqlValue(std::declval<T>()))>> : std::true_type {
    };
}

#endif //CHANCHAN_UTIL_H
