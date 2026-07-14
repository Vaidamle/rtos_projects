/* MemoryPool.cpp
APIs definition for Memory allocation/deallocation module 
*/
#include "MemoryPool.hpp"

#include <stdexcept>
#include <limits>

MemoryPool::MemoryPool(std::size_t blockSize, std::size_t blockCount) :
    blockSize_(blockSize), blockCount_(blockCount), freeBlockCount_(blockCount)
{
    /* Configuration validation */
   // check block count (should be atleast 1)
    if(blockCount_ == 0){
        throw std::invalid_argument("MemoryPool Block count cannot be zero");
    }

    // block size (should atleast contain next block pointer)
    if(blockSize_ < sizeof(FreeBlock)){
        throw std::invalid_argument("MemoryPool block size is too small");
    }

    //block alignment
    if(blockSize_ % alignof(FreeBlock) != 0){
        throw std::invalid_argument("MemoryPool block size is not properly aligned");
    }

    // Protect against multiplication overflow
    if(blockCount_ > std::numeric_limits<std::size_t>::max() / blockSize_) {
        throw std::overflow_error("MemoryPool total size overflow");
    }

    // allocate the continuous storage
    const std::size_t totalSize = blockSize_ * blockCount_;
    memory_.resize(totalSize);

    std::uint8_t* base = memory_.data();        // pointing to start of memory area

    // create block links
    for (std::size_t index = 0; index < blockCount_; ++index) {
        auto* currentBlock = reinterpret_cast<FreeBlock*>(base + (index * blockSize_)); // calculate block address

        if(index + 1 < blockCount_) {
            currentBlock -> next = reinterpret_cast<FreeBlock*>(base + (index + 1) * blockSize_);
        }
        else {
            currentBlock -> next = nullptr;
        }
    }

    freeListHead_ = reinterpret_cast<FreeBlock*> (base);
}

// how many free blocks available?
std::size_t MemoryPool::freeBlocks() const
{
    return freeBlockCount_;
}

// total number of blocks owned by the memory pool(blockCount)
std::size_t MemoryPool::capacity() const
{
    return blockCount_;
}

// how large is each block?
std::size_t MemoryPool::blockSize() const
{
    return blockSize_;
}

// configuration check : requested ptr belong to memory pool area
bool MemoryPool::owns(const void* ptr) const
{
    // check for nullptr
    if(ptr == nullptr) {
        return false;
    }

    //check block address is within memory range
    const auto ptrAddress = reinterpret_cast<std::uintptr_t>(ptr);

    const auto begin = reinterpret_cast<std::uintptr_t>(memory_.data());
    const auto end = begin + memory_.size();

    return(ptrAddress >= begin && ptrAddress < end);
}

// configuration check : requested ptr is aligned to the start of memory block
bool MemoryPool::isBlockBoundary(const void* ptr) const
{
    if(!owns(ptr)){
        return false;
    }

    const auto ptrAddress = reinterpret_cast<std::uintptr_t>(ptr);

    const auto begin = reinterpret_cast<std::uintptr_t>(memory_.data());

    const std::size_t offset = reinterpret_cast<std::size_t>(ptrAddress - begin);

    return (offset % blockSize_ == 0);
    
}

// configuration check : requested block is not present in the free list already
bool MemoryPool::isAlreadyFree(const void* ptr) const
{

    for(FreeBlock* current = freeListHead_; current != nullptr; current = current->next){
        if(current == ptr){
            return true;
        }
    }
    return false; 
}

void* MemoryPool::allocate()
{
    if(freeListHead_ == nullptr) {
        return nullptr;
    }
    
    // Allocate block from the free list head
    FreeBlock* allocatedBlock = freeListHead_;
    freeListHead_ =  freeListHead_->next;

    // decrement free block count
    --freeBlockCount_;

    return allocatedBlock;
}

bool MemoryPool::deallocate (void *ptr){

    // ptr ownership validation
    if(!owns(ptr)){
        return false;
    }

    // Block boundary validation
    if(!isBlockBoundary(ptr)){
        return false;
    }

    // Double-free validation
    if(isAlreadyFree(ptr)){
        return false;
    }

    // Insert block at the free list head
    auto* returnedBlock = static_cast<FreeBlock*>(ptr);
    returnedBlock->next = freeListHead_;
    freeListHead_ = returnedBlock;

    // inrement free block count
    ++freeBlockCount_;                      
    return true;
}
