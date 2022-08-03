// Author: shr-go

#ifndef CHANCHAN_SQLCORE_H
#define CHANCHAN_SQLCORE_H

#include "Reflection.h"
#include "TypeMapping.h"
#include "SimpleBuf.h"
#include "Util.h"
#include <string>
#include <unordered_set>

namespace chanchan {
    struct createHelper {
        SimpleBuf &buf_;
        std::vector<std::string> prefix_;

        explicit createHelper(SimpleBuf &buf) : buf_(buf), prefix_() {}

        template<typename T1, typename T2>
        enable_if_t<!isReflection<reflectMemberType<T1, T2>>::value>
        operator()(const T1 &, const T2 &t2, std::size_t idx) {
            using Type = reflectMemberType<T1, T2>;
            for (const auto &p: prefix_) {
                buf_ << p << '_';
            }
            buf_ << T1::names[idx] << " " << getSQLType(Type{}) << ",\n";
        }

        template<typename T1, typename T2>
        enable_if_t<isReflection<reflectMemberType<T1, T2>>::value>
        operator()(const T1 &, const T2 &t2, std::size_t idx) {
            using Type = reflectMemberType<T1, T2>;
            prefix_.emplace_back(T1::names[idx]);
            typeMap<Type>(*this);
            prefix_.pop_back();
        }
    };

    ///@brief This method return a string which contains a create table sql by template argument type.
    ///@note Currently, we use the first element as key.
    template<typename T>
    std::string createTable() {
        using R = reflectMeber<T>;
        SimpleBuf buf;
        buf << "CREATE TABLE IF NOT EXISTS `" << R::name << "`\n(\n";
        typeMap<T>(createHelper{buf});
        buf << "PRIMARY KEY (" << R::names[0] << ")\n);";
        return buf.retrieveAllAsString();
    }

//    void escapeValue(SimpleBuf &buf) {
//        static std::regex re("[\0\b\t\n\r\x1a\"\'\\]");
//        size_t len = buf.getLastElementSize();
//        buf.ensureWritableBytes(len);
//        buf.unWrite(len);
//        char *begin = buf.beginWrite();
//        char *end = std::regex_replace(begin, begin, begin + len, re, R"(\$&)");
//        buf.hasWritten(end - begin);
//    }

    struct getKeysHelper {
        std::vector<std::string> &keys_;
        std::vector<std::string> prefix_;
        SimpleBuf buf_;

        explicit getKeysHelper(std::vector<std::string> &keys) : keys_(keys), prefix_(), buf_() {}

        template<typename T1, typename T2>
        enable_if_t<!isReflection<reflectMemberType<T1, T2>>::value>
        operator()(const T1 &, const T2 &t2, std::size_t idx) {
            for (const auto &p: prefix_) {
                buf_ << p << '_';
            }
            buf_ << T1::names[idx];
            keys_.push_back(buf_.retrieveAllAsString());
        }

        template<typename T1, typename T2>
        enable_if_t<isReflection<reflectMemberType<T1, T2>>::value>
        operator()(const T1 &, const T2 &t2, std::size_t idx) {
            using Type = reflectMemberType<T1, T2>;
            prefix_.emplace_back(T1::names[idx]);
            typeMap<Type>(*this);
            prefix_.pop_back();
        }
    };

    template<typename T>
    const std::vector<std::string> &getKeys() {
        static std::vector<std::string> result;
        if (result.empty()) {
            typeMap<T>(getKeysHelper{result});
        }
        return result;
    }

    template<typename T>
    std::string concatKey() {
        static std::string result;
        if (result.empty()) {
            SimpleBuf buf;
            const auto &keys = getKeys<T>();
            for (const auto &key: keys) {
                buf << '`' << key << "`, ";
            }
            buf.unWrite(2);
            result = buf.retrieveAllAsString();
        }
        return result;
    }

    template<typename V>
    enable_if_t<!needEscape<V>::value>
    writeValue(SimpleBuf& buf, const V& v) {
        buf << v;
    }

    template<typename V>
    enable_if_t<needEscape<V>::value>
    writeValue(SimpleBuf& buf, const V& v) {
        buf << '\'' << SqlValue(v) << '\'';
    }

    struct concatValueHelper {
        SimpleBuf &buf_;
        uint8_t depth_;

        explicit concatValueHelper(SimpleBuf &buf) : buf_(buf), depth_(0) {}

        template<typename T1, typename T2>
        enable_if_t<!isReflection<memberType<T1, T2>>::value>
        operator()(const T1 &t1, const T2 &t2, std::size_t idx) {
            using R = reflectMeber<T1>;
            writeValue(buf_, t1.*t2);
            if (idx < R::value - 1 || depth_ != 0) {
                buf_ << ", ";
            }
        }

