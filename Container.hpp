#ifndef CONTAINER_H
#define CONTAINER_H

#include <stdexcept>

#include "Iterator.hpp"

template<typename T, typename Allocator = std::allocator<T>>
class Container {
public:

    using iterator = Iterator<T>;
    using const_iterator = const Iterator<T>;

    Container() : _size(0), _capacity(0), _storage(nullptr) {}
    Container(std::size_t size, const T& value = {}, std::size_t capacity = START_CAPACITY);
    Container(const Container& value);
    Container& operator=(const Container& value);
    Container(Container&& value) noexcept;
    Container(const T& value);
    Container(T&& value);
    ~Container();

    void reserve(std::size_t new_capacity);
    void resize(std::size_t new_size, const T& value = {});
    void add(const T& value);
    void add(T&& value);
    T& at(std::size_t pos);
    const T& at(std::size_t pos) const;

    T& operator[](std::size_t pos) {
        return _storage[pos];
    }
    const T& operator[](std::size_t pos) const {
        return _storage[pos];
    }
    iterator begin() {
        return iterator(_storage);
    }
    iterator end() {
        return iterator(_storage + _size);
    }
    const_iterator begin() const {
        return const_iterator(_storage);
    }
    const_iterator end() const {
        return const_iterator(_storage + _size);
    }
    std::size_t size() const {
        return _size;
    }
    bool empty() const {
        return _size == 0;
    }

private:
    static constexpr std::size_t START_CAPACITY {8};
    std::size_t _size {0};
    std::size_t _capacity {0};
    Allocator _allocator;
    T* _storage {nullptr};
};

template<typename T, typename Allocator>
Container<T, Allocator>::Container(std::size_t size, const T& value, std::size_t capacity)
        : _size(size), _capacity(capacity)
{
    _storage = _allocator.allocate(_capacity);
    for (std::size_t i = 0; i < _size; ++i) {
        _allocator.construct(&_storage[i], value);
    }
}

template<typename T, typename Allocator>
Container<T, Allocator>::Container(const Container& value) : _size(value._size), _capacity(value._capacity) {
    _storage = _allocator.allocate(_capacity);
    for (std::size_t i = 0; i < _size; ++i) {
        _allocator.construct(&_storage[i], value._storage[i]);
    }
}

template<typename T, typename Allocator>
Container<T, Allocator>& Container<T, Allocator>::operator=(const Container& value) {
    if (this == &value) {
        return *this;
    }
    for (std::size_t i = 0; i < _size; ++i) {
        _allocator.destroy(&_storage[i]);
    }
    _size = 0;
    if (value._size > _capacity) {
        reserve(value._capacity);
    }
    _size = value._size;
    for (std::size_t i = 0; i < _size; ++i) {
        _allocator.construct(&_storage[i], value._storage[i]);
    }
    return *this;
}

template<typename T, typename Allocator>
Container<T, Allocator>::Container(Container&& value) noexcept : _size(value._size), _capacity(value._capacity)  {
    _storage = _allocator.allocate(_capacity);
    for (std::size_t i = 0; i < _size; ++i) {
        _allocator.construct(&_storage[i], std::forward<T>(value._storage[i]));
    }
    value._allocator.deallocate(value._capacity);
    value._size = 0;
    value._capacity = 0;
    value._storage = nullptr;
}

template<typename T, typename Allocator>
Container<T, Allocator>::Container(const T& value) {
    reserve(START_CAPACITY);
    _allocator.construct(_storage, value);
    _size = 1;
}

template<typename T, typename Allocator>
Container<T, Allocator>::Container(T&& value) {
    reserve(START_CAPACITY);
    _allocator.construct(_storage, std::forward<T>(value));
    _size = 1;
}

template<typename T, typename Allocator>
Container<T, Allocator>::~Container() {
    for (std::size_t i = 0; i < _size; i++) {
        _allocator.destroy(&_storage[i]);
    }
    _allocator.deallocate(_storage, _capacity);
    _storage = nullptr;
}

template<typename T, typename Allocator>
void Container<T, Allocator>::reserve(std::size_t new_capacity) {
    if (new_capacity <= _capacity) {
        return;
    }
    T* ptr = _allocator.allocate(new_capacity);
    for (std::size_t i = 0; i < _size; ++i) {
        _allocator.construct(&ptr[i], _storage[i]);
        _allocator.destroy(&_storage[i]);
    }
    if (_storage != nullptr) {
        _allocator.deallocate(_storage, _capacity);
    }
    _storage = ptr;
    _capacity = new_capacity;
}

template<typename T, typename Allocator>
void Container<T, Allocator>::resize(std::size_t new_size, const T& value) {
    reserve(new_size);
    for (std::size_t i = _size; i < new_size; ++i) {
        _allocator.construct(&_storage[i], value);
    }
    for (std::size_t i = new_size; i < _size; ++i) {
        _allocator.destroy(&_storage[i]);
    }
    _size = new_size;
}

template<typename T, typename Allocator>
void Container<T, Allocator>::add(const T& value) {
    if (_capacity == 0) {
        reserve(START_CAPACITY);
    } else if (_size == _capacity) {
        reserve(2 * _capacity);
    }
    _allocator.construct(&_storage[_size], value);
    ++_size;
}

template<typename T, typename Allocator>
void Container<T, Allocator>::add(T&& value) {
    if (_capacity == 0) {
        reserve(START_CAPACITY);
    } else if (_size == _capacity) {
        reserve(2 * _capacity);
    }
    _allocator.construct(&_storage[_size], std::forward<T>(value));
    ++_size;
}

template<typename T, typename Allocator>
T& Container<T, Allocator>::at(std::size_t pos) {
    if (pos >= _size) {
        throw std::out_of_range("Index out of range: " + std::to_string(pos));
    }
    return _storage[pos];
}

template<typename T, typename Allocator>
const T& Container<T, Allocator>::at(std::size_t pos) const {
    if (pos >= _size) {
        throw std::out_of_range("Index out of range: " + std::to_string(pos));
    }
    return _storage[pos];
}

#endif //CONTAINER_H
