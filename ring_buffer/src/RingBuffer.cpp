#include "RingBuffer.hpp"

#include <stdexcept>

RingBuffer::RingBuffer(std::size_t capacity) : buffer_(capacity)
{
    if(capacity == 0 )
    {
        throw std::invalid_argument("RingBUffer capacity cannot be zero");
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
    ++count_;

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
    --count_;

    return true;
}

bool RingBuffer::isFull() const
{
    return count_ == buffer_.size();
}

bool RingBuffer::isEmpty() const{
    return count_ == 0;
}

std::size_t RingBuffer::size() const
{
    return count_;
}

std::size_t RingBuffer::capacity() const
{
    return buffer_.size();
}

