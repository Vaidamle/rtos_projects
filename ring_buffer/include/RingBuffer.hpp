/* RingBUffer.hpp
APIs declaration for RingBuffer module 
*/
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class RingBuffer {
    public:
    explicit RingBuffer(std::size_t capacity);

    bool push(const std::uint8_t* data, std::size_t length);    // Atomic bulk data insertion 
    bool push(std::uint8_t byte);                               // Single byte write
    bool pop(std::uint8_t& byte);                               // Single byte read

    bool isFull() const;
    bool isEmpty() const;

    std::size_t size() const;
    std::size_t capacity() const;
    std::size_t freeSpace() const;

    private:
    std::vector<std::uint8_t> buffer_;
    std::size_t head_{0};                   // next write position
    std::size_t tail_{0};                   // next read position
    std::size_t count_{0};                   // # of elements present

};
