#pragma once

#include "RingBuffer.hpp"

#include <cstddef>
#include <string>

class Logger {
    public:
    explicit Logger(std::size_t capacity);

    bool log(const std::string& message);
    void flush();

    private:
    RingBuffer buffer_;
};