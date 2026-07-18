#include "RingBuffer.hpp"
#include "Logger.hpp"
#include "MemoryPool.hpp"
#include "MessageQueue.hpp"

#include <iostream>
#include <cstring>
#include <array>
#include <sstream>
#include <type_traits>

struct SensorMessage {
    std::uint32_t sensorID;
    float value; 
};

static_assert(
    std::is_trivially_copyable_v<SensorMessage>, "SensorMessage must be trivially copyable");

int main()
{
    /* Producer-consumer flow  using message pointer (single-threaded) 
        Producer
        -> MemoryPool::allocate() [Producer owns responsibility for returning it]
        -> Construct SensorMessage in pool Block
        -> MessageQueue stores only the pointer
        -> queue send() succeeds => responsibility transfers to the consumer
           -> queue.send() fails => Producer must return block to the pool.
        -> consumer receives the pointer
        -> Processes SensorMessage 
        -> MemoryPool::deallocate() => consumer returns block to the pool.

    */
    
    // Producer: create objects : Sensor message, Message queue, Logger

    MessageQueue queue(2, sizeof(SensorMessage*));
    MemoryPool pool(sizeof(SensorMessage), 3);
    Logger logger(256);

    std::cout << "pool.freeBlocks() initial == " << pool.freeBlocks() << '\n';          // unit_test

    // first message
    void* block = pool.allocate();

    if(block == nullptr){
        throw std::runtime_error("Memory pool not available");
    }

    std::cout << "pool.freeBlocks() after allocation == " << pool.freeBlocks() << '\n';          // unit_test

    //Construct SensorMessage in pool Block
    auto* message = static_cast<SensorMessage*>(block);

    message->sensorID = 1;
    message->value = 1.25F;

    // Producer: send message pointer to MessageQueue, if failed, return block to the pool
    if(!queue.send(&message, sizeof(message))) {
        pool.deallocate(message);
        std::cout << "pool.freeBlocks() after queue.send() failed == " << pool.freeBlocks() << '\n';    //unit_test         
        throw std::runtime_error("Send failed, queue not updated");
    }

    std::cout << "queue.size() after successful send == " << queue.size() << '\n';          // unit_test 

    // second message
    void* block2 = pool.allocate();

    if(block2 == nullptr){
        throw std::runtime_error("Memory pool not available");
    }

    std::cout << "pool.freeBlocks() after allocation == " << pool.freeBlocks() << '\n';          // unit_test

    //Construct SensorMessage in pool Block
    auto* message2 = static_cast<SensorMessage*>(block2);

    message2->sensorID = 2;
    message2->value = 2.25F;

    // Producer: send message pointer to MessageQueue, if failed, return block to the pool
    if(!queue.send(&message2, sizeof(message2))) {
        pool.deallocate(message2);
        std::cout << "pool.freeBlocks() after queue.send() failed == " << pool.freeBlocks() << '\n';    //unit_test         
        throw std::runtime_error("Send failed, queue not updated");
    }

    std::cout << "queue.size() after successful send == " << queue.size() << '\n';          // unit_test 

    // third message
    void* block3 = pool.allocate();

    if(block3 == nullptr){
        throw std::runtime_error("Memory pool not available");
    }

    std::cout << "pool.freeBlocks() after allocation == " << pool.freeBlocks() << '\n';          // unit_test

    //Construct SensorMessage in pool Block
    auto* message3 = static_cast<SensorMessage*>(block3);

    message3->sensorID = 2;
    message3->value = 2.25F;

    // Producer: send message pointer to MessageQueue, if failed, return block to the pool
    if(!queue.send(&message3, sizeof(message3))) {
        pool.deallocate(message3);
        std::cout << "pool.freeBlocks() after queue.send() failed == " << pool.freeBlocks() << '\n';    //unit_test         
        std::cout << "Send failed, queue not updated" << '\n';
    }

    std::cout << "queue.size() after successful send == " << queue.size() << '\n';          // unit_test
         

    // Consumer: Receive message pointer
    SensorMessage* receivedMessage = nullptr;
    std::size_t receivedLength = 0;

    while(queue.receive(
        &receivedMessage, sizeof(receivedMessage), receivedLength)) {

            // Why constexpr? : Suppose later in the project, if message struct size grows, constexpr updates automatically with sizeof(SensorMessage)
            constexpr std::size_t kExptectedMessageSize = sizeof(SensorMessage*);

            // Validate receivedLength
            if(receivedLength != kExptectedMessageSize) {
                throw std::runtime_error("Unexpected queued pointer size");
            }

            std::cout << "queue.size() after successful receive == " << queue.size() << '\n';          // unit_test 

            //Format a log Message
            std::ostringstream stream;

            stream << "Sensor ID: " << receivedMessage->sensorID
                    << ", value: " << receivedMessage->value;
            
            // send it to Logger
            logger.log(stream.str());  

            // return block to the pool
            pool.deallocate(receivedMessage);

            std::cout << "pool.freeBlocks() after consumer deallocation == " << pool.freeBlocks() << '\n';          // unit_test 
    }
    
    // Output received data 
    if((queue.isEmpty())) {
        logger.flush();
    }

    return 0;
}