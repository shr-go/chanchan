// Author: shr-go

#ifndef CHANCHAN_TYPEMAPPING_H
#define CHANCHAN_TYPEMAPPING_H

#include <string>

//FIXME Add more mapping type.

namespace chanchan {
    constexpr inline const char* getSQLType(const std::string&) {
        return "TEXT";
    }

    constexpr inline const char* getSQLType(int) {
        return "INT";
    }

    constexpr inline const char* getSQLType(long long) {
        return "BIGINT";
    }

    constexpr inline const char* getSQLType(double) {
        return "DOUBLE";
    }
}

#endif //CHANCHAN_TYPEMAPPING_H
