/* MessageQueue.cpp
APIs definition for MessageQueue module 
*/
#include "MessageQueue.hpp"

#include <stdexcept>
#include <limits>
#include <cstring>

/* MessageQueue::constructor()
    reject capacity == 0
    reject maxMessageSize == 0
    check multiplication overflow for capacity * maxMessageSize
    allocate capacity * maxMessageSize bytes in storage_
    allocate capacity entries in messageLengths_
    leave head_, tail_, and count_ at zero
*/
MessageQueue::MessageQueue (std::size_t capacity, std::size_t maxMessageSize) :
    capacity_(capacity), maxMessageSize_(maxMessageSize)
{
    /* Configuration validation */
   // check capacity (should be atleast 1)
    if(capacity_ == 0){
        throw std::invalid_argument("MessageQueue capacity cannot be zero");
    }

    // check maxMessageSize (should be atleast 1)
    if(maxMessageSize_ == 0){
        throw std::invalid_argument("MessageQueue max message size cannot be zero");
    }

    // Protect against multiplication overflow
    if(capacity_ > std::numeric_limits<std::size_t>::max() / maxMessageSize_) {
        throw std::overflow_error("MessageQueue storage size overflow");
    }

    // allocate the MessageQueue storage
    storage_.resize(capacity_ * maxMessageSize_);
    messageLengths_.resize(capacity_);

}

/* Helper APIs*/

bool MessageQueue::isEmpty() const
{
    return (count_ == 0);
}

bool MessageQueue::isFull() const
{
    return (count_ == capacity_);
}

std::size_t MessageQueue::size() const
{
    return count_;
}

std::size_t MessageQueue::capacity() const
{
    return capacity_;
}

std::size_t MessageQueue::maxMessageSize() const
{
    return maxMessageSize_;
}

std::uint8_t* MessageQueue::slotAddress(std::size_t index){
    return storage_.data() + index * maxMessageSize_;
}

const std::uint8_t* MessageQueue::slotAddress(std::size_t index) const{
    return storage_.data() + index * maxMessageSize_;
}

/* MessageQueue::send()
    Input validation checks
    calculate destination slot
    copy message bytes
    store message length
    advance tail
    increment count
    return true
*/
bool MessageQueue::send(const void* data, std::size_t length)
{
    /* Input validation checks*/
    if(data == nullptr) {
        return false;
    }

    if(length == 0) {
        return false;
    }

    if(length > maxMessageSize_) {
        return false;
    }

    if(isFull()) {
        return false;
    }

    // find the current tail slot and copy data
    auto* currentSlot = slotAddress(tail_);
    std::memcpy(currentSlot, data, length);
    messageLengths_[tail_] = length;
    tail_ = (tail_ + 1) % capacity_;
    ++count_;
    return true;

}

/* MessageQueue::Receive()
    Read the message length at head_.
    Find the current head slot.
    Copy exactly that many bytes.
    Write the copied length into receivedLength.
    Advance head_ with wraparound.
    Decrement count_.
    Return true.
*/
bool MessageQueue::receive (void* destination, std::size_t destinationSize, std::size_t& receivedLength)
{
    /* Input validation checks*/
    if(destination == nullptr) {
        return false;
    }

    if(isEmpty()) {
        return false;
    }

    if(destinationSize < messageLengths_[head_]) {
        return false;
    }

    // Read the message length at head_
    const std::size_t messageLength = messageLengths_[head_];

    // Find the current head slot
    const auto* currentReadSlot = slotAddress(head_);

    // Copy exactly that many bytes
    std::memcpy(destination, currentReadSlot, messageLength);

    // update receivedLength
    receivedLength = messageLength;

    // Advance head_ with wraparound
    head_ = (head_ + 1) % capacity_;

    // Decrement count_
    --count_;

    // Return true
    return true;

}
                    
