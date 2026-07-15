#include "RingBuffer.hpp"
#include "Logger.hpp"
#include "MemoryPool.hpp"
#include "MessageQueue.hpp"

#include <iostream>
#include <cstring>
#include <array>

void test_empty_pop()
{
    RingBuffer rb(4);
    std::uint8_t value{};
    std::cout << "empty pop: " << rb.pop(value) << '\n';
}

void test_full_push()
{
    RingBuffer rb(2);
   std::cout << rb.push(1) << '\n';
   std::cout << rb.push(2) << '\n';
   std::cout << rb.push(3) << '\n';
}

void test_wraparound()
{
    RingBuffer rb(3);

    rb.push(10);
    rb.push(20);

    std::uint8_t value{};
    rb.pop(value);

    rb.push(30);
    rb.push(40);

    while(rb.pop(value)) {
        std::cout << static_cast<int>(value) << '\n';
    }
}

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
    // Logger logger(128);

    // logger.log("ECU simulator started");
    // logger.log("Speed sensor task initiated");
    // logger.log("Diag task initiated");

    // logger.flush();

    /* Unit tests for RingBuffer*/

    // test_empty_pop();
    // test_full_push();
    // test_wraparound();

    /* Memory pool module test */ 
    // MemoryPool pool(32, 2);

    // void* p1 = pool.allocate(); 
    // void* p2 = pool.allocate();
    // void* p3 = pool.allocate();         // out of blockCount : nullptr

    // bool r1 = pool.deallocate(p1);      // true
    // bool r2 = pool.deallocate(p1);      // double free : false

    // int foreign{};
    // bool r3 = pool.deallocate(&foreign);        // invalid ptr : false

    // auto* interior = static_cast<std::uint8_t*>(p2) + 1;

    // bool r4 = pool.deallocate(interior);        // misaligned block address : false

    // bool r5 = pool.deallocate(p2);      // true

    // std::cout << "Free blocks: " << pool.freeBlocks() << '\n';

    /* MessageQueue module test */
    std::cout << std::boolalpha;

    MessageQueue queue(3, 10);

    const auto* str1 = "ABC";
    const auto* str2 = "1234";
    // const auto* str3 = "Overloaded message";
    // const auto* str4 = "PQR";
    
    // /* queue send() unit test_s*/
    // // std::cout << queue.send(nullptr, 3) << '\n';                    // null data : false
    // // std::cout << queue.send(str1, 0) << '\n';                       // zero length : false
    // // std::cout << queue.send(str3, std::strlen(str3)) << '\n';       // oversized message : false
    std::cout << queue.send(str1, std::strlen(str1)) << '\n';       // first messagge send
    std::cout << queue.send(str2, std::strlen(str2)) << '\n';       // second messagge send
    // // std::cout << queue.send(str4, std::strlen(str4)) << '\n';       // extra send after queue full : false
    // /* queue send() unit test_e*/

    // /* queue receive() unit test_s*/
    std::size_t rxMsgLength = 999;
    std::array<char, 16> destination{};

    // // //1. Empty receive : fail
    // bool result = queue.receive(destination.data(), destination.size(), rxMsgLength);

    // // std::cout << "Empty receive: " << result << '\n';
    // // std::cout << "Queue size: " << queue.size() << '\n';
    // // std::cout << "RECEIVED length: " << rxMsgLength << '\n';

    // // 2. send 2 messages
    // std::cout << queue.send(str1, std::strlen(str1)) << '\n';       // true
    // std::cout << queue.send(str2, std::strlen(str2)) << '\n';       // true

    // std::cout << "queue size after sends.." << queue.size() << '\n';

    // // 3. Destination too small
    // std::array<char, 2> smallDestination{};
    // rxMsgLength = 777;

    // // result = queue.receive(smallDestination.data(), smallDestination.size(), rxMsgLength);

    // // std::cout << "small destination receive: " << result << '\n';
    // // std::cout << "Queue size after failed receive: " << queue.size() << '\n';
    // // std::cout << "Received length: " << rxMsgLength << '\n';

    // // 4. Successfull receive for first message
    destination.fill('\0');
    rxMsgLength = 0;

    bool result = queue.receive(destination.data(), destination.size(), rxMsgLength);

    std::cout << "First receive: " << result << '\n';
    std::cout << "Message: " << std::string(destination.data(), rxMsgLength) << '\n';
    std::cout << "Received length: " << rxMsgLength << '\n';
    std::cout << "Queue size: " << queue.size() << '\n';

    // 4. Successfull receive for second message
    destination.fill('\0');
    rxMsgLength = 0;

    result = queue.receive(destination.data(), destination.size(), rxMsgLength);

    std::cout << "Second receive: " << result << '\n';
    std::cout << "Message: " << std::string(destination.data(), rxMsgLength) << '\n';
    std::cout << "Received length: " << rxMsgLength << '\n';
    std::cout << "Queue size: " << queue.size() << '\n';

    // 5. Try receive with empty queue
    destination.fill('\0');
    rxMsgLength = 0;

    result = queue.receive(destination.data(), destination.size(), rxMsgLength);

    std::cout << "Third receive: " << result << '\n';
    std::cout << "Message: " << std::string(destination.data(), rxMsgLength) << '\n';
    std::cout << "Received length: " << rxMsgLength << '\n';
    std::cout << "Queue size: " << queue.size() << '\n';

    /* 5. Wraparound test */
    // MessageQueue queue(2, 10);      // Message queue with size = 2

    // std::size_t rxMsgLength = 999;
    // std::array<char, 16> destination{};
    // destination.fill('\0');
    // rxMsgLength = 0;

    // queue.send("A", 1);         // First message send
    // queue.send("B", 1);         // second message send

    // bool result = queue.receive(destination.data(), destination.size(), rxMsgLength);   // First message receive

    // std::cout << "Message: " << std::string(destination.data(), rxMsgLength) << '\n';

    // queue.send("C", 1);     // Third message send (tail wraps around)

    // result = queue.receive(destination.data(), destination.size(), rxMsgLength);        // Second message receive

    // std::cout << "Message: " << std::string(destination.data(), rxMsgLength) << '\n';

    // result = queue.receive(destination.data(), destination.size(), rxMsgLength);        // Third message receive (First message was replaced due to wraparound)

    // std::cout << "Message: " << std::string(destination.data(), rxMsgLength) << '\n';


    /* queue receive() unit test_e*/

    return 0;
}