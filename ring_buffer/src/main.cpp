#include "RingBuffer.hpp"
#include "Logger.hpp"

#include <iostream>

int main()
{
    /* RingBuffer task check */
    // RingBuffer rb(4);

    // rb.push(10);
    // rb.push(20);
    // rb.push(30);

    // //std::cout << "Free space after push: " << static_cast<int>(rb.freespace()) << '\n';

    // std::uint8_t value{};

    // while(rb.pop(value)) {
    //     std::cout << static_cast<int>(value) << '\n';
    // }

    /* Logger task check  */
    Logger logger(128);

    logger.log("ECU simulator started");
    logger.log("Speed sensor task initiated");
    logger.log("Diag task initiated");

    logger.flush();

    return 0;
}