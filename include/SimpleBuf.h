// Author: shr-go

#ifndef CHANCHAN_SIMPLEBUF_H
#define CHANCHAN_SIMPLEBUF_H

#include <vector>
#include <cassert>
#include <algorithm>
#include <cstring>

namespace chanchan {
    constexpr char digits[] = "9876543210123456789";
    constexpr const char *const zero = digits + 9;
    constexpr char digitsHex[] = "0123456789ABCDEF";

    template<typename T>
    size_t convert(char buf[], T value) {
        T i = value;
        char *p = buf;

        do {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while (i != 0);
        if (value < 0) {
            *p++ = '-';
        }
        *p = '\0';
        std::reverse(buf, p);
        return p - buf;
    }

    size_t convertHex(char buf[], uintptr_t value) {
        uintptr_t i = value;
        char *p = buf;

        do {
            int lsd = static_cast<int>(i % 16);
            i /= 16;
            *p++ = digitsHex[lsd];
        } while (i != 0);
        *p = '\0';
        std::reverse(buf, p);
        return p - buf;
    }

    class SqlValue;

    class SimpleBuf {
    public:
        typedef SimpleBuf self;

        explicit SimpleBuf(size_t initialSize = kInitialSize)
                : buffer_(kCheapPrepend + initialSize),
                  readerIndex_(kCheapPrepend),
                  writerIndex_(kCheapPrepend) {
            assert(readableBytes() == 0);
            assert(writableBytes() == initialSize);
            assert(prependableBytes() == kCheapPrepend);
        }

        size_t readableBytes() const { return writerIndex_ - readerIndex_; }

        size_t writableBytes() const { return buffer_.size() - writerIndex_; }

        size_t prependableBytes() const { return readerIndex_; }

        const char *peek() const { return begin() + readerIndex_; }

        void swap(SimpleBuf &rhs) noexcept {
            buffer_.swap(rhs.buffer_);
            std::swap(readerIndex_, rhs.readerIndex_);
            std::swap(writerIndex_, rhs.writerIndex_);
        }

        void retrieveAll() {
            readerIndex_ = kCheapPrepend;
            writerIndex_ = kCheapPrepend;
        }

        void retrieve(size_t len) {
            assert(len <= readableBytes());
            if (len < readableBytes()) {
                readerIndex_ += len;
            } else {
                retrieveAll();
            }
        }

        std::string retrieveAllAsString() { return retrieveAsString(readableBytes()); }

        std::string retrieveAsString(size_t len) {
            assert(len <= readableBytes());
            std::string result(peek(), len);
            retrieve(len);
            return result;
        }

        void append(const char *data, size_t len) {
            ensureWritableBytes(len);
            std::copy(data, data + len, beginWrite());
            hasWritten(len);
        }

        void ensureWritableBytes(size_t len) {
            if (writableBytes() < len) {
                makeSpace(len);
            }
            assert(writableBytes() >= len);
        }

        char *beginWrite() { return begin() + writerIndex_; }

        const char *beginWrite() const { return begin() + writerIndex_; }

        void hasWritten(size_t len) {
            assert(len <= writableBytes());
            writerIndex_ += len;
            lastElementSize_ = len;
        }

        void unWrite(size_t len) {
            assert(len <= readableBytes());
            writerIndex_ -= len;
        }

        size_t getLastElementSize() const {
            return lastElementSize_;
        }

        self &operator<<(bool v) {
            if (v)
                append("true", 4);
            else
                append("false", 5);
            return *this;
        }

        self &operator<<(short v) {
            *this << static_cast<int>(v);
            return *this;
        }

        self &operator<<(unsigned short v) {
            *this << static_cast<unsigned int>(v);
            return *this;
        }

        self &operator<<(int v) {
            formatInteger(v);
            return *this;
        }

        self &operator<<(unsigned int v) {
            formatInteger(v);
            return *this;
        }

        self &operator<<(long v) {
            formatInteger(v);
            return *this;
        }

        self &operator<<(unsigned long v) {
            formatInteger(v);
            return *this;
        }

        self &operator<<(long long v) {
            formatInteger(v);
            return *this;
        }

        self &operator<<(unsigned long long v) {
            formatInteger(v);
            return *this;
        }

        self &operator<<(float v) {
            *this << static_cast<double>(v);
            return *this;
        }

        self &operator<<(double v) {
            ensureWritableBytes(kMaxNumericSize);
            size_t len = snprintf(beginWrite(), kMaxNumericSize, "%.12g", v);
            hasWritten(len);
            return *this;
        }

        self &operator<<(char v) {
            append(&v, 1);
            return *this;
        }

        self &operator<<(const char *str) {
            if (str) {
                append(str, strlen(str));
            } else {
                append("(NULL)", 6);
            }
            return *this;
        }

        self &operator<<(const std::string &v) {
            append(v.c_str(), v.size());
            return *this;
        }

        self &operator<<(const SimpleBuf &rhs) {
            append(rhs.peek(), rhs.readableBytes());
            return *this;
        }

        inline self &operator<<(const SqlValue &rhs);

    private:
        char *begin() { return buffer_.data(); }

        const char *begin() const { return buffer_.data(); }

        void makeSpace(size_t len) {
            if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
                buffer_.resize(writerIndex_ + len);
            }
            /// if possible, move readable data to the front.
            if (kCheapPrepend < readerIndex_) {
                size_t readable = readableBytes();
                std::copy(begin() + readerIndex_,
                          begin() + writerIndex_,
                          begin() + kCheapPrepend);
                readerIndex_ = kCheapPrepend;
                writerIndex_ = readerIndex_ + readable;
                assert(readable == readableBytes());
            }
        }

        template<typename T>
        void formatInteger(T v) {
            ensureWritableBytes(kMaxNumericSize);
            size_t len = convert(beginWrite(), v);
            hasWritten(len);
        }

        static constexpr size_t kCheapPrepend = 8;
        static constexpr size_t kInitialSize = 1000;
        static constexpr size_t kMaxNumericSize = 48;

        std::vector<char> buffer_;
        size_t readerIndex_;
        size_t writerIndex_;
        size_t lastElementSize_; //FIXME remove this member
    };

    class SqlValue {
    public:
        SqlValue(const char *str) : ptr_(str), length_(strlen(ptr_)) {}

        SqlValue(const std::string &str) : ptr_(str.c_str()), length_(str.length()) {}

        void toEscape(SimpleBuf& buf) const {
            //FIXME use macro to decide which escape method will be used.
//            static std::string needEscapeStr("\0\b\t\n\r\x1a\"\'\\", 9);
//            for (size_t i = 0; i < length_; ++i) {
//                if (needEscapeStr.find(ptr_[i]) != std::string::npos) {
//                    buf << '\\';
//                }
//                buf << ptr_[i];
//            }
            buf.append(ptr_, length_);
        }

    private:
        const char *ptr_;
        size_t length_;
    };

    inline SimpleBuf& SimpleBuf::operator<<(const SqlValue &rhs) {
        rhs.toEscape(*this);
        return *this;
    }
};

#endif //CHANCHAN_SIMPLEBUF_H
