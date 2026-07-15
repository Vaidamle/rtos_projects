/* MessageQueue.hpp
APIs declaration for MessageQueue module 

State invariants
1. 0 <= count_ <= capacity_
2. head_ is the next readable slot
3. tail_ is the next writable slot
4. messageLengths_[i] <= maxMessageSize_
5. A failed send or receive must not change queue state
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class MessageQueue {
    public:
    MessageQueue (std::size_t capacity, std::size_t maxMessageSize);

    bool send(const void* data, std::size_t length);
    bool receive (void* destination, 
                    std::size_t destinationSize, 
                    std::size_t& receivedLength);
    bool isEmpty() const;
    bool isFull() const;

    std::size_t size() const;
    std::size_t capacity() const;
    std::size_t maxMessageSize() const;
    std::uint8_t* slotAddress(std::size_t index);
    const std::uint8_t* slotAddress(std::size_t index) const;

    private:
    std::vector<std::uint8_t> storage_;
    std::vector<std::size_t> messageLengths_;

    std::size_t capacity_{0};
    std::size_t maxMessageSize_{0};

    std::size_t head_{0};
    std::size_t tail_{0};
    std::size_t count_{0};

};