/* RingBuffer.cpp
APIs definition for RingBuffer module 
*/
#include "RingBuffer.hpp"

#include <stdexcept>

RingBuffer::RingBuffer(std::size_t capacity) : buffer_(capacity)
{
    if(capacity == 0 )
    {
        throw std::invalid_argument("RingBuffer capacity cannot be zero");
    }
}

bool RingBuffer::push(std::uint8_t byte)
{
    if(isFull())
    {
        return false;
    }

    buffer_[head_] = byte;
    head_ = (head_ + 1) % buffer_.size();
    ++size_;

    return true;
}

bool RingBuffer::push(const std::uint8_t* data, std::size_t length)
{
    // Check if buffer full / nullptr / not enough free space for requested data -> return false
    if(isFull() || ((data == nullptr) && (length > 0)) || (length > freeSpace()))
    {
        return false;
    }  

    // push whole data stream into buffer byte-by-byte
    for (std::size_t i = 0 ; i < length; ++i)
    {
        push(data[i]);
    }

    return true;
}

bool RingBuffer::pop(std::uint8_t& byte)
{
    if(isEmpty())
    {
        return false;
    }

    byte = buffer_[tail_];
    tail_ = (tail_ + 1) % buffer_.size();
    --size_;

    return true;
}

bool RingBuffer::isFull() const
{
    return size_ == buffer_.size();
}

bool RingBuffer::isEmpty() const{
    return size_ == 0;
}

std::size_t RingBuffer::size() const
{
    return size_;
}

std::size_t RingBuffer::capacity() const
{
    return buffer_.size();
}

std::size_t RingBuffer::freeSpace() const
{
    return (buffer_.size() - size_);
}

