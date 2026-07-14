#include "RingBuffer.hpp"
#include "Logger.hpp"
#include "MemoryPool.hpp"

#include <iostream>

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

    // Memory pool test
    MemoryPool pool(32, 2);

    void* p1 = pool.allocate(); 
    void* p2 = pool.allocate();
    void* p3 = pool.allocate();         // out of blockCount : nullptr

    bool r1 = pool.deallocate(p1);      // true
    bool r2 = pool.deallocate(p1);      // double free : false

    int foreign{};
    bool r3 = pool.deallocate(&foreign);        // invalid ptr : false

    auto* interior = static_cast<std::uint8_t*>(p2) + 1;

    bool r4 = pool.deallocate(interior);        // misaligned block address : false

    bool r5 = pool.deallocate(p2);      // true

    std::cout << "Free blocks: " << pool.freeBlocks() << '\n';

    return 0;
}