#include "Logger.hpp"

#include <cstdint>
#include <iostream>

Logger::Logger(std::size_t capacity) : buffer_(capacity) {}

bool Logger::log(const std::string& message)
{
    for(char ch : message)
    {
        if(!buffer_.push(static_cast<std::uint8_t>(ch)))
        {
            return false;
        }
    }

    if (!buffer_.push(static_cast<std::uint8_t>('\n')))
    {
        return false;
    }

    return true;
}

void Logger::flush()
{
    std::uint8_t byte{};

    while(buffer_.pop(byte))
    {
        std::cout << static_cast<char>(byte);
    }
}