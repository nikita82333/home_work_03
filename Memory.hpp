#ifndef MEMORY_H
#define MEMORY_H

#include <list>
#include <cmath>

#include "Block.hpp"

template <typename T, std::size_t BLOCK_SIZE>
class Memory {
public:
    Memory() = default;
    ~Memory() = default;
    T* allocate(std::size_t size);
    void deallocate(T* ptr, std::size_t size);

private:
    std::list<Block<T>> _blocks;
};

template <typename T, std::size_t BLOCK_SIZE>
T* Memory<T, BLOCK_SIZE>::allocate(std::size_t size) {
    T* ptr = nullptr;
    for (auto& block : _blocks) {
        ptr = block.try_allocate(size);
        if (ptr != nullptr) {
            break;
        }
    }
    if (ptr == nullptr) {
        std::size_t multiplier = std::ceil((double)size / BLOCK_SIZE);
        _blocks.emplace_back(BLOCK_SIZE * multiplier);
        ptr = _blocks.back().try_allocate(size);
    }
    return ptr;
}

template <typename T, std::size_t BLOCK_SIZE>
void Memory<T, BLOCK_SIZE>::deallocate(T* ptr, std::size_t size) {
    if (ptr == nullptr || size == 0) {
        return;
    }
    for (auto& block : _blocks) {
        if (block.is_inside(ptr)) {
            block.deallocate(ptr, size);
            break;
        }
    }
}

#endif //MEMORY_H
