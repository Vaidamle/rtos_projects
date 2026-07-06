#include "RingBuffer.hpp"

#include <iostream>

int main()
{
    RingBuffer rb(4);

    rb.push(10);
    rb.push(20);
    rb.push(30);

    std::uint8_t value{};

    while(rb.pop(value)) {
        std::cout << static_cast<int>(value) << '\n';
    }

    return 0;
}