        template<typename T1, typename T2>
        enable_if_t<isReflection<memberType<T1, T2>>::value>
        operator()(const T1 &t1, const T2 &t2, std::size_t idx) {
            using R = reflectMeber<T1>;
            depth_ += 1;
            map(t1.*t2, *this);
            depth_ -= 1;
            if (idx >= R::value - 1 && depth_ == 0) {
                buf_.unWrite(2);
            }
        }
    };

    ///@brief This method generate an insert sql for single record.
    template<typename T>
    std::string insertRecord(const T &t) {
        using R = reflectMeber<T>;
        SimpleBuf buf;
        buf << "INSERT INTO `" << R::name << "` (" << concatKey<T>() << ")\nVALUES (";
        map(t, concatValueHelper{buf});
        buf << ");";
        return buf.retrieveAllAsString();
    }

    ///@brief This method generate an insert sql for records.
    template<typename T>
    std::string insertRecord(const std::vector<T> &vt) {
        using R = reflectMeber<T>;
        SimpleBuf buf;
        buf << "INSERT INTO `" << R::name << "` (" << concatKey<T>() << ")\nVALUES\n";
        for (const auto &t: vt) {
            buf << '(';
            map(t, concatValueHelper{buf});
            buf << "),\n";
        }
        buf.unWrite(2);
        buf << ';';
        return buf.retrieveAllAsString();
    }

    struct updateHelper {
        SimpleBuf &buf_;
        SimpleBuf &bufWhere_;
        const std::unordered_set<std::string> &wheres_;
        const std::vector<std::string> &keys_;
        uint16_t n_;

        updateHelper(SimpleBuf &buf,
                     SimpleBuf &bufWhere,
                     const std::unordered_set<std::string> &wheres,
                     const std::vector<std::string> &keys)
                : buf_(buf), bufWhere_(bufWhere), wheres_(wheres), n_(0), keys_(keys) {}

        template<typename T1, typename T2>
        enable_if_t<!isReflection<memberType<T1, T2>>::value>
        operator()(const T1 &t1, const T2 &t2, std::size_t idx) {
            std::size_t realN = idx + n_;
            const auto &key = keys_[realN];
            if (wheres_.find(key) != wheres_.end()) {
                bufWhere_ << '`' << key << "` = ";
                writeValue(bufWhere_, t1.*t2);
                bufWhere_ << " AND ";
            } else {
                buf_ << '`' << key << "` = ";
                writeValue(buf_, t1.*t2);
                buf_ << ", ";
            }
            if (realN == keys_.size() - 1) {
                buf_.unWrite(2);
                bufWhere_.unWrite(5);
                buf_ << "\nWHERE " << bufWhere_ << ';';
            }
        }

        template<typename T1, typename T2>
        enable_if_t<isReflection<memberType<T1, T2>>::value>
        operator()(const T1 &t1, const T2 &t2, std::size_t idx) {
            n_ = idx;
            map(t1.*t2, *this);
        }
    };

    ///@brief This method generate an updateRecord sql for single record.
    ///@note You can pass a string as update key, if no string offered, we use the first element as the updateRecord key.
    ///@note You can not use compound type as updateRecord key.
    ///@example
    /// struct person{std::string name, int age};\n
    /// person p{"One", 2};\n
    /// updateRecord(p);\n
    /// updateRecord(p, "age");
    template<typename T, typename... Args>
    std::string updateRecord(const T &t, Args &&... args) {
        using R = reflectMeber<T>;
        SimpleBuf buf, bufWhere;
        std::unordered_set<std::string> wheres{args...};
        if (wheres.empty()) {
            wheres.insert(R::names[0]);
        }
        buf << "UPDATE `" << R::name << "`\nSET ";
        map(t, updateHelper{buf, bufWhere, wheres, getKeys<T>()});
        return buf.retrieveAllAsString();
    }

    //TODO Add multiple rows update method

    ///@brief This method generate a delete sql using user provided condition
    ///@example
    /// deleteRecord<person>(); //delete all records from table person.\n
    /// deleteRecord<person>("`name`=`Jack` and `age`=`10`") //delete records which name equals "Jack" and age is 10.
    template<typename T>
    std::string deleteRecord(const std::string &condition = "") {
        using R = reflectMeber<T>;
        SimpleBuf buf;
        buf << "DELETE FROM `" << R::name << '`';
        if (!condition.empty()) {
            buf << " WHERE " << condition;
        }
        buf << ';';
        return buf.retrieveAllAsString();
    }

    ///@brief This method generate a query sql using user provided condition
    ///@example
    /// pass
    template<typename T>
    std::string queryRecord(const std::string &condition = "") {
        using R = reflectMeber<T>;
        SimpleBuf buf;
        buf << "SELECT " << concatKey<T>() << " FROM `" << R::name << '`';
        if (!condition.empty()) {
            buf << ' ' << condition;
        }
        buf << ';';
        return buf.retrieveAllAsString();
    }
}

#endif //CHANCHAN_SQLCORE_H
