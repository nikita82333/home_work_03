#ifndef ITERATOR_H
#define ITERATOR_H

template<typename T>
class Iterator : public std::iterator<std::input_iterator_tag, T> {
public:
    explicit Iterator(T* data): _data(data) {}
    Iterator(const Iterator &it): _data(it._data) {}

    Iterator& operator=(const Iterator &it) {
        _data = it._data;
        return *this;
    }
    bool operator==(const Iterator &it) const {
        return _data == it._data;
    }
    bool operator!=(const Iterator &it) const {
        return _data != it._data;
    }
    Iterator& operator++() {
        ++_data;
        return *this;
    }
    Iterator& operator--() {
        --_data;
        return *this;
    }
    T& operator*() const {
        return *_data;
    }

private:
    T* _data = nullptr;
};

#endif //ITERATOR_H
