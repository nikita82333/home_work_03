#ifndef BLOCKALLOCATOR_H
#define BLOCKALLOCATOR_H

#include "Memory.hpp"

template <typename T, std::size_t BLOCK_SIZE = 10>
class BlockAllocator {
public:
    using value_type = T;

    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    template <typename U>
    struct rebind {
        using other = BlockAllocator<U, BLOCK_SIZE>;
    };

    BlockAllocator() = default;
    ~BlockAllocator() = default;

    template <typename U>
    BlockAllocator(const BlockAllocator<U, BLOCK_SIZE> &) {}

    T *allocate(std::size_t n) {
        std::cout << "allocate: [n = " << n << "]" << std::endl;
        T* ptr = _memory.allocate(n);
        return ptr;
    }

    void deallocate(T *p, std::size_t n) {
        std::cout << "deallocate: [n = " << n << "]" << std::endl;
        _memory.deallocate(p, n);
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&...args) {
        std::cout << "construct" << std::endl;
        new (p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U *p) {
        std::cout << "destroy" << std::endl;
        p->~U();
    }

private:
    Memory<T, BLOCK_SIZE> _memory;
};

#endif //BLOCKALLOCATOR_H
