/* Logger.cpp
APIs definition for Logger module 
    - Log and flush messages from Ring Buffer
*/
#include "Logger.hpp"

#include <cstdint>
#include <iostream>

Logger::Logger(std::size_t capacity) : buffer_(capacity) {}

/* 
First logic : push message byte by byte 
Latest logic : Atomic bulk push support for logger    
*/
bool Logger::log(const std::string& message)
{
    /* First-logic_s*/
    // for(char ch : message)
    // {
    //     if(!buffer_.push(static_cast<std::uint8_t>(ch)))
    //     {
    //         return false;
    //     }
    // }

    // if (!buffer_.push(static_cast<std::uint8_t>('\n')))
    // {
    //     return false;
    // }
    
    // return true;
    /* First-logic_e*/

    std::string formattedMessage = message + '\n';

    return buffer_.push(reinterpret_cast<const std::uint8_t*>(formattedMessage.data()), formattedMessage.size());
}

void Logger::flush()
{
    std::uint8_t byte{};

    while(buffer_.pop(byte))
    {
        std::cout << static_cast<char>(byte);
    }
}