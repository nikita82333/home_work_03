#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
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
    std::vector<Block<T>> _blocks;
};

template <typename T, std::size_t BLOCK_SIZE>
T* Memory<T, BLOCK_SIZE>::allocate(std::size_t size) {
    std::size_t multiplier = 1;
    if (size > BLOCK_SIZE) {
        multiplier = std::round(size / BLOCK_SIZE);
        if (size % BLOCK_SIZE != 0) {
            ++multiplier;
        }
    }
    T* ptr = nullptr;
    for (auto& block : _blocks) {
        if (block.can_allocate(size)) {
            ptr = block.allocate(size);
            break;
        }
    }
    if (ptr == nullptr) {
        _blocks.emplace_back(Block<T>(BLOCK_SIZE * multiplier));
        ptr = _blocks.back().allocate(size);
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
