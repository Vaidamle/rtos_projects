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
    /* Producer-consumer flow (single-threaded) 
        Producer:
            create SensorMessage objects
            send them to MessageQueue

        Consumer:
            receive messages in FIFO order
            validate receivedLength why? : Producer is sending data sizeof(SensorMessage). 
                We want to ensure Receiver received exacly one complete SensorMessage
            log sensorId and value
    */
    
    // Producer: create objects : Sensor message, Message queue, Logger
    SensorMessage first{1, 1.25F};
    SensorMessage second{2, 2.50F};

    MessageQueue queue(3, sizeof(SensorMessage));
    Logger logger(256);

    // Producer: send messages to MessageQueue
    if(!queue.send(&first, sizeof(first))) {
        throw std::runtime_error("Send failed, queue not updated");
    }
    if(!queue.send(&second, sizeof(second))) {
        throw std::runtime_error("Send failed, queue not updated");
    }

    // Consumer: Receive messages in FIFO order
    SensorMessage received{};
    std::size_t receivedLength{};

    while(queue.receive(
        &received, sizeof(received), receivedLength)) {

            // Why constexpr? : Suppose later in the project, if message struct size grows, constexpr updates automatically with sizeof(SensorMessage)
            constexpr std::size_t kExptectedMessageSize = sizeof(SensorMessage);

            // Validate receivedLength
            if(receivedLength != kExptectedMessageSize) {
                throw std::runtime_error("Unexpected message length");
            }

            //Format a log Message
            std::ostringstream stream;

            stream << "Sensor ID: " << received.sensorID
                    << ", value: " << received.value;
            
            // send it to Logger
            logger.log(stream.str());  
    }
    
    // Output received data 
    if((queue.isEmpty())) {
        logger.flush();
    }

    return 0;
}