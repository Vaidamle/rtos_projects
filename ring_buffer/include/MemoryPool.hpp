/* MemoryPool.hpp
APIs declaration for Memory allocation module 

State invariants
1. 0 <= freeBlockCount <= blockCount_
2. freeListHead_ == nullptr when freeBlockCount_ == 0
3. memory_.size() == blockSize_ * blockCount_
4. Every node in the free list points inside memory_
*/
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class MemoryPool {
    public:
    MemoryPool(std::size_t blockSize, std::size_t blockCount);

    //allocation/deallocation APIs
    void* allocate(void);
    bool deallocate (void *ptr);

    //supporting APIs
    std::size_t freeBlocks() const;         // how many free blocks available?
    std::size_t capacity() const;           // total number of blocks owned by the memory pool(blockCount)     
    std::size_t blockSize() const;          // how large is each block?

    private:
    struct FreeBlock
    {
        /* data */
        FreeBlock* next;
    };

    bool owns(const void* ptr) const;       // configuration check : requested ptr belong to memory pool area
    bool isBlockBoundary(const void* ptr) const; // configuration check : requested ptr is aligned to the start of memory block
    bool isAlreadyFree(const void* ptr) const;      // configuration check : requested block is not present in the free list already

    std::vector<std::uint8_t> memory_;      // memory area to store blocks

    FreeBlock* freeListHead_ {nullptr};     // Free list head; points to first available block

    std::size_t blockSize_{0};              // blockSize_ >= sizeof(FreeBlock) (Should be large enough to hold pointer to next block)
    std::size_t blockCount_{0};             // never changes during pool's lifetime (==capacity)
    std::size_t freeBlockCount_{0};         // # of free block available (0 <= freeBlockCount <= blockCOunt_)
};