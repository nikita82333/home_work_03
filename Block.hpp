#ifndef BLOCK_H
#define BLOCK_H

template <typename T>
class Block {
public:
    explicit Block(std::size_t size);
    Block(Block&& other_block) noexcept;
    ~Block();
    T* try_allocate(std::size_t size);
    void deallocate(T* ptr, std::size_t size);
    bool is_inside(T* ptr);

private:
    std::size_t get_start_free_area_index(std::size_t size);

    std::size_t _size {0};
    std::size_t _object_count {0};
    std::size_t _first_free_cell {0};
    T* _data {nullptr};
    bool* _reserved {nullptr};
};

template <typename T>
Block<T>::Block(std::size_t size) : _size(size) {
    _data = new T[_size];
    _reserved = new bool[_size];
    if (!_data || !_reserved) {
        delete[] _data;
        delete[] _reserved;
        throw std::bad_alloc();
    }
    for (std::size_t i = 0; i < _size; ++i) {
        _reserved[i] = false;
        _data[i] = {};
    }
}

template <typename T>
Block<T>::Block(Block&& other_block) noexcept {
    _size = other_block._size;
    _object_count = other_block._object_count;
    _first_free_cell = other_block._first_free_cell;
    _data = other_block._data;
    _reserved = other_block._reserved;
    other_block._size = 0;
    other_block._object_count = 0;
    other_block._first_free_cell = 0;
    other_block._data = nullptr;
    other_block._reserved = nullptr;
}

template <typename T>
Block<T>::~Block() {
    delete [] _data;
    delete [] _reserved;
}

template <typename T>
T* Block<T>::try_allocate(std::size_t size) {
    if (size > _size - _object_count)
        return nullptr;

    std::size_t start_index = 0;
    if (_object_count != 0) {
        start_index = get_start_free_area_index(size);
        if (start_index == _size) {
            return nullptr;
        }
    }

    for (size_t i = start_index; i < start_index + size; ++i) {
        _reserved[i] = true;
    }
    if (_first_free_cell == start_index) {
        _first_free_cell += size;
        if (_reserved[_first_free_cell]) {
            _first_free_cell = get_start_free_area_index(1);
        }
    }
    _object_count += size;
    return _data + start_index;
}

template <typename T>
void Block<T>::deallocate(T* ptr, std::size_t size) {
    if (size > _object_count) {
        throw std::exception();//bad_deallocate
    }
    std::size_t index = ptr - _data;
    if (index + size > _size) {
        throw std::exception();//bad_deallocate
    }
    for (size_t i = index; i < index + size; ++i) {
        _reserved[i] = false;
    }
    _object_count -= size;
    if (index < _first_free_cell) {
        _first_free_cell = index;
    }
}

template <typename T>
bool Block<T>::is_inside(T* ptr) {
    return ptr >= _data && ptr <= _data + _size - 1;
}

template <typename T>
std::size_t Block<T>::get_start_free_area_index(std::size_t size) {
    std::size_t counter = 0;
    std::size_t return_index = _size;//will be return in case of fail
    std::size_t start_index = _first_free_cell;
    for (std::size_t i = _first_free_cell; i < _size; ++i) {
        if (!_reserved[i]) {
            if (counter == 0) start_index = i;
            ++counter;
            if (counter == size) {
                return_index = start_index;
                break;
            }
        } else {
            counter = 0;
        }
    }
    return return_index;
}

#endif //BLOCK_H
