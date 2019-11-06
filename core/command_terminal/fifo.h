#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

template<class T, uint16_t max_size_>
class FIFO {
public:
    FIFO() {
        flush();
    }

    void append(T element) {
        if (size() != max_size_) {
            count++;
            writePos = (writePos + 1) % max_size_;
            data[writePos] = element;
        }
    }

    T get() {
        if (size() != 0) {
            count--;
            readPos = (readPos + 1) % max_size_;
            return data[readPos];
        }
        return T();
    }

    constexpr size_t max_size() const {
        return max_size_;
    }

    constexpr size_t size() const {
        return count;
    }

    size_t free_space() const {
        return max_size() - count;
    }

    constexpr void flush() {
        writePos = readPos = count = 0;
    }

private:
    volatile uint16_t writePos, readPos, count;
    T data[max_size_];
};
